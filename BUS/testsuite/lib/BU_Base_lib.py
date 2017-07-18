#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.9.17
# 2013.5.8 delete TCUMap, use tcu.replace('-','') by YHJ.
# 2013.5.21 use log replaced of print
# 2013.5.22 "/" won't be removed in GetAvailableRUs() and GetAvailableRGs()
# so "/" won't be added on the return of those 2 functions when usinglib_MO.MO_state()
#=====================================================================
import os
import re
import time

g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *

lib_MO=boaimport('BU_MOState_lib')

if g_debug:
    def cli(cmd):
        return lib_MO.cli(cmd)

    def log(str):
        return lib_MO.log(str)

    def report_fail():
        return lib_MO.report_fail()

    def report_pass():
        return lib_MO.report_pass()


# get nodes which are available show by hwcli, filter the unpluged and unkown ones
# for example:
# IPNI1P-1          : node available /cabinet-1/chassis-1/piu-10/AMC-B1/CPU-0/core-0,1
# SE-1              : node available /cabinet-1/chassis-1/piu-10/AMC-B1/CPU-0/core-2,3,4,5,6,7,8,9,10,11
# IPNI10P-3         : node unplugged /cabinet-1/chassis-1/piu-10/AMC-B2/CPU-0/core-0,1
# SE-3              : node unplugged /cabinet-1/chassis-1/piu-10/AMC-B2/CPU-0/core-2,3,4,5,6,7,8,9,10,11
# TDMNIP-5          : node available /cabinet-1/chassis-1/piu-10/AMC-B3/CPU-0/core-0
# TDMNIP-7          : node unplugged /cabinet-1/chassis-1/piu-10/AMC-B4/CPU-0/core-0
# TCU-4             : node unplugged /cabinet-1/chassis-1/piu-11/LMP-0/CPU-0/core-0
#
# the nodes such as IPNI10P-3, SE-3, TDMNIP-5, TCU-4 wont be  in the list
def GetAvailableNodes():
    nodes = []
    cmd = 'hwcli -q -o off -e ^[^d]'
    output = cli(cmd)
    ls=output.rstrip('\n').split('\n')
    for l in ls:
        if l=='\r' or l == '':
            continue
        h = l.split(':')
        if not re.search("^SE",h[0]) and not re.search("^LMP",h[0]):
            nodes.append('/' + h[0].strip(' ')) 
    return nodes

def IsNodeAvailable(nd):
    cmd = 'hwcli -eo off '+nd
    output = cli(cmd)
    if re.search("available", output):
	return True
    return False

def IsMOEnabled(mo):
    cmd = 'hascli -s '+mo
    output = cli(cmd)
    if re.search("ENABLED", output):
	return True
    return False
    
