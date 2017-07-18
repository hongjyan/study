#======================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.11.26
#  2013.1.14 replace os.popen(cmd).read() by bus_cli(cmd) since the previous one can't use in BOA. By YHJ.
#  2013.1.4  replace print() by log() since print won't work in BOA. By YHJ
#  2013.1.15 add grep -v "address instance" when deleting SS7SGU address for UI5.
#  2013.1.17 add delaytimer=2 for some commands since those commands need more time for buffer ready. By YHJ
#  2013.1.17 change command format to """ """.
#=====================================================================
#!/usr/bin/python
import os
import re
import time
import sys


g_debug=True
#g_debug=False
if g_debug:
    from boa_simulate import *



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

def bus_cli(str, delaytimer=0.3):  # add delaytimer here since bus_cli used in following code need parameter:delaytimer.
    if g_debug:
        out = os.popen(str + " 2>&1").read()
#        sys.stdout.write('\n'+str+ '\n' + out)
        return out 
    else:
        return cli(str, delaytimer=0.3)
       
def bus_fsclish(str):
    if g_debug:
        cmd = 'fsclish -c "' + str + '"' 
        out = os.popen(cmd + " 2>&1").read()
#        sys.stdout.write('\n'+cmd + '\n' + out)
        return out 
    else:
        return fsclish(str)


def kuk(obj_name, kw, name_or_id):
    if obj_name == 'global-title-translation result':
        #cmd="fsclish -c \"show signaling " + obj_name + " all\" | grep  ': ' | awk -F ':' '{print $2}'"
        cmd = """ fsclish -c "show signaling %s all"|grep ': '|awk -F ':' '{print $2}'""" %obj_name
    else:
        #cmd="fsclish -c \"show signaling " + obj_name + " all\" | grep \"" + kw + "\" | awk -F ':' '{print $2}'"
        cmd = """ fsclish -c "show signaling %s all"|grep "%s"|awk -F ':' '{print $2}'""" %(obj_name, kw)
    output = bus_cli(cmd)
    log("\n\n\n******************************************************************************************")
    log(cmd,)
    log(output)
    
    zs=output.rstrip('\n').split('\n')
    for z in zs:
        v = z.strip().strip('\r')
        if v!='':                
            #cmd="fsclish -c \"delete signaling " + obj_name + " " + name_or_id + " " + v + "\""
            cmd = """ fsclish -c "delete signaling %s %s %s" """ %(obj_name, name_or_id, v)
            output = bus_cli(cmd, delaytimer=2)  # delete ss7 destination-point-code need more time to buffer ready
            log("\n\n\n******************************************************************************************")
            log(cmd,)
            log(output)
        else:
            log(obj_name + " does not exist")


    #check if obj_name delete succeed 
    if obj_name == 'global-title-translation result':
        #cmd="fsclish -c \"show signaling " + obj_name + " all\" | grep  ': ' | awk -F ':' '{print $2}'"
        cmd = """fsclish -c "show signaling %s all"| grep ': ' | awk -F ':' '{print $2}' """ %(obj_name)
    else:
        #cmd="fsclish -c \"show signaling " + obj_name + " all\" | grep \"" + kw + "\" | awk -F ':' '{print $2}'"
        cmd = """fsclish -c "show signaling %s all"|grep "%s"|awk -F ':' '{print $2}'""" %(obj_name, kw)
    output = bus_cli(cmd)
    zs=output.rstrip('\n').strip('\r').split('\n')
    if zs.count('') == len(zs):
        return True
    else:
        log("%s have not be deleted" %(obj_name))
        for z in zs:
            v = z.strip().strip('\r')
            if '' != v:
                log("!!!!!!!!fsclish -c \"delete signaling " + obj_name + " " + name_or_id + " " + v + " failed!!!!!!!!!!!\"")
        log("error happen when delete %s!!!!!!!!!!!!!" %(obj_name))
        return False



