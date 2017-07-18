#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.8 get PCM ID from config file by yhj
# 2013.1.15 iwf circuitgroup can be added.
# 2013.2.22 change set to SET in add_clock by YHJ
#=====================================================================

#!/usr/bin/python
import sys
import re
import getopt
import time
import collections

g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *


sys.path.append("../lib")  # not used in BOA

lib = boaimport('BU_BUS_Lib')
lib_FS=boaimport('BU_FSetupVarParser_Lib')
lib_delete_tdm=boaimport('delete_tdm_lib')
APCM_deque = collections.deque()

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

def add_SET(set_index, chassis_index, piu_index, amc_index, port_index):
    cmd = 'add tdm sdh set chassis-index '+chassis_index+' piu-index '+piu_index+' amc-index '+amc_index+'  port-index '+port_index + ' set-index ' + set_index
    bus_fsclish(cmd)
    time.sleep(10)
 
    cmd1 = 'show tdm sdh set-state set-index ' + set_index
    cmd2 = 'show tdm sdh set-state set-index ' + str(int(set_index)+128)
    out1 = bus_fsclish(cmd1)
    out2 = bus_fsclish(cmd2)
    if re.search('unlocked', out1) and re.search('enabled', out1) and re.search('unlocked', out2) and re.search('enabled', out2):
        return True
    else:
        return False
        
        
def add_pgr(wo_index, sp_index):
    cmd = 'add tdm sdh pgr wo-index ' + wo_index + ' sp-index ' + sp_index
    bus_fsclish(cmd)

    out = bus_fsclish('show tdm sdh pgr')
    if re.search('\s+SET unit_index\s+:\s+'+wo_index, out) and re.search('\s+SET unit_index\s+:\s+'+sp_index, out):
        return True
    else:
        return False


def add_letgr(index):
    bus_fsclish("add tdm pdh letgr set-index %s" %(index))
   
    out = bus_fsclish("show tdm pdh letgr letgr-index %s" %(index))
    if re.search('LETGR index', out):
        return True 
    else:
        return False


def add_circuitgroup(cgr_name):
    bus_fsclish("add tdm circuitgroup ncgr %s spe vmgw" %(cgr_name))
    
    if re.search("PSTN|pstn", cgr_name):
        bus_fsclish("set tdm circuitgroup ncgr %s inter pstn" %(cgr_name))
    elif re.search("ATER|ater", cgr_name):
        bus_fsclish("set tdm circuitgroup ncgr %s inter a" %(cgr_name))
    elif re.search("iwf|IWF", cgr_name):
        bus_fsclish("set tdm circuitgroup ncgr %s inter other" %(cgr_name))
    else:
        log("circuitgroup name must inlcude \"ater|ATER|pstn|PSTN|iwf|IWF\"")
        return False

    out = bus_fsclish("show tdm circuitgroup ncgr %s" %(cgr_name))
    if re.search('circuit group name', out):
        ##added by Rene,2013.7.5,delete vmg circuitgroup start##
	out1 = "fsclish -c \"show tdm circuitgroup ncgr %s\"|awk '/group number/ {print $5}'" %(cgr_name)
	cgr_num = bus_cli(out1).strip().split()[0]
        out2 = "fsclish -c \"show vmgw mgw vid 0 \"|awk '/virtual media gateway name/ {print $6}'"
        vmgw_name = bus_cli(out2).strip().split()[0]
	out3 = "add vmgw circuitgroup cgr %s vmn %s" %(cgr_num,vmgw_name)
	bus_fsclish(out3)
        ##added by Rene,2013.7.5,delete vmg circuitgroup end##
        return True
    else:
        return False


def add_apcm(et_index, tcpcm_index, apcm_index, cpool):
    out = bus_fsclish("add tdm ater et-index %s tcpcm-index %s apcm-index %s cpool-value %s" %(et_index, tcpcm_index, apcm_index, cpool))
    if re.search("in ascending order", out):
        return 1

    out = bus_fsclish("show tdm ater apcm-index %s" %(apcm_index))
    if re.search("APCM", out):
        return 0
    else:
        return -1