def GetRUsByNode(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\' | awk -F / \'{print $3}\''
    output = cli(cmd).strip()
    li = output.split("\n")
    return li

def GetRUsByNodeWithActiveRole(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\''
    output = cli(cmd).strip()
    rus = output.split("\n")
    new_rus = []
    for ru in rus: 
	cmd = 'hascli -s '+ru
	output = cli(cmd)
	if re.search('role\(ACTIVE\)', output):
	    new_rus.append(ru)
    return new_rus

def GetRUsByNodeWithoutActiveRole(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\''
    output = cli(cmd).strip()
    rus = output.split("\n")
    new_rus = []
    for ru in rus: 
	cmd = 'hascli -s '+ru
	output = cli(cmd)
	if not re.search('role\(ACTIVE\)', output):
	    new_rus.append(ru)
    return new_rus

def GetRUsByNodeWithColdStandByRole(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\''
    output = cli(cmd).strip()
    rus = output.split("\n")
    new_rus = []
    for ru in rus: 
	cmd = 'hascli -s '+ru
	output = cli(cmd)
	if re.search('role\(COLDSTANDBY\)', output):
	    new_rus.append(ru)
    return new_rus

def GetRUsByNodeWithoutColdStandByRole(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\''
    output = cli(cmd).strip()
    rus = output.split("\n")
    new_rus = []
    for ru in rus: 
	cmd = 'hascli -s '+ru
	output = cli(cmd)
	if not re.search('role\(COLDSTANDBY\)', output):
	    new_rus.append(ru)
    return new_rus
    
def GetRUsByNodeWithHotStandByRole(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\''
    output = cli(cmd).strip()
    rus = output.split("\n")
    new_rus = []
    for ru in rus: 
	cmd = 'hascli -s '+ru
	output = cli(cmd)
	if re.search('role\(HOTSTANDBY\)', output):
	    new_rus.append(ru)
    return new_rus

def GetRUsByNodeWithoutHotStandByRole(node):
    cmd = 'hascli --children '+node + ' | awk -F / \'NF<=3\''
    output = cli(cmd).strip()
    rus = output.split("\n")
    new_rus = []
    for ru in rus: 
	cmd = 'hascli -s '+ru
	output = cli(cmd)
	if not re.search('role\(HOTSTANDBY\)', output):
	    new_rus.append(ru)
    return new_rus


def shorten(rus):
    short_rus = []
    for ru in rus:
	cmd = 'echo '+ru+' | awk -F / \'{print $3}\''
	output = cli(cmd)
	short_rus.append(output.rstrip())
    return short_rus

# get all RecoverGroups which contains at least one available HW node
# filter the RGs which contains both missing HW.
# exmaple:
# if IPNI1P-0, and IPNI1P-1 is all missing
# then the RG MGW_IPNIURG-0 is not allow to be in the return list
def GetAvailableRGs():
    nodes = GetAvailableNodes() 
    rgs = []
    for nd in nodes:
        cmd = 'hascli --children ' + nd + ' | awk -F/ \'NF<=3\' | xargs hascli --parent | sort | uniq | grep -v ' + nd
#       output = connections.execute_mml_without_check(cmd)
        output = cli(cmd)
        ls=output.split('\n')
        for l in ls:
            rg = l.rstrip()
            if rg == '':
                continue
            if rg == '/FTP':
                continue
            rgs.append(rg) 
    #uniq the rgs
    rgs = list(set(rgs))


    #get the unplgged TCUs
    unplugged_TCUs = GetUnpluggedTCUs()

    #delete DSPMRG rg in TCU-0
    deleteRGs = []
    for rg in rgs:
        if re.search('^/MGW_DSPMRG', rg):
            for tcu in unplugged_TCUs:
                if rg == '/MGW_DSPMRG' + tcu.strip('/').replace('-',''):
                    deleteRGs.append(rg)

    for rg in deleteRGs:
        rgs.remove(rg)
    return rgs

 
# get all RUs which contains at least one available HW node
# filter the RUs which contains both missing HW.
def GetAvailableRUs():
    nodes = GetAvailableNodes() 
	#collections of ru
    rus = []
    for nd in nodes:
        cmd = 'hascli --children ' + nd + ' | awk -F/ \'NF==3\' | sort | uniq'
        output = cli(cmd)
        ls=output.split('\n')
        for l in ls:
            ru = l.rstrip()
            if ru == '':
                continue
            if ru == '/CLA-0/FSFTPServer':
                continue
            if ru == '/CLA-1/FSFTPServer':
                continue
            rus.append(ru)


    #get the unplgged TCUs
    unplugged_TCUs = GetUnpluggedTCUs()

    #delete DSPMRG rg in TCU-0
    deleteRUs = []
    for ru in rus:
        if re.search('^/TCU-0/MGW_DSPMRU', ru):
            for tcu in unplugged_TCUs:
                if ru == '/TCU-0/MGW_DSPMRU' + tcu.strip('/').replace('-','') + '-0':
                    deleteRUs.append(ru)

    for ru in deleteRUs:
        rus.remove(ru)

    return rus


# get all Processes which contains at least one available HW node
# filter the Processes which contains both missing HW.
def GetAvailableProcess():
    nodes = GetAvailableNodes() 
    rus = []
    for nd in nodes:
        cmd = 'hascli --children ' + nd + ' | awk -F/ \'NF==4\' | sort | uniq'
        output = cli(cmd)
        ls=output.split('\n')
        for l in ls:
            ru = l.rstrip()
            if ru == '':
                continue
            if ru == '/CLA-0/FSFTPServer':
                continue
            if ru == '/CLA-1/FSFTPServer':
                continue
            rus.append(ru.lstrip(ru[0]))

    #setup TCU Map
    TCUMap = {'TCU-0':'TCU0', 'TCU-1':'TCU1', 'TCU-2':'TCU2', 'TCU-3':'TCU3', 'TCU-4':'TCU4', 'TCU-5':'TCU5', 'TCU-6':'TCU6', 'TCU-7':'TCU7', 'TCU-8':'TCU8', 'TCU-9':'TCU9'}

    #get the unplgged TCUs
    unplugged_TCUs = GetUnpluggedTCUs()

    #delete DSPMRG rg in TCU-0
    deleteRUs = []
    for ru in rus:
        if re.search('^/TCU-0/MGW_DSPMRU', ru):
            for tcu in unplugged_TCUs:
                if ru == '/TCU-0/MGW_DSPMRU' + TCUMap[tcu] + '-0':
                    deleteRUs.append(ru)

    for ru in deleteRUs:
        rus.remove(ru)

    return rus


#return a list of string such as "['TCU-0', 'TCU-1']"
#all TCU-x by default, including missing HWs.
def GetAllTCUs():
    tcus = []
    cmd = 'hwcli -o off  -e ^[^d] | grep TCU'
    output = cli(cmd)
    ls = output.split('\n')
    for l in ls:
        tcu = l.split(':')[0].rstrip()
        if tcu == '':
            continue
        tcus.append(tcu)
    return tcus

# return a list of  Available HWs showed by "hwcli" commands
# paramater obj could be any Node name, RG name, RU name, or evern dsp core name, 
# such as 'TCU', 'TDMNIP', 'CLA'
# '/TCU-1', the '/', '-1', and '-0' is no need to provide, just 'TCU'
# and it will list all available TCUs in a list, such as '["TCU-1","TCU-2"]'
def GetAvailableHWs(obj):
    hws = []
    cmd = 'hwcli -q -o off  -e ^[^d] | grep '+obj
    output = cli(cmd)
    ls = output.split('\n')
    for l in ls:
        hw = l.split(':')[0].rstrip()
        if hw == '':
            continue
        hws.append(hw)
    return hws    

#return avalable TCUs which shows 'available' by 'hwcli' command
# '/TCU-1', the '/', '-1', and '-0' is no need to provide, just 'TCU'
# and it will list all available TCUs in a list, such as '["TCU-1","TCU-2"]'
def GetAvailableTCUs():
    return GetAvailableHWs('/TCU')

#return avalable TCUs which shows 'available' by 'hwcli' command
# '/IPNI1P-0', the '/', '-1', and '-0' is no need to provide, just 'TCU'
# and it will list all available IPNI1Ps in a list, such as '["IPNI1P-0","IPNI1P-1"]'
def GetAvailableIPNIUs():
    return GetAvailableHWs('/IPNI1P')

#return avalable TCUs which shows 'available' by 'hwcli' command
# '/TDMNIP-4', the '/' and '-4'  is no need to provide, just 'TCU'
# and it will list all available TDMNIPs in a list, such as '["TDMNIP-4","IPNI1P-5"]'
def GetAvailableTDMNIPs():
    return GetAvailableHWs('/TDMNIP')

# return True of False to detect whether the Funtional Unit can be Swtich over or not
# paramater 1 is the unit type,  it could be any of these shows below:
#      CM         
#      DSPM       
#      DSPOAM     
#      HCLB       
#      IPNIU      
#      OMU        
#      SAGU       
#      SCLIU      
#      SISU       
#      SS7UP      
#      TDMMGU     
#      TDMSNIUP  
# parameter 2 is the unit index, it depends on the unit-type
def CanFUSwitchover(ut, index):
    s = []
    cmd = 'fsclish -c \"show functional-unit unit-info unit-type ' + ut +'\" | grep ' + ut
    output = cli(cmd)
    ls = output.split('\n')
    for l in ls:
        l = l.rstrip()
        if l == '':
            continue
        d = l.split()
        s.append(d[3])
    i = int(index) 

	#only 'WO-EX' and 'SP-EX' could be switch over
    if (i%2==0) and (((s[i] == 'WO-EX') and (s[i+1]== 'SP-EX')) or ((s[i+1] == 'WO-EX') and (s[i]== 'SP-EX'))) :
        return True
    if (i%2==1) and(((s[i] == 'WO-EX') and (s[i-1]== 'SP-EX')) or ((s[i-1] == 'WO-EX') and (s[i]== 'SP-EX'))) :
        return True
    return False
            

# return a list of TCU that is 'unplugged' showed by 'hwcli' command
# such as ['TCU-0', 'TCU-4']
def GetUnpluggedTCUs():
    tcus = []
    cmd = 'hwcli -o off  -e ^[^d] | grep TCU | grep unplugged'
    output = cli(cmd)
    ls = output.split('\n')
    for l in ls:
        tcu = l.split(':')[0].rstrip()
        if tcu == '':
            continue
        tcus.append("/" + tcu)
    return tcus

# return a list of MOState object
# parameter is a lsit of MO object, could be Node, RG, RU, and Process
# such as '/CLA-0', '/SSH', 'CLA-0/MGW_OMUFU'
def GetAvailableNodesState(nodes):
    mos = []
    for n in nodes:
        mo=lib_MO.class_MOState(n)
        mos.append(mo)
    return mos

# verdict whether all MO objects is OK or not
# only all of the MO objects are OK it return True
def test_mo_state_from_object_list(ojs):
    passed = True
    for oj in ojs:
        mo = lib_MO.class_MOState(oj)
        if mo.MO_is_OK()[0] == False:
            log("%s is not OK\n\n" %mo.MODic['name'])
            passed = False
    if passed:
        return True
    else:
        return False


# verdict whethere all MO objects are OK or not
# only all of the MO objects are OK it return True
# prameter 1 is the obj_type, there are 4 types of MO State 
# 1.NODE
# 2.RG
# 3.RU
# 4.PROCESS
# parameter 2 kept as a history reason, plz ignore it
def all_objects_should_be_available(obj_type):
    if obj_type == "NODE":
        ojs = GetAvailableNodes()
    if obj_type == "RG":
        ojs = GetAvailableRGs()
    if obj_type == "RU":
        ojs = GetAvailableRUs()
    if obj_type == "PROCESS":
        ojs = GetAvailableProcess()
    return test_mo_state_from_object_list(ojs)

# a list of hard code of important RGs
def get_important_rgs():
    return ['MGW_OMURG', 'MGW_CMRG', 'MGW_HCLBRG-0', 'MGW_HCLBRG-1', 'MGW_SISURG-0', 'MGW_SISURG-1', 'MGW_SISURG-2', 'MGW_SISURG-3']

# return a list of RUs which come from the important RGs
def get_important_rus():
    rgs = get_important_rgs()
    rus = []
    for rg in rgs:
        cmd = 'hascli --children ' + rg + ' | awk -F/ \'NF==3\' | sort | uniq'
        output = cli(cmd)
        ls=output.split('\n')
        for l in ls:
            ru = l.rstrip()
            if ru == '':
                continue
            rus.append(ru.lstrip(ru[0]))

    #setup TCU Map
    TCUMap = {'TCU-0':'TCU0', 'TCU-1':'TCU1', 'TCU-2':'TCU2', 'TCU-3':'TCU3', 'TCU-4':'TCU4', 'TCU-5':'TCU5', 'TCU-6':'TCU6', 'TCU-7':'TCU7', 'TCU-8':'TCU8', 'TCU-9':'TCU9'}

    #get the unplgged TCUs
    unplugged_TCUs = GetUnpluggedTCUs()

    #delete DSPMRG rg in TCU-0
    deleteRUs = []
    for ru in rus:
        if re.search('^/TCU-0/MGW_DSPMRU', ru):
            for tcu in unplugged_TCUs:
                if ru == '/TCU-0/MGW_DSPMRU' + TCUMap[tcu] + '-0':
                    deleteRUs.append(ru)

    for ru in deleteRUs:
        rus.remove(ru)

    return rus

# check the state of CMF RG
# will rais error is it is failed.
def check_cmf_state():
    cmd = "fscmfcli -s /CLA-0"
    output = cli(cmd)
    ls=output.split('\n')
    s0 = ls[0].split()[1]
    s1 = ls[1].split()[1]
    if s0 == 'CMF-SERVING' and s1 == 'CMF-BACKUP' or s1 == 'CMF-SERVING' and s0 == 'CMF-BACKUP':
        pass
    else:
        log('CMF is NOK')
        report_fail()

def is_state_ok(s):
    if len(s) == 2:
        return s[0] == "WO-EX" and s[1] == "SP-EX" or s[0] == "SP-EX" and s[1] == "WO-EX"
    elif len(s) == 4:
        return (s[0] == "WO-EX" and s[1] == "SP-EX" or s[0] == "SP-EX" and s[1] == "WO-EX") \
            and (s[2] == "WO-EX" and s[3] == "SP-EX" or s[2] == "SP-EX" and s[3] == "WO-EX")
    elif len(s) == 8:
        return (s[0] == "WO-EX" and s[1] == "SP-EX" or s[0] == "SP-EX" and s[1] == "WO-EX") \
            and (s[2] == "WO-EX" and s[3] == "SP-EX" or s[2] == "SP-EX" and s[3] == "WO-EX") \
            and (s[4] == "WO-EX" and s[5] == "SP-EX" or s[4] == "SP-EX" and s[5] == "WO-EX") \
            and (s[6] == "WO-EX" and s[7] == "SP-EX" or s[6] == "SP-EX" and s[7] == "WO-EX") 


# verdict the function unit is OK or not
# paramater 1 is the unit type,  it could be any of these shows below:
#      CM         
#      DSPM       
#      DSPOAM     
#      HCLB       
#      IPNIU      
#      OMU        
#      SAGU       
#      SCLIU      
#      SISU       
#      SS7UP      
#      TDMMGU     
#      TDMSNIUP  
def check_function_unit(unit_type):
    cmd = "fsclish -c \"" + "show functional-unit unit-info unit-type " + unit_type + "\""
    output = cli(cmd)
    ls = output.split('\n')
    states = []
    ru_monames = []
    matched_lines = []
    for r in ls:
        if re.search('^'+unit_type, r):
            c = r.split()
            states.append(c[3])
            ru_monames.append(c[5])
            matched_lines.append(r)

    if unit_type == "DSPM":
        tcus = GetAvailableTCUs()
        for tcu in tcus:
            for l in matched_lines:
                if re.search('/'+tcu, l):
                    if not l.split()[3] == "WO-EX":
                        log(unit_type + ' is NOK')
                        report_fail()
        return

    if unit_type == "IPNIU":
        us = GetAvailableIPNIUs()
        for u in us:
            for l in matched_lines:
                if re.search('/'+u, l):
                    if not l.split()[3] == "WO-EX":
                        log(unit_type + ' is NOK')
                        report_fail()
        return

    if unit_type == "TDMSNIUP":
        us = GetAvailableTDMNIPs()
        for u in us:
            for l in matched_lines:
                if re.search('/'+u, l):
                    if not l.split()[3] == "WO-EX":
                        log(unit_type + ' is NOK')
                        report_fail()
        return
            
    if not is_state_ok(states):
        log(unit_type + ' is NOK')
        report_fail()
    

# verdict the the plugged DSP cores 
# for exmaple:
# hascli -s /TCU-1
# /TCU-1:
# administrative(UNLOCKED)
# operational(ENABLED)
# usage(ACTIVE)
# procedural()
# availability()
# unknown(FALSE)
# alarm()
# Dynamic attributes:
#   FAULTY_DSPS = 000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000-000000
#     OOSYNCH_DSPS = 111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111-111111
# 
# the FAULTY_DSPS is all 0, so it is OK, else if only one 1 shows, it is failed
def check_dsp_cores():
    tcus = GetAvailableTCUs()
    for tcu in tcus: 
        cmd = "fshascli -s " + tcu
        output = cli(cmd)
        ls = output.split('\n')
        for l in ls:
            if re.search('FAULTY_DSPS', l):
                if not re.search('0', l):
                    log('dsp core is NOK')
                    report_fail()
    
# verdict a single MO object
# parameter 1 is the OM object
# such as '/', '/CLA-1', 'TCU-1/FSDMPServer'
def check_single_obj(obj):
    mo = lib_MO.class_MOState(obj)
    if not mo.MO_is_OK()[0]:
        log(obj +' is NOK')
        return False
    return True

# return a string of the MO object's role
# parameter 1 is the MO object
# such as '/', '/CLA-1', 'TCU-1/FSDMPServer'
# example:
# give the parameter '/TCU-1/FSDMPServer'
# if the situation like
# /TCU-1/FSDMPServer:
# administrative(UNLOCKED)
# operational(ENABLED)
# usage(ACTIVE)
# procedural()
# availability()
# unknown(FALSE)
# alarm()
# role(ACTIVE)
# it will return 'ACTIVE'
def get_obj_role(obj):
    mo = lib_MO.class_MOState(obj)
    if 'role' in mo.MODic.keys():
        return mo.MODic['role']
    else:
        return 'None'
