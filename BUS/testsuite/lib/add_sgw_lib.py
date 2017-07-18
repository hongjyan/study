#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.24
# 2013.1.17 modify 
# out = bus_cli("""fsclish -c "show signaling global-title-translation action  name %s" """ %(name)) for 
# out = bus_cli(""""show nnsf mss name %s" """ %(name))
# ssh.sendline(cmd) problem on BOA 
# 2013.3.14 Rene modify
# Added a new version parameter "EP2".
#=====================================================================
import os
import re
import sys
import time


g_debug=True
#g_debug=False
if g_debug:
    from boa_simulate import *

lib_delete_sgw=boaimport('delete_sgw_lib')
lib_FS = boaimport('BU_FSetupVarParser_Lib')

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


def bus_cli(str):
    if g_debug:
        out = os.popen(str + " 2>&1").read()
        sys.stdout.write('\n'+str+ '\n' + out)
        return out
    else:
        return cli(str)
    
def bus_fsclish(str):
    if g_debug:
        cmd = 'fsclish -c "' + str + '"'
        out = os.popen(cmd + " 2>&1").read()
        sys.stdout.write('\n'+cmd + '\n' + out)
        return out
    else:
        return fsclish(str)

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


def add_SGW_TDM(set_index, chassis_index, piu_index, amc_index, port_index, pgr_wo_index, pgr_sp_index):
    if add_SET(set_index, chassis_index, piu_index, amc_index, port_index) and \
    add_pgr(pgr_wo_index, pgr_sp_index) and \
    add_letgr(set_index):
        return True
    else:
        return False
 


                                                                                                                                        
def add_SS7SGU(node, iface, ip_mask):
    bus_fsclish("add networking address dedicated %s iface %s ip-address %s user /SS7SGU" %(node, iface, ip_mask))

    out = bus_fsclish("show networking address owner %s iface %s user /SS7SGU"  %(node, iface)) 
    if re.search(ip_mask, out):
        return True
    else:
        return False

def add_SAP(name, id, net_app, net_ind, net_type):
    bus_fsclish("add signaling service-access-point name NA0 id 1 network-appearance 1 network-indicator NA0 network-type itu")
    
    out = bus_fsclish("show signaling service-access-point name NA0")
    if re.search("service-access-point-name               : NA0", out):
        return True
    else:
        return False

def add_LOCAL_AS(id, name, routing_context, routing_dpc, sap, traffic_mode):
    bus_fsclish("add signaling ss7 local-as id %s name %s routing-context %s routing-key-dpc %s service-access-point-name %s traffic-mode %s" %(id, name, routing_context, routing_dpc, sap, traffic_mode))

    out = bus_fsclish("show signaling ss7 local-as name %s" %(name))
    if re.search("routing-key-dpc", out):
        return True
    else:
        return False

def add_REMOTE_AS(id, name, pending_timer, sap, routing_context, routing_dpc, traffic_mode, communication_type):
    bus_fsclish("add signaling ss7 remote-as id %s name %s pending-timer %s service-access-point-name %s routing-context %s routing-key-dpc %s traffic-mode %s communication-type %s" %(id, name, pending_timer, sap, routing_context, routing_dpc, traffic_mode, communication_type))
    
    out = bus_fsclish("show signaling ss7 remote-as name %s" %(name))
    if re.search("routing-key-dpc", out):
        return True
    else:
        return False



def add_ASSOCIATION(id, local_ip, local_port, role, node, remote_as_name, remote_ip, remote_port, exchange_mode, sctp_profile, com_type, local_as_name="None"):
    local_ip_list = local_ip.split('|')
    remote_ip_list = remote_ip.split('|')
    local_ip_parameter = ' '.join(("local-ip-addr " + ip for ip in local_ip_list))
    remote_ip_parameter = ' '.join(("remote-ip-addr " + ip for ip in remote_ip_list))
    if "ipsp" == com_type:
        bus_fsclish("add signaling ss7 association id %s  %s local-server-port %s local-as-name %s role %s node %s remote-as-name %s %s remote-port %s exchange-mode %s sctp-profile %s communication-type %s" %(id, local_ip_parameter, local_port, local_as_name, role, node, remote_as_name, remote_ip_parameter, remote_port, exchange_mode, sctp_profile, com_type))
    else:
        bus_fsclish("add signaling ss7 association id %s  %s local-server-port %s role %s node %s remote-as-name %s  %s remote-port %s exchange-mode %s sctp-profile %s communication-type %s" %(id, local_ip_parameter, local_port, role, node, remote_as_name, remote_ip_parameter, remote_port, exchange_mode, sctp_profile, com_type))
    
    bus_fsclish("set signaling ss7 association id %s admin-state enabled" %(id))
    out = bus_fsclish("show signaling ss7 association id %s" %(id))
    if re.search("enabled", out):
        return True
    else:
        return False