def add_APCM(et_index, tcpcm_index, apcm_index, cpool):
    global APCM_deque
    cnt = 0
    APCM_deque.appendleft([et_index, tcpcm_index, apcm_index, cpool])
    rc = add_apcm(*APCM_deque[0])
    if 0 == rc:  # if APCM added in order and succeed
        APCM_deque.popleft()
        if 0 == len(APCM_deque):
            return True
        else:
            temp_list = APCM_deque.popleft()
            add_APCM(*temp_list)
    elif 1 == rc:  # if APCM added isn't in order
        APCM_deque.append(APCM_deque[0])
        APCM_deque.popleft()
        cnt += 1 
        if len(APCM_deque) == cnt:
            return True
        else:
            add_APCM(*APCM_deque[0])
    else:   # if add APCM error, return False
        return False
    
    return True # if add_APCM(*temp_list) is the last call and return True, then the execution will be here and exit. should set True since                  # all APCM are added compelete
    

        
        

def add_crct(cgr_name, pcm_index, tsl):
    bus_fsclish("add tdm crct ncgr %s crct-pcm %s crct-tsl %s" %(cgr_name, pcm_index, tsl))
    bus_fsclish("set tdm crct-state admin-state unlocked crct-pcm %s crct-tsl %s" %(pcm_index, tsl))
    
    out = bus_fsclish("show tdm crct-state crct-pcm %s crct-tsl %s" %(pcm_index, tsl))
    size = 1
    array = str(tsl).split('-')
    if(len(array) == 2):
        size = (int(array[1]) - int(array[0]) + 1)
    out = bus_fsclish("show tdm crct-state crct-pcm %s crct-tsl %s" %(pcm_index, tsl))
    count = len(re.findall('\s+unlocked\s+', out))
    enabled_count = len(re.findall('\s+enabled\s+', out))
    if size == count and size == enabled_count:
        return True
    else:
        return False

    
def add_clock(set_index):
    bus_fsclish("add mgw synchronization inputreference port SET-%s" %(set_index))
    bus_fsclish("set mgw synchronization inputreference port SET-%s priority 12 forceql 2"  %(set_index))
    
    out = bus_fsclish("show mgw synchronization inputreference")
    pattern = "SET-%s" %(set_index) #can be stricter
    if re.search(pattern, out): 
        return True
    else:
        return False



# find word equal to s from l.
def find_equal(s, l):
    for i in l:
        if s == i:
            return True
    return False

         


#BU_FSetupVarParser_Lib will generate dictionary uncluding all parameters from cnf file. 
#like: {'CRCT_APCM11000':'catherine_ATER, 11000, 1-31', 'PGR_1':'0', .............}
#get_tdm_parameter will generate tdm related dictionary from upper bigger dictionary, 
#like: { 'SET':[[0],[1]], 'CRCT':[[catherine_PSTN, 1,1-31],[catherine_ATER, 11000, 1-31]], .......... }
def get_tdm_parameter(dict):
    tdm_dict = {}
    temp_list = []
    tdm_items = ('SET', 'PGR', 'LETGR', 'CGR', 'APCM', 'CRCT', 'CLOCK')
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if find_equal(j, tdm_items): # find the fittest tdm items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if tdm_dict.has_key(j):
                tdm_dict[j].append(temp_list) # if this item class is already one of tdm_dict's keys, append list to this key's value(it is a list) 
            else:
                tdm_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 

    return tdm_dict

def main_add_tdm(filename):
    lib_FS.FSetupVarParser.parsevars(filename)
    tdm_dict = get_tdm_parameter(lib_FS.FSetupVarParser.datacontent)
#    for i in tdm_dict.keys():
#        print "%s: %s" %(i, tdm_dict[i])
    tdm_items = ('SET', 'PGR', 'LETGR', 'CGR', 'APCM', 'CRCT', 'CLOCK')
    tdm_functions = {'SET':add_SET, 'PGR':add_pgr, 'LETGR':add_letgr, 'CGR':add_circuitgroup, 'APCM':add_APCM, 'CRCT':add_crct, 'CLOCK':add_clock}
    try:
        for i in range(len(tdm_items)): # config TDM in sequence as defined in tdm_items
            if tdm_dict.has_key(tdm_items[i]): # if item of tdm_items exist in tdm_dict, then config it
                for l in tdm_dict[tdm_items[i]]:
                    func = tdm_functions.get(tdm_items[i])
                    if not func(*l):      #if config failed, return False, and exit. 
                        return False 
                    else:                 #else, sleep 1s.
                        time.sleep(1)
            else:                       #some item of tdm_items maybe not exist. then go to next item
                pass

        return True
    except KeyboardInterrupt:
        log("KeyboardInterrupt, delete all TDM resource")
        #lib_delete_tdm.remove_all_TDM_resource()


if '__main__' == __name__:
    filename = lib.get_paraFileName_for_different_cluster()
    print main_add_tdm(filename)
