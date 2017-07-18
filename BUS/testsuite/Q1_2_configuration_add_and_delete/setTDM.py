#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.11.7
#=====================================================================
import sys
import re

g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *


sys.path.append("../lib")  # not used in BOA

lib=boaimport('BU_BUS_Lib')
lib_delete_tdm = boaimport('delete_tdm_lib')
lib_add_tdm = boaimport('add_tdm_lib')

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

def help():
    print "usage: python setTDM.py [--up|--down|--both]\nif no opotion, then --both will be used"


def bus_cli(cmd):
    return lib.bus_cli(cmd)

def bus_fsclish(cmd):
    return lib.bus_fsclish(cmd)

def delete_tdm():
    return lib_delete_tdm.remove_all_TDM_resource()

def add_tdm(filename):
    return lib_add_tdm.main_add_tdm(filename)


####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_tdm(mode):
    global filename
    lib.bus_log('\n=====TestCase for TDM=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_tdm, filename)
    elif mode == 'down':
        lm.call(delete_tdm)
    elif mode == 'both':
        lm.call(add_tdm, filename)
        lm.call(delete_tdm)



def main():  
    if len(sys.argv) == 1:
        set_tdm('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_tdm('up')
        elif "--down" == sys.argv[1]:
            set_tdm('down')
        elif "--both" == sys.argv[1]:
            set_tdm('both') 
        else:
            help()
            sys.exit(-1)

if __name__ == "__main__":
    main()
elif not g_debug:
    wait_trigger()
    set_tdm('both')
    set_tdm('up') 
    send_trigger()
else:
    pass