def add_SS7_OPC(name, sap, code, type):
    bus_fsclish("add signaling ss7 own-point-code name %s service-access-point-name %s point-code %s type %s" %(name, sap, code, type))
    out = bus_fsclish("show signaling ss7 own-point-code name %s" %(name))

    if re.search("self-point-code", out):
        return True
    else:
        return False



def add_SS7_DPC(name, type, code, sap, sls, mode):
    bus_fsclish("add signaling ss7 destination-point-code name %s type %s point-code %s service-access-point-name %s sls-number %s special-ansi-sltm-mode %s" %(name, type, code, sap, sls, mode))
    out = bus_fsclish("show signaling ss7 destination-point-code name %s" %(name))
    
    if re.search("destination-point-code-name", out):
        return True
    else:
        return False



def add_LINK(bandwidth, dpc_name,  opc_name, type, id, node, slc, link_profile, timer_profile, num_of_slots, pcm, link_rate, time_slot, spmc="no", chassis="1"):
    global version
    if "UI5" == version: #if UI5, chassis-id should be there.
        cmd = "add signaling ss7 link bandwidth %s destination-point-code-name %s own-point-code-name %s type %s id %s node-name %s slc %s mtp2-link-profile %s mtp2-timer-profile %s number-of-time-slots %s pcm-id %s link-rate %s chassis-id %s time-slot %s spmc-link %s" %(bandwidth, dpc_name, opc_name, type, id, node, slc, link_profile, timer_profile, num_of_slots, pcm, link_rate, chassis, time_slot, spmc)
    else:
        cmd = "add signaling ss7 link bandwidth %s destination-point-code-name %s own-point-code-name %s type %s id %s node-name %s slc %s mtp2-link-profile %s mtp2-timer-profile %s number-of-time-slots %s pcm-id %s link-rate %s time-slot %s spmc-link %s" %(bandwidth, dpc_name, opc_name, type, id, node, slc, link_profile, timer_profile, num_of_slots, pcm, link_rate, time_slot, spmc)
    bus_fsclish(cmd)

    out = bus_fsclish("show signaling ss7 link id %s" %(id))
    if re.search("link-id", out):
        return True
    else:
        return False
        
def add_LINKSET(set_id, name, link_id):
    bus_fsclish("add signaling ss7 linkset id %s name %s link-id %s" %(set_id, name, link_id))
    
    out = bus_fsclish("show signaling ss7 linkset name %s" %(name))

    if re.search("linkset-name", out):
        return True
    else:
        return False

def add_LINKROUTE(name, dpc_name, linkset_name, id, priority, opc_name):
    bus_fsclish("add signaling ss7 route name %s destination-point-code-name %s linkset-name %s id %s priority %s own-point-code-name %s" %(name, dpc_name, linkset_name, id, priority, opc_name))

    out = bus_fsclish("show signaling ss7 route name %s" %(name))
    if re.search("route-name", out):
        return True
    else:
        return False

def set_LINKSTATE(id):
    if "UI5" == version:
        bus_fsclish("set signaling ss7 link admin-state enabled id %s" %(id))
    else:
        bus_fsclish("set signaling ss7 link config admin-state enabled id %s" %(id))

    out = bus_fsclish("show signaling ss7 link id %s" %(id))
    if re.search("ENABLED", out):
        return True
    else:
        return False


def add_NIF_POINT_CODE(local_as_name, type, mgw_id="1"):
    if "mgw_own" == type or "mgw_mate" == type:
        bus_fsclish("add signaling ss7 nif-point-code local-as-name %s type %s mgw-id %s" %(local_as_name, type, mgw_id))
    else:
        bus_fsclish("add signaling ss7 nif-point-code local-as-name %s type %s" %(local_as_name, type))

    out = bus_fsclish("show signaling ss7 nif-point-code name %s" %(local_as_name))

    if re.search("nif-point-code-name", out):
        return True
    else:
        return False