def kuk_nnsf (obj_name, name_or_id):
    if obj_name == 'nnsf mgw':
        cmd="fsclish -c \"delete nnsf mgw rspc all\" "
        output = bus_cli(cmd)
        log("\n\n\n******************************************************************************************")
        log(cmd,)
        log(output)
        if re.search("not defined", bus_cli("fsclish -c \"show nnsf mgw\"")):
            return True
        else:
            return False
        
    elif obj_name == 'nnsf ran':
        #cmd="fsclish -c \"show  " + obj_name + " \" | grep -v '/' | grep -v '@' | grep -v '==' | grep -v 'COMMAND EXECUTED' | grep ' '| awk '{print $2}'"
        cmd = """fsclish -c "show %s "|grep -v '/'|grep -v '@'|grep -v '=='|grep -v 'COMMAND EXECUTED' |grep ' '| awk '{print $2}'""" %(obj_name)
    elif obj_name == 'nnsf mss':
        #cmd="fsclish -c \"show " + obj_name + " \" | grep \"MSS name\" | awk -F ':' '{print $2}'"
        cmd="""fsclish -c "show %s"|grep "MSS name"|awk -F ':' '{print $2}'""" %(obj_name)
    
    output = bus_cli(cmd)
    log("\n\n\n******************************************************************************************")
    log(cmd,)
    log(output)
    
    zs=output.rstrip('\n').strip('\r').split('\n')
    for z in zs:
        v = z.strip().strip('\r')
        if v!='':                
            #cmd="fsclish -c \"delete " + obj_name + " " + name_or_id + " " + v + "\""
            cmd = """fsclish -c "delete %s %s %s" """ %(obj_name, name_or_id, v)
            output = bus_cli(cmd)
            log("\n\n\n******************************************************************************************")
            log(cmd,)
            log(output)
        else:
             log(obj_name + " does not exist")
    
    #check if obj_name delete succeed 
    if obj_name == 'nnsf ran':
        #cmd = "fsclish -c \"show  " + obj_name + " \" | grep -v '/' | grep -v '@' | grep -v '==' | grep -v 'COMMAND EXECUTED' | grep ' '| awk '{print $2}'"
        cmd = """fsclish -c "show %s"|grep -v '/' | grep -v '@' | grep -v '==' | grep -v 'COMMAND EXECUTED'| grep ' '| awk '{print $2}'""" %(obj_name)
    elif obj_name == 'nnsf mss':
        #cmd = "fsclish -c \"show " + obj_name + " \" | grep \"MSS name\" | awk -F ':' '{print $2}'"
        cmd = """fsclish -c "show %s"| grep "MSS name" | awk -F ':' '{print $2}'""" %(obj_name)
    output = bus_cli(cmd)
    zs=output.rstrip('\n').strip('\r').split('\n')
    if zs.count('') == len(zs):
        return True
    else:
        for z in zs:
            v = z.strip().strip('\r')
            if '' != v:
                log("!!!!!!!!fsclish -c \"delete signaling " + obj_name + " " + name_or_id + " " + v + " failed!!!!!!!!!!!\"")
        log("error happen when delete %s!!!!!!!!!!!!!" %(obj_name))
        return False


def D_admin_state(obj_name, kw, name_or_id):
    #cmd="fsclish -c \"show signaling " + obj_name + " all\" | grep \"" + kw + "\" | awk -F ':' '{print $2}'"
    cmd = """fsclish -c "show signaling %s all"|grep "%s"|awk -F ':' '{print $2}'""" %(obj_name, kw)
    output = bus_cli(cmd) 
    
    zs=output.rstrip('\n').strip('\r').split('\n')
    for z in zs:
        v = z.strip().strip('\r')
        if v!='':
            if name_or_id == "id" and "ss7 link" == obj_name and re.search("UI5EP",version):
                # if version is UI5EP, then "config' added in command
                #cmd="fsclish -c \"set signaling " + obj_name + " config admin-state disabled " + name_or_id + " " + v + "\""
                cmd = """fsclish -c "set signaling %s config admin-state disabled %s %s" """ %(obj_name, name_or_id, v)
            else: 
                #cmd="fsclish -c \"set signaling " + obj_name + " admin-state disabled " + name_or_id + " " + v + "\"" 
                cmd = """ fsclish -c "set signaling %s admin-state disabled %s %s" """ %(obj_name, name_or_id, v)
            output = bus_cli(cmd, delaytimer=2)  #set link state need more time for buffer ready
            log("\n\n\n******************************************************************************************")
            log(cmd,)
            log(output)
        else:
            log(obj_name + " does not exist ")
    
    return True
    

