#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.4
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

def add_AHUB():
    return AHUB("add")

def delete_AHUB():
    return AHUB("delete")

def AHUB(action):
    clusterName = lib.cluster_name()
    if action == "add":
        AHUB8ConfigFile = "%s_Switch-1-8_fi.conf" %(clusterName)
        AHUB9ConfigFile = "%s_Switch-1-9_fi.conf" %(clusterName)
    elif action == "delete":
        AHUB8ConfigFile = "%s_Switch-1-8_fi_empty.conf" %(clusterName)
        AHUB9ConfigFile = "%s_Switch-1-9_fi_empty.conf" %(clusterName)
    else:
        log("invalid action")
        return False

    bus_fsclish("set hardware fabric-config switch-id Switch-1-8 file /root/BUS/testsuite/Q1_2_configuration_add_and_delete/cnf/%s" %(AHUB8ConfigFile))
    bus_fsclish("set hardware fabric-config switch-id Switch-1-9 file /root/BUS/testsuite/Q1_2_configuration_add_and_delete/cnf/%s" %(AHUB9ConfigFile))    
    time.sleep(20)  #sleep 20s to wait config upload and save
    
    #using grep -v "!" to move system running info of AHUB
    cmd_AHUB8RunningConfig = '''expect -c 'spawn ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no Switch-1-8;expect "#" ; send "swcf\r";expect "#"; send "terminal length 0\r"; expect "#"; send "show running-config\r";expect "#"; send "exit\rn\r";expect "#";'|grep -v "!" '''
    cmd_AHUB9RunningConfig = '''expect -c 'spawn ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no Switch-1-9;expect "#" ; send "swcf\r";expect "#"; send "terminal length 0\r"; expect "#"; send "show running-config\r";expect "#"; send "exit\rn\r";expect "#";'|grep -v "!" '''

    savedAHUB8Config = bus_cli("cat /root/BUS/testsuite/Q1_2_configuration_add_and_delete/cnf/%s" %(AHUB8ConfigFile))
    savedAHUB9Config = bus_cli("cat /root/BUS/testsuite/Q1_2_configuration_add_and_delete/cnf/%s" %(AHUB9ConfigFile))
    CLAAHUB8Config = bus_cli("cat /tftpboot/fabric_staging/Switch-1-8_fi.conf")
    CLAAHUB9Config = bus_cli("cat /tftpboot/fabric_staging/Switch-1-9_fi.conf")
    draft_runningAHUB8Config = bus_cli(cmd_AHUB8RunningConfig).split("show running-config")[1].split("terminal length")[0]
    draft_runningAHUB9Config = bus_cli(cmd_AHUB9RunningConfig).split("show running-config")[1].split("terminal length")[0]
    runningAHUB8Config = ""
    runningAHUB9Config = ""
    for l in draft_runningAHUB8Config.splitlines(True):
    #remove "^M" in all lines and remove empty line of draft running AHUB8 config gotten by expect. 
        if l.endswith(" \r\n"): #if line ending with space+\r\n, remove ending space
            #strip will remove space and \r,\n, so need add '\n' again.
            runningAHUB8Config += (l.lstrip("\r\n").replace("\r", "").rstrip() + '\n')
        else:
            runningAHUB8Config += l.lstrip("\r\n").replace("\r", "")

    for l in draft_runningAHUB9Config.splitlines(True):
    #remove "^M" in all lines and remove empty line of draft running AHUB9 config gotten by expect. 
        if l.endswith(" \r\n"): #if line ending with space+\r\n, remove ending space
            #strip will remove space and \r,\n, so need add '\n' again.
            runningAHUB9Config += (l.lstrip("\r\n").replace("\r", "").rstrip() + '\n')
        else:
            runningAHUB9Config += l.lstrip("\r\n").replace("\r", "")
    
    log("**********************AHUB8 running config after filter*************************")
    log(runningAHUB8Config)
    log("**********************AHUB9 running config after filter*************************")
    log(runningAHUB9Config)

    if cmp(savedAHUB8Config, runningAHUB8Config) != 0:
         log("AHUB8 running config is not the same as saved config")
         return False
    if cmp(savedAHUB9Config, runningAHUB9Config) != 0:
         log("AHUB9 running config is not the same as saved config")
         return False
    if cmp(savedAHUB8Config, CLAAHUB8Config) != 0:
         log("CLA AHUB8 config is not the same as saved config")
         return False
    if cmp(savedAHUB9Config, CLAAHUB9Config) != 0:
         log("CLA AHUB9 config is not the same as saved config")
         return False
    log("AHUB8&9 running config, CLA AHUB8&9 config, saved AHUB8&9 config, are the same")
    return True



def help():
    print "usage: python setAHUB.py [--up|--down|--both]\nif no opotion, then --both will be used"
##################################################################################################
def get_func_name():
    import sys  
    return sys._getframe(1).f_code.co_name
                
            
def set_AHUB(mode):
    lib.bus_log('\n=====TestCase for AHUB=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_AHUB)
    elif mode == 'down':
        lm.call(delete_AHUB)
    elif mode == 'both':
        lm.call(add_AHUB)
        time.sleep(180)
        lm.call(delete_AHUB)
    
        
    
def main():
    if len(sys.argv) == 1:
        set_AHUB('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_AHUB('up')
            print "set_AHUB finished"
        elif "--down" == sys.argv[1]:
            set_AHUB('down')
        elif "--both" == sys.argv[1]:
            set_AHUB('both')
        else:
            help()
            sys.exit(-1) 
       
if __name__ == "__main__": 
    #for i in AHUB_dict:
    #   print "%s:%s\n" %(i, AHUB_dict[i])
    main() 
elif not g_debug:  # For BOA
    wait_trigger() 
    set_AHUB('down')
    set_AHUB('up')
    send_trigger()
else:
    pass