def add_NIF_POINT_CODE_OWN(local_as_name, type, mgw_id="1"):  # we need add NIF_POINT_CODE own firstly, so this funcation added.
    bus_fsclish("add signaling ss7 nif-point-code local-as-name %s type %s mgw-id %s" %(local_as_name, type, mgw_id))
    out = bus_fsclish("show signaling ss7 nif-point-code name %s" %(local_as_name))

    if re.search("nif-point-code-name", out):
        return True
    else:
        return False

def add_NIF_MGW_TO_SLC_BYDPC(slc, mgw_id, dpc_name):
    bus_fsclish("add signaling ss7 nif-mgw-to-slc slc %s mgw-id %s destination-point-code-name %s" %(slc, mgw_id, dpc_name))

    out = bus_fsclish("show signaling ss7 nif-mgw-to-slc name %s" %(dpc_name))
    if re.search("mgw-slc", out):
        return True
    else:
        return False

def add_NIF_MGW_TO_SLC_BYAS(slc, mgw_id, as_name):
    bus_fsclish("add signaling ss7 nif-mgw-to-slc slc %s mgw-id %s remote-as-name %s" %(slc, mgw_id, as_name))

    out = bus_fsclish("show signaling ss7 nif-mgw-to-slc name %s" %(as_name))
    if re.search("mgw-slc", out):
        return True
    else:
        return False


def add_NIF_RAN_POINT_CODE_BYDPC(point_type, dpc_name, e5_value="1"):
    if "UI5" == version:
        if "nnsf-ran" == point_type:
            cmd = "add signaling ss7 nif-ran-point-code type %s destination-point-code-name %s e5-value %s" %(point_type, dpc_name, e5_value)
        else:
            cmd = "add signaling ss7 nif-ran-point-code type %s destination-point-code-name %s" %(point_type, dpc_name)
    
        bus_fsclish(cmd)
    
        out = bus_fsclish("show signaling ss7 nif-ran-point-code name %s" %(dpc_name))
        if re.search("ran point code name", out):
            return True
        else:
            return False

    else:
        point_type = point_type.rstrip("-ran")
        if "nnsf" == point_type:
            cmd = "add signaling ss7 nif-destination-point-code mgw-function-type %s destination-point-code-name %s e5-value %s" %(point_type, dpc_name, e5_value)
        else:
            cmd = "add signaling ss7 nif-destination-point-code mgw-function-type %s destination-point-code-name %s" %(point_type, dpc_name)
    
        bus_fsclish(cmd)
    
        out = bus_fsclish("show signaling ss7 nif-destination-point-code name %s" %(dpc_name))
        if re.search('NIF-DESTINATION-POINT-CODE', out):
            return True
        else:
            return False


def add_NIF_RAN_POINT_CODE_BYAS(point_type, as_name, e5_value="1"):
    if "UI5" == version:
        if "nnsf-ran" == point_type:
            cmd = "add signaling ss7 nif-ran-point-code type %s remote-as-name %s e5-value %s" %(point_type, as_name, e5_value)
        else:
            cmd = "add signaling ss7 nif-ran-point-code type %s remote-as-name %s" %(point_type, as_name)

        bus_fsclish(cmd)
    
        out = bus_fsclish("show signaling ss7 nif-ran-point-code name %s" %(as_name))
        if re.search("ran point code name", out):
            return True
        else:
            return False

    else:
        point_type = point_type.rstrip("-ran")
        if "nnsf" == point_type:
            cmd = "add signaling ss7 nif-destination-point-code mgw-function-type %s remote-as-name %s e5-value %s" %(point_type, as_name, e5_value)
        else:
            cmd = "add signaling ss7 nif-destination-point-code mgw-function-type  %s remote-as-name %s" %(point_type, as_name)

        bus_fsclish(cmd)
    
        out = bus_fsclish("show signaling ss7 nif-destination-point-code name %s" %(as_name))
        if re.search('NIF-DESTINATION-POINT-CODE', out):
            return True
        else:
            return False

def add_SCCP_OPC_BYAS_SECONDARY(id, local_as_name, opc_type):
    bus_fsclish("add signaling sccp own-point-code id %s local-as-name %s type %s" %(id, local_as_name, opc_type))

    out = bus_fsclish("show signaling sccp own-point-code name %s" %(local_as_name))
    if re.search("SECONDARY", out):
        return True
    else:
        return False