def clear_all():
    global version
    if re.search("EP3", bus_cli("fsswcli -c")):
        version = "UI5EP3"
    elif re.search("UI50_5.4", bus_cli("fsswcli -c")):
        version = "UI5EP3"
    elif re.search("UI50_5.3", bus_cli("fsswcli -c")):
        version = "UI5EP2"
    elif re.search("UI50_5.2", bus_cli("fsswcli -c")):
        version = "UI5EP1"
    elif re.search("UI50_5.1", bus_cli("fsswcli -c")):
        version = "UI5"

        #*********************************
        #NSNF MGW
    if not kuk_nnsf("nnsf mgw"," "):
        return False

        #NSNF RAN
    if not kuk_nnsf ("nnsf ran", "name"):
        return False

        #NSNF MSS
    if not kuk_nnsf ("nnsf mss", "name"):
        return False


        #*********************************
        #GT restult part
    if not kuk ("global-title-translation result", " " , "name"):
        return False

        #GT rule part
    if not kuk("global-title-translation rule", "name                                              :", "name"):
        return False

        #GT action part
    if not kuk("global-title-translation action", "action-name", "name"):
        return False

        #*********************************
        #SCCP 

    if not (kuk("sccp concerned-point-code", "Concerned Point Code Name", "name") and \

        kuk("sccp concerned-subsystem", "Concerned Subsystem Name", "name") and \

        kuk("sccp subsystem", "SCCP SubSystem    ", "name") and \

        kuk("sccp destination-point-code", "Point code Name", "name")):
        return False

    #delte SCCP OPC twice because primary SCCP OPC should be deleted fristly!
    if not (kuk("sccp own-point-code", "Point code Name", "name") or \
    
    kuk("sccp own-point-code", "Point code Name", "name")):
        return False
        #*********************************
        #NIF
    if "UI5" == version: #UI5, command
        if not (kuk("ss7 nif-ran-point-code", "ran point code name", "name") and \
                kuk("ss7 nif-mgw-to-slc", "ran point code name ", "name")):
            return False

    else:
        if not (kuk("ss7 nif-destination-point-code", "destination point code name", "name") and \
                    kuk("ss7 nif-mgw-to-slc", "destination point code name", "name")):
            return False
    
    #delete nif-point-code twice because of mgw-own should be deleted firstly
    if not (kuk("ss7 nif-point-code", "nif-point-code-name", "name") or \

        kuk("ss7 nif-point-code", "nif-point-code-name", "name") ):
        return False
        #*********************************
        #TDM

        #link part
    if not kuk("ss7 route","route-name", "name"):
        return False

    D_admin_state("ss7 linkset","linkset-name", "name")
    if not kuk("ss7 linkset","linkset-name", "name"):
        return False

        #SET Admin_State Disable
    D_admin_state("ss7 link","link-id", "id")
    if not kuk("ss7 link","link-id", "id"):
        return False

        #ss7 DPC part
    if not kuk("ss7 destination-point-code","destination-point-code-name", "name"):
        return False

        #ss7 OPC part
    if not kuk("ss7 own-point-code","self-point-code-name", "name"):
        return False
        #*********************************
        #IP

    D_admin_state("ss7 association", "association id", "id")
    if not ( kuk("ss7 association", "association id", "id") and \

    kuk("ss7 remote-as", 'name                                              :', "name") and \

    kuk("ss7 local-as", 'name                                              :', "name") ):
        return False
        #*********************************
        #SAP part
    if not kuk("service-access-point", "service-access-point-name", "name"):
        return False

    #*********************************
    #remove related TDM resource. Need to to added in future when 4 PCM are used.
    #if not lib_delete_tdm.remove_set()


    #*********************************
    #remove SS7SGU address
    log("\n\n\n******************************************************************************************")
    cmd = "fsclish -c \"show networking address user /SS7SGU\" |grep -v \"Executing\" |grep -v \"IP addresses\"|grep -v \"address instance\""
    output = bus_cli(cmd)
    log(cmd)
    log(output)
    SS7_para_list = []
    temp_list = []
    if re.search("SS7SGU", output):
        l = output.strip('\n\n').split('\n\n')
        for i in l:
            ll = i.splitlines()
            temp_list.append(ll[0])
            for lll in ll[1:]:
                if re.search("address", lll):
                    temp_list.append(lll.split(':')[1].split('/')[0])
                if re.search("owner", lll):
                    temp_list.append(lll.split(':')[1])
            SS7_para_list.append(temp_list)
            temp_list = []
    else:
        pass

    for paras in SS7_para_list:
        cmd = "fsclish -c \"delete networking address dedicated %s iface %s ip-address %s\"" %(paras[2], paras[0], paras[1])
        output = bus_cli(cmd)
        log(cmd)
        log(output)
    
    cmd = "fsclish -c \"show networking address user /SS7SGU\""
    output = bus_cli(cmd)
    log(cmd)
    log(output)
    if re.search('SS7SGU', output):
        return False
    #*************************************

    return True


if __name__ == "__main__":
    print clear_all()
