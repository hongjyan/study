#!/usr/bin/python
#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# 2013.5.28 modified. Active/coldstandy should be in UI5
#=====================================================================
import sys  # not used in BOA
import re
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_MO = boaimport('BU_MOState_lib')

if g_debug:
    VERDICT_FAIL = 0
    VERDICT_PASS = 1
    def verdict(v):   #simulate funcation of BOA, Verdict case pass or fail
        if v == VERDICT_FAIL:
            print 'FAIL'
        else:
            print 'PASS'

    def log(str):
        return lib_MO.log(str)
 
    def cli(str):
        return lib_MO.cli(str)
 
    def stop_tc_execution():
        return lib_MO.stop_tc_execution()
 
    def wait_trigger():
        return lib_MO.wait_trigger()
 
    def send_trigger():
         return lib_MO.send_trigger()


CLASS_CLA0_CSS = lib_MO.class_MOState("/CLA-0/FSClusterStateServer")
CLASS_CLA1_CSS = lib_MO.class_MOState("/CLA-1/FSClusterStateServer")
CLA0_CSS = CLASS_CLA0_CSS.MO_is_OK()
CLA1_CSS = CLASS_CLA1_CSS.MO_is_OK()

if (CLA0_CSS[0] and CLA0_CSS[1] == "ACTIVE" and CLA1_CSS[0] and CLA1_CSS[1] == "HOTSTANDBY")\
or (CLA0_CSS[0] and CLA0_CSS[1] == "HOTSTANDBY" and CLA1_CSS[0] and CLA1_CSS[1] == "ACTIVE"):
    verdict(VERDICT_PASS)
elif (CLA0_CSS[0] and CLA0_CSS[1] == "ACTIVE" and CLA1_CSS[0] and CLA1_CSS[1] == "COLDSTANDBY")\
or (CLA0_CSS[0] and CLA0_CSS[1] == "COLDSTANDBY" and CLA1_CSS[0] and CLA1_CSS[1] == "ACTIVE"):
    verdict(VERDICT_PASS)
else:
    verdict(VERDICT_FAIL)