def add_SCCP_OPC_BYAS_PRIMARY(id, local_as_name, opc_type, e5="1", lowest_slc="1"): # PRIMARY OPC should be added firstly.
    if "UI5" == version:
        bus_fsclish("add signaling sccp own-point-code id %s local-as-name %s type %s" %(id, local_as_name, opc_type))
        bus_fsclish("add signaling sccp e5-slc e5 %s lowest-slc %s point-code-name %s" %(e5, lowest_slc, local_as_name))
    else:
        bus_fsclish("add signaling sccp own-point-code id %s local-as-name %s e5 %s lowest-slc %s type %s" %(id, local_as_name, e5, lowest_slc, opc_type))

    out = bus_fsclish("show signaling sccp own-point-code name %s" %(local_as_name))
    if re.search("E5", out):
        return True
    else:
        return False


def add_SCCP_DPC_BYMTP3(id, mtp3_name, e5="1", lowest_slc="0"):
    if "UI5" == version:
        out = bus_fsclish("show signaling ss7 nif-ran-point-code  name %s" %(mtp3_name))
    else:
        out = bus_fsclish("show signaling ss7 nif-destination-point-code  name %s" %(mtp3_name))

    if ("UI5EP3" == version or "UI5EP2" == version or "UI5EP1" == version) and re.search("nnsf", out):
        bus_fsclish("add signaling sccp destination-point-code id %s e5 %s lowest-slc %s name %s" %(id, e5, lowest_slc, mtp3_name)) 
    elif "UI5" == version and re.search("nnsf-ran", out):
        bus_fsclish("add signaling sccp destination-point-code id %s name %s" %(id, mtp3_name))
        bus_fsclish("add signaling sccp e5-slc e5 %s lowest-slc %s point-code-name %s" %(e5, lowest_slc, mtp3_name))
    else:
        bus_fsclish("add signaling sccp destination-point-code id %s name %s" %(id, mtp3_name))
    
    out = bus_fsclish("show signaling sccp destination-point-code name %s" %(mtp3_name))
    if re.search("Point code Name", out):
        return True
    else:
        return False

def add_SCCP_DPC_BYAS(id, as_name, e5="1", lowest_slc="0"):
    if "UI5" == version:
        out = bus_fsclish("show signaling ss7 nif-ran-point-code name %s" %(as_name))
    else:
        out = bus_fsclish("show signaling ss7 nif-destination-point-code name %s" %(as_name))    

    if ("UI5EP3" == version or "UI5EP2" == version or "UI5EP1" == version) and re.search("nnsf", out):
        bus_fsclish("add signaling sccp destination-point-code id %s e5 %s lowest-slc %s remote-as-name %s" %(id, e5, lowest_slc, as_name))
    elif "UI5" == version and re.search("nnsf-ran", out):
        bus_fsclish("add signaling sccp destination-point-code id %s remote-as-name %s" %(id, as_name))
        bus_fsclish("add signaling sccp e5-slc e5 %s lowest-slc %s point-code-name %s" %(e5, lowest_slc, as_name))
    else:
        bus_fsclish("add signaling sccp destination-point-code id %s remote-as-name %s" %(id, as_name))

    out = bus_fsclish("show signaling sccp destination-point-code name %s" %(as_name))
    if re.search("Point code Name", out):
        return True
    else:
        return False



def add_SCCP_SUBSYSTEM(id, name, ssn, point_code_name):
    bus_fsclish("add signaling sccp subsystem id %s name %s ssn %s point-code-name %s" %(id, name, ssn, point_code_name))
    
    out = bus_fsclish("show signaling sccp subsystem name %s" %(name))
    if re.search("SCCP SubSystem Number", out):
        return True
    else:
        return False


def add_GT_ACTION(name, max_digi, min_digi, opera_code, skip_digi):
    bus_fsclish("add signaling global-title-translation action name %s maximum-digits %s minimum-digits %s operation-code %s skip-digits %s" %(name, max_digi, min_digi, opera_code, skip_digi))
    out = bus_cli("""fsclish -c "show signaling global-title-translation action  name %s" """ %(name))
    if re.search("action-name", out):
        return True
    else:
        return False


def add_GT_RULE(direction, encoding_scheme, GT_indicator, address_indicator, numbering_plan, tran_type, id, rule_name, *actions):
    action_name = ""
    for action in actions:
        action_name += "action-name %s " %(action)
    bus_fsclish("add signaling global-title-translation rule %s direction %s encoding-scheme %s global-title-indicator %s nature-of-address-indicator %s numbering-plan %s translation-type %s id %s name %s" %(action_name, direction, encoding_scheme, GT_indicator, address_indicator, numbering_plan, tran_type, id, rule_name))

    out = bus_fsclish("show signaling global-title-translation rule name %s" %(rule_name))
    if re.search("GTT Rule", out):
        return True
    else:
        return False


