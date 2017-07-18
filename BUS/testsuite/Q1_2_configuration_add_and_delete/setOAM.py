#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.11.6
#2013.1.8 NTP configuration support added
# Modified by Rene 2013.3.14.Added a new version parameter "EP2".
#2013.4.27 support add multi OAM_GW by YHJ.
#=====================================================================
import sys
import re
import getopt


#g_debug=False
g_debug=True
if g_debug:
    from boa_simulate import *

sys.path.append("../lib")  # not used in BOA
lib=boaimport('BU_BUS_Lib')


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
    print "usage: python setOAM.py [--up|--down|--both]\nif no opotion, then --both will be used"



   
def get_oam_parameter(filename):
    lib.lib_FS.FSetupVarParser.parsevars(filename)
    dict = lib.lib_FS.FSetupVarParser.datacontent
    oam_dict = {}
    temp_list = []
    oam_items = ('OAM_IP', 'OAM_GW', 'OAM_ROUTE', 'OAM_NTP')
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if lib.find_equal(j, oam_items): # find the fittest oam items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if oam_dict.has_key(j):
                oam_dict[j].append(temp_list) # if this item class is already one of oam_dict's keys, append list to this key's value(it is a list) 
            else:
                oam_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 
            
    return oam_dict

oam_dict = get_oam_parameter(filename)

def oam_pingable():
    global oam_dict
    version = bus_cli('fsswcli -c')
    if re.search("UI50", version):
        ssh_ip = bus_cli("fsclish -c \"show networking address owner /SharedIP\" |grep -v \"instance\"|grep \"address\"|awk -F: '{split($2,a,\"/\");print a[1]}'").strip("\n")
    else:
        ssh_ip = bus_cli("fsclish -c \"show networking address owner /SharedIP\" |grep -v \"instance\"|grep \"address\"|awk -F: '{split($2,a,\"/\");print a[1]}'").strip("\n")
    if lib.pingable_from_ip("CLA-0", ssh_ip, oam_dict['OAM_GW'][0][0]) or lib.pingable_from_ip("CLA-1", ssh_ip, oam_dict['OAM_GW'][0][0]):
        return True
    else:
        return False
    


def delete_oam():
    global oam_dict
    #delete ip 
    for l in oam_dict['OAM_IP']:
        if 0 == l.count("/SSH") and 0 == l.count("/SharedIP"): # won't delete SSH ip address to avoid disconnection to system
            if not lib.delete_ip(*l):
                return False
    #delete route
    for l in oam_dict['OAM_ROUTE']:
        value_l = l[:] #copy oam_dict['OAM_ROUTE'] to route_list, make global variable oam_dict unchanged.
        if 0 == value_l.count("10.140.0.0/16") and 0 == value_l.count("10.145.0.0/16"):  # won't delete office route to avoid disconnection to system
            for oam_gw in oam_dict['OAM_GW']:
                value_l.append(oam_gw[0])
                if not lib.delete_CLAroute(*value_l): # assume there is just one OAM_GW in config file
                    return False
                value_l = l[:]

    #delete ntp networking-service
    bus_fsclish("delete networking-service ntp")
    out = bus_fsclish("show networking-service ntp")
    if re.search(oam_dict['OAM_NTP'][0][0], out):
        return False

    return True

def add_oam():
    global oam_dict
    #add ip 
    for l in oam_dict['OAM_IP']:
        if not lib.add_ip(*l):
            return False
    #add route  
    for l in oam_dict['OAM_ROUTE']:
        value_l = l[:] #copy oam_dict['OAM_ROUTE'] to route_list, make global variable oam_dict unchang.
        for  oam_gw in oam_dict['OAM_GW']:
            value_l.append(oam_gw[0])
        #value_l.append(oam_dict['OAM_GW'][0][0])
            if not lib.add_CLAroute(*value_l):
                return False
            value_l = l[:] 
                
    #add ntp networking-service
    bus_fsclish("add networking-service ntp forwarder %s" %(oam_dict['OAM_NTP'][0][0]))
    out = bus_fsclish("show networking-service ntp")
    if not re.search(oam_dict['OAM_NTP'][0][0], out):
        return False

    if not oam_pingable():
        return False
            
    return True

#############################################################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_oam(mode):
    lib.bus_log('\n=====TestCase for OAM=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_oam)
    elif mode == 'down':
        lm.call(delete_oam)
    elif mode == 'both':
        lm.call(add_oam)
        lm.call(delete_oam)



def main():
    if len(sys.argv) == 1:
        set_oam('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_oam('up')
        elif "--down" == sys.argv[1]:
            set_oam('down')
        elif "--both" == sys.argv[1]:
            set_oam('both')
        else:
            help()
            sys.exit(-1)



if __name__ == "__main__":
    #for i in oam_dict:
    #	print "%s:%s\n" %(i, oam_dict[i])
    main()
elif not g_debug:
    wait_trigger()
    set_oam('both')
    set_oam('up')
    send_trigger()
else:
    pass
