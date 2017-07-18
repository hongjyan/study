#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.11.7
# add clock_is_OK by YHJ 2013.4.26
# clock_is_OK suitable both in Ui5 and Ui5EP1. 2013.5.28.
#=====================================================================
import sys  # not used in BOA
import re
sys.path.append("../lib")  # not used in BOA
g_debug = True
if g_debug:
    from boa_simulate import *

lib_Base = boaimport('BU_Base_lib')

if g_debug:
    VERDICT_PASS = 1
    VERDICT_FAIL = 0
    def verdict(v):
        if v:
            print "PASS"
        else:
            print "FAIL"
            
    def log(str):
        return lib_Base.log(str)
 
    def cli(str):
        return lib_Base.cli(str)

def OMU_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type OMU\""
    output = cli(cmd)
    OMU_FUState = []
    for l in output.splitlines():
        if re.search("OMU", l):
            OMU_FUState.append(l)
    for i in OMU_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def CM_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type CM\""
    output = cli(cmd)
    CM_FUState = []
    for l in output.splitlines():
        if re.search("CM", l):
            CM_FUState.append(l)
    for i in CM_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def HCLB0_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type HCLB\""
    output = cli(cmd)
    HCLB0_FUState = []
    for l in output.splitlines():
        if re.search("HCLB-0|HCLB-1", l):
            HCLB0_FUState.append(l)
    for i in HCLB0_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def SISU_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type SISU\""
    output = cli(cmd)
    SISU_FUState = []
    for l in output.splitlines():
        if re.search("SISU", l):
            SISU_FUState.append(l)
    for i in SISU_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def DSPM_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type DSPM\""
    output = cli(cmd)
    DSPM_FUState = []
    for l in output.splitlines():
        if re.search("DSPM", l): 
            DSPM_FUState.append(l)
    for i in DSPM_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def IPNIU_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type IPNIU\""
    output = cli(cmd)
    IPNIU_FUState = []
    for l in output.splitlines():
        if re.search("IPNIU", l): 
            IPNIU_FUState.append(l)
    for i in IPNIU_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def TDMSNIUP_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type TDMSNIUP\""
    output = cli(cmd)
    TDMSNIUP_FUState = []
    for l in output.splitlines():
        if re.search("TDMSNIUP", l): 
            TDMSNIUP_FUState.append(l)
    for i in TDMSNIUP_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def TDMMGU_service_is_OK():
    cmd = "fsclish -c \"show has functional-unit unit-info unit-type TDMMGU\""
    output = cli(cmd)
    TDMMGU_FUState = []
    for l in output.splitlines():
        if re.search("TDMMGU", l): 
            TDMMGU_FUState.append(l)
    for i in TDMMGU_FUState:
        if i.split()[3] == "WO-EX":
            return True
    return False

def clock_is_OK():
    cmd ="fsclish -c \"show mgw synchronization inputreference\""
    output = cli(cmd)
    for l in output.splitlines():
        if re.search("SET-|set-", l) and re.search("YES|yes", l):
            return True
    return False

def main():
    if OMU_service_is_OK() and CM_service_is_OK() and HCLB0_service_is_OK() and SISU_service_is_OK() and clock_is_OK() \
    and DSPM_service_is_OK() and IPNIU_service_is_OK() and TDMSNIUP_service_is_OK() and TDMMGU_service_is_OK():
        verdict(VERDICT_PASS)
    else:
        verdict(VERDICT_FAIL)
main()
            
