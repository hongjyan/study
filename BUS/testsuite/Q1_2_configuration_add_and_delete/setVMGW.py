#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.6
# 2013.1.15 modify vmgw_pingable, opip-pip and  osip-sip should be pingable.
# 2013.1.22 sleep 1s to wait MGW go to idle state. PR 89690ESPE01: [BU UI5EP1] NO SCTP heartbeat after VMGW delete and add.
#=====================================================================
import sys
import re
import getopt
import time


#g_debug=False
g_debug=True
if g_debug:
    from boa_simulate import *

sys.path.append("../lib")  # not used in BOA
lib=boaimport('BU_BUS_Lib')
lib_RG = boaimport('BU_RG_lib')

filename = lib.get_paraFileName_for_different_cluster()

if g_debug:
        VERDICT_FAIL = 0
        VERDICT_PASS = 1
        def verdict(v):   #simulate funcation of BOA, Verdict case pass or fail
                if v == VERDICT_FAIL:
                        print 'FAIL'
                else:
                        print 'PASS'
 
        def cli(str):     #simulate funcation of BOA, issue command:str on MGW
                return os.popen(str).read() 

        def log(str):     #simulate funcation of BOA, log anything what you want 
                print str


        #simulate funcation of BOA,BOA will run corresponding code between wait_trigger
                #and send_trigger when there is a test procedure named TpAutomationTrigger in 
                #test case,More question, please ask yikun.huang@nsn.com
        def wait_trigger(): 
                print "end call execution"
                print "start script execution"
                print '.'
                print '.'
                print '.'

        def send_trigger():
                print "end script execution"
                print "start call execution"
                print '*'
                print '*'
                print '*'

        def stop_tc_execution():       #simulate funcation of BOA, stop test case execution, if there is some critical error happened.
                verdict(VERDICT_FAIL)
                sys.exit(-1)

def bus_cli(cmd):
    return lib.bus_cli(cmd)

def bus_fsclish(cmd):
    return lib.bus_fsclish(cmd)


def help():
    print "usage: python setVMGW.py [--up|--down|--both]\nif no opotion, then --both will be used"

   
def get_vmgw_parameter(filename):
    lib.lib_FS.FSetupVarParser.parsevars(filename)
    dict = lib.lib_FS.FSetupVarParser.datacontent
    vmgw_dict = {}
    temp_list = []
    vmgw_items = ('VMGW',)
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if lib.find_equal(j, vmgw_items): # find the fittest vmgw items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if vmgw_dict.has_key(j):
                vmgw_dict[j].append(temp_list) # if this item class is already one of vmgw_dict's keys, append list to this key's value(it is a list) 
            else:
                vmgw_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 
            
    return vmgw_dict

vmgw_dict = get_vmgw_parameter(filename)
 

def vmgw_pingable(vmn):
    out = bus_fsclish("show vmgw mgw vmn %s mod 0" %(vmn))
    #add strip() since BOA will add a "\n" after the end of any line
    opip = re.search("(own primary IP address)\s+:\s+(.*)",out).group(2).strip()
    osip = re.search("(own secondary IP address)\s+:\s+(.*)",out).group(2).strip()
    pip = re.search("(control primary IP address)\s+:\s+(.*)",out).group(2).strip()
    sip = re.search("(control secondary IP address)\s+:\s+(.*)",out).group(2).strip()
    rg = re.search("(HCLB rg)\s+:\s+(.*)",out).group(2).strip()
        
    nodes = lib_RG.get_nodes("/MGW_HCLBRG-%s" %(rg))

    flag = False
    for n in nodes:
        if (lib.pingable_from_ip(n, opip, pip) and lib.pingable_from_ip(n, osip, sip)): 
            flag = True
            break

    return flag


# define two functions; add_1vmgw&add_vmgw(delete_1vmgw&delete_vmgw) is: 1. other file can use add_1vmgw 2. add_vmgw is simple and use global variable vmgw_dict
   

