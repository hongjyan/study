#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.28
# 2013.2.26 bug fix: False should be return if e164 is found after delete e164 address. by YHJ. 
# 2013.4.28 bug fix: True should be return if e164 have't found after delete e164 address, 
# or there isn't e164 address at first in delete_e164. By YHJ 
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
    print "usage: python setCP.py [--up|--down|--both]\nif no opotion, then --both will be used"

def get_cp_parameter(filename):
    lib.lib_FS.FSetupVarParser.parsevars(filename)
    dict = lib.lib_FS.FSetupVarParser.datacontent
    cp_dict = {}
    temp_list = []
    global version
    if re.search('EP3', bus_cli("fsswcli -c")):
        version = 'UI5EP3'
    elif re.search('UI50_5.3', bus_cli("fsswcli -c")):
        version = 'UI5EP2'
    elif re.search('UI50_5.2', bus_cli("fsswcli -c")):
        version = 'UI5EP1'
    else:
        version = 'UI5'

    cp_items = ('HCLBRG','CP_ROUTE', 'e164')
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if lib.find_equal(j, cp_items): # find the fittest cp items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if cp_dict.has_key(j):
                cp_dict[j].append(temp_list) # if this item class is already one of cp_dict's keys, append list to this key's value(it is a list) 
            else:
                cp_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 

    return cp_dict

cp_dict = get_cp_parameter(filename)

#Setup Cases
def set_e164(num):
    cmd = 'show mgw common'
    out = bus_fsclish(cmd)
    if re.search(num, out):
        return True
    cmd1 = 'set mgw common service-endpoint-address e164 ' + num
    out1 = bus_fsclish(cmd1)
    if re.search('Error', out1):
        return False
    return True


#TearDown Cases
def delete_e164(num): # num won't be used.
    cmd = 'show mgw common'
    out = bus_fsclish(cmd)
    if re.search('e164', out):
        cmd1 = 'delete mgw common service-endpoint-address'
        out1 = bus_fsclish(cmd1)
        if re.search('Error', out1):
            return False
    cmd2 = 'show mgw common'
    out2 = bus_fsclish(cmd2)
    if not re.search('e164', out2):
        return True
    else:
        return False




def cp_pingable():
    global cp_dict
#    #get TW's address corresponding to Node
#    tw_node_dict = {}
#    for l in cp_dict['CP_ROUTE']:
#        tw_node_dict[l[0]+l[3]] = l[1].split("/")[0] # initial the interfacce of node's TW, one TW is corresponding to one node+interface
#    
#    #One HCLB RG interface ip address should be pingable to one TW address at least on one of CLA/ISU node
#    for l in cp_dict['HCLBRG']:
#        flag = False
#        nodes = lib_RG.get_nodes(l[0]) #get HCLB RG location nodes
#        for node in nodes:
#            if lib.pingable_from_ip(node, l[2], tw_node_dict[node+l[1]]):
#                flag = True
#                break # if pingable on one of nodes, break, and go to if not flag.....
#            else:     #else, flag = False, test if pingable on next node 
#                flag = False    
#        if not flag: # if one HCLB RG interface IP address can't pingable to one TW, then stop to return False
#            return False
#    
#    return True
    tw_to_HCLBRG = {}
    for l in cp_dict['HCLBRG']:
        tw_to_HCLBRG[l[2]] = [l[0],] #intialize tw_to_HCLBRG.
    #update tw_to_HCLBRG according info of HCLBRG and CP_ROUTE
    for l in cp_dict['CP_ROUTE']:
        if l[3] in tw_to_HCLBRG:
            if l[1].split("/")[0] not in tw_to_HCLBRG[l[3]]: 
                tw_to_HCLBRG[l[3]].append(l[1].split("/")[0])
            else:
                continue
        else:                                           
            log("wrong ping source ip address in configuration file")
            return False 
    
    for i in tw_to_HCLBRG:
        flag = False
        nodes = lib_RG.get_nodes(tw_to_HCLBRG[i][0]) #get HCLB RG location nodes
        for j in tw_to_HCLBRG[i][1:]:
            for node in nodes:
                if lib.pingable_from_ip(node, i, j):
                    flag = True
                    break
                else:
                    flag = False
            if not flag:
                return False
    return True    
    

def add_CP():
    global cp_dict
    cp_items = ('HCLBRG','CP_ROUTE', 'e164')
    cp_functions = {'HCLBRG':lib.add_ip, 'CP_ROUTE':lib.add_CProute, 'e164':set_e164}
    try:
        for i in range(len(cp_items)): # config TDM in sequence as defined in cp_items
            if cp_dict.has_key(cp_items[i]): # if item of cp_items exist in cp_dict, then config it
                for l in cp_dict[cp_items[i]]:
                    func = cp_functions.get(cp_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False 
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of cp_items maybe not exist. then go to next item
                pass
        time.sleep(20)  # unclear reason here for waiting 20s
            
        if not cp_pingable():
            return False
    
        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, attion!!!")


def delete_CP():
    global cp_dict
    cp_items = ('HCLBRG','CP_ROUTE', 'e164')
    cp_functions = {'HCLBRG':lib.delete_ip, 'CP_ROUTE':lib.delete_route, 'e164':delete_e164}
    try:
        for i in range(len(cp_items)): # config TDM in sequence as defined in cp_items
            if cp_dict.has_key(cp_items[i]): # if item of cp_items exist in cp_dict, then config it
                for l in cp_dict[cp_items[i]]:
                    func = cp_functions.get(cp_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of cp_items maybe not exist. then go to next item
                pass
    
        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, attion!!!")



####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_cp(mode):
    lib.bus_log('\n=====TestCase for CP=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_CP)
    elif mode == 'down':
        lm.call(delete_CP)
    elif mode == 'both':
        lm.call(add_CP)
        lm.call(delete_CP)



def main():
    if len(sys.argv) == 1:
        set_cp('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_cp('up')
        elif "--down" == sys.argv[1]:
            set_cp('down')
        elif "--both" == sys.argv[1]:
            set_cp('both')
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
    set_cp('both')
    set_cp('up')
    send_trigger()
else:
    pass
