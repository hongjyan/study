#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.4
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
    print "usage: python setUP.py [--up|--down|--both]\nif no opotion, then --both will be used"

#Setup Cases
def get_up_parameter(filename):
    lib.lib_FS.FSetupVarParser.parsevars(filename)
    dict = lib.lib_FS.FSetupVarParser.datacontent
    up_dict = {}
    temp_list = []
    up_items = ('IPNIURG','IPNIUFU','UP_ROUTE', 'UP_TW')
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if lib.find_equal(j, up_items): # find the fittest up items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if up_dict.has_key(j):
                up_dict[j].append(temp_list) # if this item class is already one of cp_dict's keys, append list to this key's value(it is a list) 
            else:
                up_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 

    return up_dict

up_dict = get_up_parameter(filename)

def UP_pingable():
    global up_dict
    IPNIURG_address_list = []
    IPNIUFU_address_list = []
    IPNNode_list = []
    TW_list = []
    for l in up_dict['IPNIURG']:
        IPNIURG_address_list.append(l[2])
    if up_dict.has_key('IPNIUFU'):
        for l in up_dict['IPNIUFU']:
            IPNIUFU_address_list.append((l[0],l[2])) #IPNIUFU_address_list's element is a tuple like: (/IPNI1P-0/MGW_IPNIUFU-0, 10.69.9.82)
    for l in up_dict['UP_ROUTE']:
        IPNNode_list.append(l[0])
    for l in up_dict['UP_TW']:
        TW_list.append(l[0])

    IPNNode_list = list(set(IPNNode_list)) #unique IPN Node 

    for rg in IPNIURG_address_list:
        for tw in TW_list:
            if (re.search("\.", tw) and re.search(":", rg)) or (re.search(":", tw) and re.search("\.", rg)):
                continue         #if tw address formart is different from rg/RU address, then continue.
            flag = False
            for node in IPNNode_list:
                if lib.pingable_from_ip(node, rg, tw):
                    flag = True
                    break #if TW is pingable from IPNIURG on any one node, break to go to next TW 
            if not flag: #if TW is not pingable from IPNIURG on anyone node, return False
                return False

    pingable_IPNIUFU_list = []
    for fu in IPNIUFU_address_list:
        flag = True
        for tw in TW_list:
            if (re.search("\.", tw) and re.search(":", rg)) or (re.search(":", tw) and re.search("\.", rg)):
                continue         #if tw address formart is different from rg/RU address, then continue.
            if not lib.pingable_from_ip(fu[0].split('/')[1], fu[1], tw):
                flag = False
        if flag:
            pingable_IPNIUFU_list.append(fu)
    
    if not len(pingable_IPNIUFU_list)*2 == len(IPNIUFU_address_list): # if all IP address on one node of node pair are pingable to all TW addresses, return true
         return False
            
            
    return True # if all TW is pingable from all IPNIURG on any one node, return True.
    

def add_UP():
    global up_dict
    up_items = ('IPNIURG','IPNIUFU','UP_ROUTE')
    up_functions = {'IPNIURG':lib.add_ip, 'IPNIUFU':lib.add_ip, 'UP_ROUTE':lib.add_route}
    try:
        for i in range(len(up_items)): # config up in sequence as defined in up_items
            if up_dict.has_key(up_items[i]): # if item of up_items exist in up_dict, then config it
                for l in up_dict[up_items[i]]:
                    func = up_functions.get(up_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False 
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of cp_items maybe not exist. then go to next item
                pass
        
        if not UP_pingable():
            return False 

        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, attention!!!")


def delete_UP():
    global up_dict
    up_items = ('IPNIURG','IPNIUFU','UP_ROUTE')
    up_functions = {'IPNIURG':lib.delete_ip, 'IPNIUFU':lib.delete_ip, 'UP_ROUTE':lib.delete_route}
    try:
        for i in range(len(up_items)): # config up in sequence as defined in up_items
            if up_dict.has_key(up_items[i]): # if item of up_items exist in up_dict, then config it
                for l in up_dict[up_items[i]]:
                    func = up_functions.get(up_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of cp_items maybe not exist. then go to next item
                pass

        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, attention!!!")



####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_up(mode):
    lib.bus_log('\n=====TestCase for UP=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_UP)
    elif mode == 'down':
        lm.call(delete_UP)
    elif mode == 'both':
        lm.call(add_UP)
        lm.call(delete_UP)



def main():
    if len(sys.argv) == 1:
        set_up('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_up('up')
        elif "--down" == sys.argv[1]:
            set_up('down')
        elif "--both" == sys.argv[1]:
            set_up('both')
        else:
            help()
            sys.exit(-1)

if __name__ == "__main__":
    #global filename
    #lib.lib_FS.FSetupVarParser.parsevars(filename)
    #cp_dict = get_cp_parameter(lib.lib_FS.FSetupVarParser.datacontent)
    #print cp_dict
    main()
elif not g_debug:
    wait_trigger()
    set_up('both')
    set_up('up')
    send_trigger()
else:
    pass
