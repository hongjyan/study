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

def add_sgw():
    return lib.add_sgw()    
    
def delete_sgw():
    return lib.clear_sgw()	
####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name


def set_sgw(mode='both'):
    lib.bus_log('\n=====TestCase for SGW=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_sgw)
    elif mode == 'down':
        lm.call(delete_sgw)
    else:
        lm.call(add_sgw)
        lm.call(delete_sgw)

def main():
    if len(sys.argv) == 1:
        set_sgw('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_sgw('up')
        elif "--down" == sys.argv[1]:
            set_sgw('down')
        elif "--both" == sys.argv[1]:
            set_sgw('both')
        else:
            help()
            sys.exit(-1)


if __name__ == "__main__":
    filename = lib.get_paraFileName_for_different_cluster()
    main()
elif not g_debug:
    wait_trigger()
    set_sgw('both')
    set_sgw('up')
    send_trigger()
else:
    pass
