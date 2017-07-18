#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
# 2013.1.7 first version
#=====================================================================
import sys
import re
import getopt
import time
g_debug = True
if g_debug:
    from boa_simulate import *

sys.path.append("../lib")  # not used in BOA

lib=boaimport('BU_BUS_Lib')
lib_RG=boaimport('BU_RG_lib')
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
    print "usage: python setIWF.py [--up|--down|--both]\nif no opotion, then --both will be used"

def get_iwf_parameter(filename):
    lib.lib_FS.FSetupVarParser.parsevars(filename)
    dict = lib.lib_FS.FSetupVarParser.datacontent
    iwf_dict = {}
    temp_list = []
    iwf_items = ('IWF_HCLBRG','IWF_ROUTE', 'IWF_HUNTANDENTRY')
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if lib.find_equal(j, iwf_items): # find the fittest iwf items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if iwf_dict.has_key(j):
                iwf_dict[j].append(temp_list) # if this item class is already one of iwf_dict's keys, append list to this key's value(it is a list) 
            else:
                iwf_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 

    return iwf_dict


iwf_dict = get_iwf_parameter(filename)


def iwf_pingable(prio, rg):
    out = bus_fsclish("show mgw iwf control-data prio %s rg %s" %(prio, rg))
    opip_show = re.search("(own primary IP address)\s+:\s+(.*)", out).group(2).strip()
    pip_show = re.search("(primary IWF IP address)\s+:\s+(.*)", out).group(2).strip()

    nodes = lib_RG.get_nodes("/MGW_HCLBRG-%s" %(rg))
    flag = False
    for node in nodes:
        if lib.pingable_from_ip(node, opip_show, pip_show):
            flag = True
            break
    
    return flag    



#Setup Cases
def add_IWF1(rg, opip, hunt, nename, pip, prio, pport, sctp):
    bus_fsclish("add mgw iwf unit-data rg %s opip %s hunt %s" %(rg, opip, hunt))
    bus_fsclish("add mgw iwf entry nename %s pip %s prio %s pport %s rg %s sctp-set-name %s" %(nename, pip, prio, pport, rg, sctp))
    bus_fsclish("set mgw iwf connection prio %s conn 0 rg %s" %(prio, rg))

    out = bus_fsclish("show mgw iwf control-data prio %s rg %s" %(prio, rg))
    rg_show = re.search("(HCLB recovery group index)\s+:\s+(.*)", out).group(2).strip().rstrip()
    opip_show = re.search("(own primary IP address)\s+:\s+(.*)", out).group(2).strip().rstrip()
    hunt_show = re.search("(hunting method)\s+:\s+(.*)", out).group(2).strip().rstrip()
    nename_show = re.search("(network element name)\s+:\s+(.*)", out).group(2).strip().rstrip()
    pip_show = re.search("(primary IWF IP address)\s+:\s+(.*)", out).group(2).strip().rstrip()
    prio_show = re.search("(IWF address priority)\s+:\s+(.*)", out).group(2).strip().rstrip()
    pport_show = re.search("(primary IWF IP port)\s+:\s+(.*)", out).group(2).strip().rstrip()
    sctp_show = re.search("(SCTP parameter set)\s+:\s+(.*)", out).group(2).strip().rstrip()
    rega_show = re.search("(registration allowed)\s+:\s+(.*)", out).group(2).strip().rstrip()
    regStatus_show = re.search("(registration status)\s+:\s+(.*)", out).group(2).strip().rstrip()
    
    if not (rg == rg_show and opip == opip_show  and nename == nename_show and pip == pip_show and prio == prio_show and pport == pport_show \
            and sctp == sctp_show and "registration allowed" == rega_show):
        log("iwf added failed")
        return False
    if not ((hunt == '1' and "cyclic" == hunt_show) or (hunt == '2' and "priority based" == hunt_show)):
        log("iwf hunt is wrong")
        return False

    if not iwf_pingable(prio, rg):
        log("ping is not able from hclb to mss")
        return False

    return True

#TearDown Cases
def delete_IWF1(rg, opip, hunt, nename, pip, prio, pport, sctp):
    bus_fsclish("set mgw iwf connection prio %s conn 4 rg %s" %(prio, rg))    
    bus_fsclish("delete mgw iwf entry prio %s rg %s" %(prio, rg))
    bus_fsclish("delete mgw iwf unit-data rg %s" %(rg))
    out = bus_fsclish("show mgw iwf control-data rg %s prio %s" %(rg, prio))
    
    if (4 == out.count("not defined")):
        return True
    else:
        return False 
    


#main fucation
def add_IWF():
    global iwf_dict
    iwf_items = ('IWF_HCLBRG','IWF_ROUTE', 'IWF_HUNTANDENTRY') # Add HCLBRG address if it doesn't exist
    iwf_functions = {'IWF_HCLBRG':lib.add_ip, 'IWF_ROUTE':lib.add_route, 'IWF_HUNTANDENTRY':add_IWF1}
    try:
        for i in range(len(iwf_items)): # config TDM in sequence as defined in iwf_items
            if iwf_dict.has_key(iwf_items[i]): # if item of iwf_items exist in iwf_dict, then config it
                for l in iwf_dict[iwf_items[i]]:
                    func = iwf_functions.get(iwf_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False 
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of iwf_items maybe not exist. then go to next item
                pass
        
        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, attion!!!")


def delete_IWF():
    global iwf_dict
    iwf_items = ('IWF_ROUTE', 'IWF_HUNTANDENTRY') #HCLB RG won't be deleted during IWF configuration
    iwf_functions = {'IWF_ROUTE':lib.delete_route, 'IWF_HUNTANDENTRY':delete_IWF1}
    try:
        for i in range(len(iwf_items)): # config iwf in sequence as defined in iwf_items
            if iwf_dict.has_key(iwf_items[i]): # if item of iwf_items exist in iwf_dict, then config it
                for l in iwf_dict[iwf_items[i]]:
                    func = iwf_functions.get(iwf_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of iwf_items maybe not exist. then go to next item
                pass

        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, attion!!!")



####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_iwf(mode):
    lib.bus_log('\n=====TestCase for IWF=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_IWF)
    elif mode == 'down':
        lm.call(delete_IWF)
    elif mode == 'both':
        lm.call(add_IWF)
        lm.call(delete_IWF)



def main():
    if len(sys.argv) == 1:
        set_iwf('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_iwf('up')
        elif "--down" == sys.argv[1]:
            set_iwf('down')
        elif "--both" == sys.argv[1]:
            set_iwf('both')
        else:
            help()
            sys.exit(-1)



if __name__ == "__main__":
    #global filename
    #lib.lib_FS.FSetupVarParser.parsevars(filename)
    #iwf_dict = get_iwf_parameter(lib.lib_FS.FSetupVarParser.datacontent)
    #print iwf_dict
    main()
elif not g_debug:
    wait_trigger()
    set_iwf('both')
    set_iwf('up')
    send_trigger()
else:
    pass