def add_GT_RESULT(name, tran_digi, *others):
    other_name = ""
    key = ""
    value = ""
    for i in others:
        key = i.split(':')[0]
        value = i.split(':')[1]
        other_name += "%s %s " %(key, value)

    bus_fsclish("add signaling global-title-translation result name %s translation-digits %s %s" %(name, tran_digi, other_name))

    out = bus_fsclish("show signaling global-title-translation result name %s" %(name))
    if re.search("SLNO", out):
        return True
    else:
        return False

def add_NNSF_RAN(gt, name, spc, id):
    bus_fsclish("add nnsf ran gt %s name %s spc %s id %s" %(gt, name, spc, id))

    out = bus_fsclish("show nnsf ran name %s" %(name))
    if re.search("SPC", out):
        return True
    else:
        return False


def add_NNSF_MSS(gt, name, nrilist, nullnri, id, mode, load):
    bus_fsclish("add nnsf mss gt %s name %s nrilist %s nullnri %s id %s" %(gt, name, nrilist, nullnri, id))
    bus_fsclish("set nnsf mss load-balancing id %s mode %s load %s" %(id, mode, load))

    out = bus_cli("""fsclish -c "show nnsf mss name %s" """ %(name))
    if re.search("Load mode" , out):
        return True
    else:
        return False


# find word equal to s from l.
def find_equal(s, l):
    for i in l:
        if s == i:
            return True
    return False

            


#BU_FSetupVarParser_Lib will generate a dictionary from cnf file. 
#get_sgw_parameter will generate sgw relevant parameters from upper dictionary
#sgw_parameter will be like:{'LOCAL_AS':[ ['1','SGWAS', '1', '251', 'NA0', 'loadshare'], ['2','IWFPC', '17', '255', 'NA0', 'loadshare'] ]
def get_sgw_parameter(dict):
    sgw_dict = {}
    temp_list = []
    sgw_items = ('SGW_TDM', 'SS7SGU', 'SAP', 'LOCAL_AS', 'REMOTE_AS', 'ASSOCIATION', 'SS7_OPC', 'SS7_DPC', 'LINK', 'LINKSET', 'LINKROUTE', 'LINKSTATE',\
                 'NIF_POINT_CODE_OWN', 'NIF_POINT_CODE','NIF_MGW_TO_SLC_BYDPC', 'NIF_MGW_TO_SLC_BYAS', 'NIF_RAN_POINT_CODE_BYDPC', \
                 'NIF_RAN_POINT_CODE_BYAS','SCCP_OPC_BYAS_PRIMARY','SCCP_OPC_BYAS_SECONDARY', 'SCCP_DPC_BYAS', 'SCCP_DPC_BYMTP3', 'SSN',\
                 'GT_ACTION','GT_RULE', 'GT_RESULT', 'NNSF_RAN', 'NNSF_MSS')
    for i in dict.keys():
        j = i.rsplit('_',1)[0]#split dict.keys into 2 parts,remove item name,leave item class,in case iteam name include other item class
        if find_equal(j, sgw_items): # find the fittest items which is in line with item class
            for k in dict[i].split(','):  # get parameter list
                temp_list.append(k.rstrip().strip())
            if sgw_dict.has_key(j):
                sgw_dict[j].append(temp_list) # if this item class is already one of sgw_dict's keys, append list to this key's value(it is a list) 
            else:
                sgw_dict[j] = [temp_list]    #else, create key-value pair: iteam class:[[list]]
            temp_list = [] # clear temp_list for next 
    
    return sgw_dict




