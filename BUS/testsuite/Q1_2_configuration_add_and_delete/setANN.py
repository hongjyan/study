#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# 2012 crated by CJF
# Modified: 2013.5.16 By YHJ.
#=====================================================================
import sys
import re
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

##########################################
def bus_fsclish(cmd):
    return lib.bus_fsclish(cmd)

def show_ann_tone():
    cmd = 'show announcement-and-tone announcement load fnbr 3-200 ftyp vad'
    out = bus_fsclish(cmd)
    if re.search('15      loaded',out):
        return False
    else:
        print "show announcement failed"
#        print out
        return True

def delete_ann_tone():
    cmd = 'delete announcement-and-tone announcement fnbr 3-200 ftyp vad'
    out = bus_fsclish(cmd)
    if re.search('152     deleted',out):
        return True
    else:
        print "delete failed"
#        print out
        return False



def load_ann_tone():
    cmd = 'load announcement-and-tone announcement ftyp vad fnbr 3-200 fdir /var/opt/nokiasiemens/annfile/ORIGINAL/'
    out = bus_fsclish(cmd)
    if re.search('COMMAND EXECUTED',out):
        return True
    else:
        print "load announcementn failed"
#        print out
        return False

def add_ann_lang():
    cmd = 'add announcement-and-tone announcement lang 0'
    out = bus_fsclish(cmd)
    if re.search('Failed 0',out):
        return True
    else:
        print "add announcement failed"
#       print out
        return False

def add_ann_oper():
    cmd = 'add announcement-and-tone announcement ftyp vad fnbr 3-200 oper 1'
    out = bus_fsclish(cmd)
    if re.search('Failed 0',out):
        return True
    else:
        print "add announcement oper "
#       print out
        return False

#def setANN():
#    if show_ann_tone():
#        load_ann_tone() and \
#        add_ann_lang() and \
#        add_ann_oper()
#    else:
#        delete_ann_tone()
#        load_ann_tone() and \
#        add_ann_lang() and \
#        add_ann_oper()

def setANN(mode):
    if mode == 'up':
        load_ann_tone() and \
        add_ann_lang() and \
        add_ann_oper()
    elif mode == 'down':
        if not show_ann_tone():
            delete_ann_tone()
            print "All deleted"
        else:
            print "No announcemet-and-tone"
            return True
    else:
        load_ann_tone() and \
        add_ann_lang() and \
        add_ann_oper() and \
        delete_ann_tone() and \
        add_ann_lang() and \
        add_ann_oper()

def run_setANN(mode='both'):
    setANN(mode)

def main():
	print "start run_setANN"
	if len(sys.argv) > 1:
         options_short = 'hud'
         options_long = ['setup', 'teardown']
         try:
             options, arguments = getopt.gnu_getopt(sys.argv, options_short, options_long)
         except getopt.GetoptError:
             usage_help()
             sys.exit(1)
         for opt, arg in options:
             if opt in ['-u', '--setup']:
                 run_setANN('up')
             elif opt in ['-d', '--teardown']:
                 run_setANN('down')
             else:
                 usage_help()
                 sys.exit(1)
	else:
         run_setANN('both')


#run_setANN()

if __name__ == "__main__":
         wait_trigger()
         lib.lib_FS.FSetupVarParser.parsevars('/var/opt/nokiasiemens/commissioning/session/fsetup.conf')
         main()
      #   main()
      #   main()
         send_trigger()


#######################for reference##############################