def add_1vmgw(vmn, dur, rg, opip, osip, opn, pip, sip, hve, hea, pro=0):
    bus_fsclish("add vmgw mgw vmn %s dur %s rg %s opip %s osip %s opn %s" %(vmn, dur, rg, opip, osip, opn))
    bus_fsclish("add vmgw controller vmn %s pip %s sip %s hve %s hea %s pro %s" %(vmn, pip, sip, hve, hea, pro))
    bus_fsclish("set vmgw registration vmn %s rega 1" %(vmn))

    out = bus_fsclish("show vmgw mgw vmn %s mod 0" %(vmn))
    print out

    dur_show = re.search("(duration time)\s+:\s+(.*)",out).group(2)
    rg_show = re.search("(HCLB rg)\s+:\s+(.*)",out).group(2) 
    opip_show = re.search("(own primary IP address)\s+:\s+(.*)",out).group(2)
    osip_show = re.search("(own secondary IP address)\s+:\s+(.*)",out).group(2)
    opn_show = re.search("(own port number)\s+:\s+(.*)",out).group(2)
    pip_show = re.search("(control primary IP address)\s+:\s+(.*)",out).group(2)
    sip_show = re.search("(control secondary IP address)\s+:\s+(.*)",out).group(2)
    hve_show = re.search("(H.248 version)\s+:\s+(.*)",out).group(2)
    hea_show = re.search("(heartbeat for H.248)\s+:\s+(\d)",out).group(2)
    rega = re.search("(registration allowed)\s+:\s+(.*)",out).group(2)
    #add strip() since BOA will add a "\n" after the end of any line
    if not (dur.strip() == dur_show.strip() and rg.strip() == rg_show.strip() and opip.strip() == opip_show.strip()\
	 and osip.strip() == osip_show.strip() and opn.strip() == opn_show.strip() and pip.strip() == pip_show.strip() \
         and sip.strip() == sip_show.strip() and hve.strip() == hve_show.strip() and hea.strip() == hea_show.strip()\
         and rega.strip() == "registration allowed"):
        return False
    
    if vmgw_pingable(vmn):
        return True

    return False

def add_vmgw():
    global vmgw_dict
    global version
    if re.search('EP3', bus_cli("fsswcli -c")):
        version = 'UI5EP3'
    elif re.search('UI50_5.3', bus_cli("fsswcli -c")):
        version = 'UI5EP2'
    elif re.search('UI50_5.2', bus_cli("fsswcli -c")):
        version = 'UI5EP1'
    elif re.search('UI50_5.1', bus_cli("fsswcli -c")):
        version = 'UI5'    
    #sort VMGW list. Make vmgw name eg:Anna1 get vid 0, Anna2 get vid 1 
    vmgw_list = vmgw_dict['VMGW'][:]
    vmgw_list.sort()

    bus_fsclish("set mgw common service-endpoint-address e164 123456789")
    out = bus_fsclish("show mgw common parameter service-endpoint-address")
    if not re.search("123456789", out):
       return False 

    for l in vmgw_list:
        if not add_1vmgw(*l):
            return False
    
    return True 
    

def delete_1vmgw(vmn, dur, rg, opip, osip, opn, pip, sip, hve, hea, pro=0):
    bus_fsclish("set vmgw registration vmn %s rega 0" %(vmn))
    time.sleep(3)
    bus_fsclish("delete vmgw mgw vmn %s" %(vmn))
    out = bus_fsclish("show vmgw mgw vmn %s mod 0" %(vmn))
    if not re.search("virtual media gateway name", out):
        return True

    return False

def delete_vmgw():
    global vmgw_dict

    for l in vmgw_dict['VMGW']:
        if not delete_1vmgw(*l):
            return False

    bus_fsclish("delete mgw common service-endpoint-address")
    out = bus_fsclish("show mgw common parameter service-endpoint-address")
    if not re.search("not defined", out):
        return False

    return True



#############################################################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_vmgw(mode):
    lib.bus_log('\n=====TestCase for VMGW=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_vmgw)
    elif mode == 'down':
        lm.call(delete_vmgw)
    elif mode == 'both':
        lm.call(add_vmgw)
        lm.call(delete_vmgw)



def main():
    if len(sys.argv) == 1:
        set_vmgw('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_vmgw('up')
        elif "--down" == sys.argv[1]:
            set_vmgw('down')
        elif "--both" == sys.argv[1]:
            set_vmgw('both')
        else:
            help()
            sys.exit(-1)

if __name__ == "__main__":
    #for i in vmgw_dict:
    #	print "%s:%s\n" %(i, vmgw_dict[i])
    main()
elif not g_debug:
    wait_trigger()
    set_vmgw('both')
    set_vmgw('up')
    send_trigger()
else:
    pass