def main_add_sgw(filename):
    global version
    if re.search("EP3", bus_cli("fsswcli -c")):
        version = "UI5EP3"
    elif re.search("UI50_5.4", bus_cli("fsswcli -c")):
        version = "UI5EP3"
    elif re.search("UI50_5.3", bus_cli("fsswcli -c")):
        version = "UI5EP2"
    elif re.search("UI50_5.2", bus_cli("fsswcli -c")):
        version = "UI5EP1"
    else:
        version = "UI5"
    lib_FS.FSetupVarParser.parsevars(filename)
    sgw_dict = get_sgw_parameter(lib_FS.FSetupVarParser.datacontent)
    parameter_list = []
    sgw_items = ('SGW_TDM', 'SS7SGU', 'SAP', 'LOCAL_AS', 'REMOTE_AS', 'ASSOCIATION', 'SS7_OPC', 'SS7_DPC', 'LINK', 'LINKSET', 'LINKROUTE', 'LINKSTATE',\
                 'NIF_POINT_CODE_OWN', 'NIF_POINT_CODE','NIF_MGW_TO_SLC_BYDPC', 'NIF_MGW_TO_SLC_BYAS', 'NIF_RAN_POINT_CODE_BYDPC', \
                 'NIF_RAN_POINT_CODE_BYAS','SCCP_OPC_BYAS_PRIMARY','SCCP_OPC_BYAS_SECONDARY', 'SCCP_DPC_BYAS', 'SCCP_DPC_BYMTP3', 'SSN',\
                 'GT_ACTION','GT_RULE', 'GT_RESULT', 'NNSF_RAN', 'NNSF_MSS')

    sgw_funcations = {'SGW_TDM':add_SGW_TDM, 'SS7SGU':add_SS7SGU, 'SAP':add_SAP, 'LOCAL_AS':add_LOCAL_AS, 'REMOTE_AS':add_REMOTE_AS,\
                      'ASSOCIATION':add_ASSOCIATION,\
                      'SS7_OPC':add_SS7_OPC, 'SS7_DPC':add_SS7_DPC,'LINK':add_LINK,'LINKSET':add_LINKSET,'LINKROUTE':add_LINKROUTE,\
                      'LINKSTATE': set_LINKSTATE, 'NIF_POINT_CODE_OWN':add_NIF_POINT_CODE_OWN, 'NIF_POINT_CODE':add_NIF_POINT_CODE,\
                      'NIF_MGW_TO_SLC_BYDPC':add_NIF_MGW_TO_SLC_BYDPC,'NIF_MGW_TO_SLC_BYAS':add_NIF_MGW_TO_SLC_BYAS, \
                      'NIF_RAN_POINT_CODE_BYDPC':add_NIF_RAN_POINT_CODE_BYDPC,'NIF_RAN_POINT_CODE_BYAS':add_NIF_RAN_POINT_CODE_BYAS,\
                      'SCCP_OPC_BYAS_PRIMARY':add_SCCP_OPC_BYAS_PRIMARY,'SCCP_OPC_BYAS_SECONDARY':add_SCCP_OPC_BYAS_SECONDARY,\
                      'SCCP_DPC_BYAS':add_SCCP_DPC_BYAS,'SCCP_DPC_BYMTP3':add_SCCP_DPC_BYMTP3,'SSN':add_SCCP_SUBSYSTEM, \
                      'GT_ACTION':add_GT_ACTION, 'GT_RULE':add_GT_RULE, 'GT_RESULT':add_GT_RESULT,'NNSF_RAN':add_NNSF_RAN,\
                      'NNSF_MSS':add_NNSF_MSS}
    try:
        for i in range(len(sgw_items)):
            if sgw_dict.has_key(sgw_items[i]): # if item of config file exist in sgw_items, then config it, else go to next one.
                for l in sgw_dict[sgw_items[i]]: #One item can have several parameter set. Eg. We have several link
                    #combine parameter set as one parameter
                    for j in l:  
                        parameter_list.append(j)
                   # print parameter_list

                    func = sgw_funcations.get(sgw_items[i])
                    if func:
                        if not func(*parameter_list):
                            return False
                        else:
                            time.sleep(1)
                    else:
                        log("funcation for %s doesn't exist" %(sgw_items[i]))
                        return False
                    parameter_list = []  # set parameter_list empty for next item starting
        return True
    #if user Ctrl-c to interrupte script execution, then clear all SGW related configuration.
    except KeyboardInterrupt:
        log("KeyboardInterrupt, delete all SGW resource")
        lib_delete_sgw.clear_all()
        
        

        
def get_paraFileName_for_different_cluster():
    paraFileName = ""
    if g_debug:
        clusterID = os.popen('cat /etc/cluster-id ').read().strip()
        paraFileName = '/root/BUS/testsuite/Q1_2_configuration_add_and_delete/cnf/cnf_' + clusterID
    else:
        clusterID = cli('cat /etc/cluster-id ')
        paraFileName =  '/home/automation/cnf/cnf_' + clusterID
    return paraFileName 

        
if __name__ == '__main__':
    filename = get_paraFileName_for_different_cluster()
    print main_add_sgw(filename)
