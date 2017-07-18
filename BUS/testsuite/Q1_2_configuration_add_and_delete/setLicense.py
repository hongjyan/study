#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.13
# 2013.1.15 modify disable_test_responder() and enable_test_responder()
# since license change in UI5 and UI5EP1 new build.
# Modified by Rene 2013.3.14.Added a new version parameter "EP2".
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
lib_MO=boaimport('BU_MOState_lib')

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
    print "usage: python setLicense.py [--up|--down|--both]\nif no opotion, then --both will be used"


def get_license_parameter(filename):
    lib.lib_FS.FSetupVarParser.parsevars(filename)
    dict = lib.lib_FS.FSetupVarParser.datacontent
    license_dict = {}
    temp_list = []
    license_items = ['License', 'Features']
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if lib.find_equal(j, license_items): # find the fittest license items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if license_dict.has_key(j):
                license_dict[j].append(temp_list) # if this item class is already one of oam_dict's keys, append list to this key's value(it is a list) 
            else:
                license_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 

    return license_dict

license_dict = get_license_parameter(filename)


def restart_CLM():
    out = bus_cli('fshascli -rn /CLM')
    if not re.search('CLM restart successful', out):
        return False
    time.sleep(3)
    CLMState = lib_MO.class_MOState("/CLM")
    if CLMState.MO_is_OK()[0]:
        time.sleep(120)
        return True
    return False

def enable_test_responder():
    cmd = 'fsclish -p -c "set cli built-in confirmation-prompt off" -c "set cli unsupported-vendor-mode on eiltonodslemac" -c  "set license test-license state enabled" -c  "show license test-license state"'
    out = bus_cli(cmd)
    if re.search('successfully', out):
        return True
    else:
        return False



def install_license(license_dict):
    bus_cli('cp ./cnf/*.XML /srv/Log/crash/')
    
    if 'UI5' == version:
        path = '/crash/'
    else:
        path = '/crash/'
    log("2")    
    time.sleep(60)   #wait for CLM Server running.
    for l in license_dict['License']:
        out = bus_fsclish('show license details unique-id '+ l[0])
        log("3")
        if re.search('not yet serving management requests', out):
            log("wait 60s for license server accept management request")
            time.sleep(60)  #wait for license server accept management request.

        out = bus_fsclish('show license details unique-id '+ l[0])
        if re.search('License name is invalid',out) or re.search('not installed', out):
            out = bus_fsclish('add license file ' + path + l[0] + '.XML')
            if not re.search('Installation of license was successful', out):
                return False

    return True

def set_license_feature_on(fl):

    for f in fl:
        bus_fsclish("set license feature-mgmt id %s feature-admin-state on" %(f))

    for f in fl:
        out = bus_fsclish('show license feature-mgmt id '+ f)
        if re.search('\w+\s+\d+\s+off', out) or re.search('\w+\s+\d+\s+config', out):
            return False
    return True



#TearDown Cases
#def disable_test_responder():
    #cmd = 'fsclish -p  -c "set cli built-in confirmation-prompt off" -c "set cli unsupported-vendor-mode on eiltonodslemac" -c  "set license test-license state disabled" -c  "show license test-license state"'
    #out = bus_cli(cmd)
    #if re.search('successfully', out):
    #    return True
    #else:
    #    return False

def uninstall_license(license_dict):
    bus_cli('fsclish -p1 -c "set cli built-in confirmation-prompt off"')


    for l in license_dict['License']:
        out = bus_fsclish('show license details unique-id '+ l[0])
        if re.search('License name is invalid',out) or re.search('not installed', out):
            pass
        elif re.search('Unique ID:\s+'+l[0],out):
            bus_cli('fsclish -p1 -c "delete license unique-id ' + l[0] + '"')
            out = bus_fsclish('show license details unique-id '+ l[0])
            if re.search('License name is invalid',out) or re.search('not installed', out):
                pass
            else:
                bus_cli('fsclish -p1 -c "set cli built-in confirmation-prompt on"')
                return False

    bus_cli('fsclish -p1 -c "set cli built-in confirmation-prompt on"')
    return True
        

def set_license_feature_off(fl):
    for f in fl:
        bus_fsclish("set license feature-mgmt id %s feature-admin-state off" %(f))

    for f in fl:
        out = bus_fsclish('show license feature-mgmt id '+ f)
        if re.search('\w+\s+\d+\s+on', out) or re.search('\w+\s+\d+\s+config', out):
            return False
    return True




def add_license():
    global license_dict
    global version
    if re.search('EP3', bus_cli("fsswcli -c")):
        version = 'UI5EP3'
    elif re.search('UI50_5.4', bus_cli("fsswcli -c")):
        version = 'UI5EP3'
    elif re.search('UI50_5.3', bus_cli("fsswcli -c")):
        version = 'UI5EP2'
    elif re.search('UI50_5.2', bus_cli("fsswcli -c")):
        version = 'UI5EP1'
    else:
        version = 'UI5'
    if not 'UI5' == version:
        if not enable_test_responder():
            return False
    if not install_license(license_dict):
        return False
    if not set_license_feature_on(license_dict['Features'][0]): # assume there is just one Features parameter
        return False
    
    return True

def delete_license():
    global license_dict

    if not uninstall_license(license_dict):
        return False
    if not set_license_feature_off(license_dict['Features'][0]): # assume there is just one Features parameter
        return False
    
    return True


####################################################################
def get_func_name():
    import sys
    return sys._getframe(1).f_code.co_name

def set_license(mode):
    lib.bus_log('\n=====TestCase for License=====')
    lm = lib.LogManager(get_func_name())
    if mode == 'up':
        lm.call(add_license)
    elif mode == 'down':
        lm.call(delete_license)
    elif mode == 'both':
        lm.call(add_license)
        lm.call(delete_license)



def main():
    if len(sys.argv) == 1:
        set_license('both')
    elif len(sys.argv) > 1:
        if "--up" == sys.argv[1]:
            set_license('up')
        elif "--down" == sys.argv[1]:
            set_license('down')
        elif "--both" == sys.argv[1]:
            set_license('both')
        else:
            help()
            sys.exit(-1)

if __name__ == "__main__":
    #global filename
    #lib.lib_FS.FSetupVarParser.parsevars(filename)
    #license_dict = get_license_parameter(lib.lib_FS.FSetupVarParser.datacontent)
    #print license_dict
    main()
elif not g_debug:
    wait_trigger()
    set_license('both')
    set_license('up')
    send_trigger()
else:
    pass
