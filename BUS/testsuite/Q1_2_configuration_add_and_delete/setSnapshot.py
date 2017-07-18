#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.11.7
#=====================================================================
import sys
import re
import getopt
import time

g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *


sys.path.append("../lib")  # not used in BOA

lib=boaimport('BU_BUS_Lib')

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
    log("usage: python setSnapshot.py [--up|--both]\nif no opotion, then --both will be used")

def generate_name():
    now=time.strftime('%Y_%m_%d__%H_%M_%S',time.localtime(time.time()))
    return now+'_by_BUS'



g_snapshot_name=generate_name().replace('_','')


def get_snapshot_name():
    cmd = 'fsclish -c "show snapshot listall" | grep ' +  g_snapshot_name
    out = bus_cli(cmd)
    name = out.rstrip()
    return name

#Setup Cases
def save_snapshot():
    cmd = 'save snapshot config-name '+g_snapshot_name
    bus_fsclish(cmd)
    name = get_snapshot_name()
    if name == '':
	return False
    elif re.search(g_snapshot_name, name):
	return True
    return False

#TearDown Cases
def delete_snapshot():
    config_name = get_snapshot_name()
    cmd = 'delete snapshot config-name ' + config_name
    bus_fsclish(cmd)
    name = get_snapshot_name()
    if name == '':
	return True
    return False


####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name

def set_snapshot(mode):
    lib.bus_log('\n=====TestCase for Snapshot=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(save_snapshot)
    elif mode == 'down':
        lm.call(delete_snapshot)
    elif mode == 'both':
        lm.call(save_snapshot)
        lm.call(delete_snapshot)



def main():
    if len(sys.argv) == 1:
        set_snapshot('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_snapshot('up')
        elif "--both" == sys.argv[1]:
            set_snapshot('both')
        else:
            help()
            sys.exit(-1)

if __name__ == "__main__":
    main()
elif not g_debug:
    set_snapshot('both')
    set_snapshot('up')
else:
    pass
