#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA
g_debug = True
if g_debug:
    from boa_simulate import *

lib_MO = boaimport('BU_MOState_lib')

if g_debug:
    VERDICT_PASS = 1
    VERDICT_FAIL = 0
    def verdict(v):
        if v:
            print "PASS"
        else:
            print "FAIL"
            
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


CLASS_CLA0_SCLI = lib_MO.class_MOState("/CLA-0/FSSCLIFwkServer")
CLASS_CLA1_SCLI= lib_MO.class_MOState("/CLA-1/FSSCLIFwkServer")
CLA0_SCLI = CLASS_CLA0_SCLI.MO_is_OK()
CLA1_SCLI = CLASS_CLA1_SCLI.MO_is_OK()
if (CLA0_SCLI[0]  and CLA0_SCLI[1] == 'ACTIVE' and CLA1_SCLI[0]  and CLA1_SCLI[1] == 'ACTIVE')\
 or (CLA0_SCLI[0] and CLA0_SCLI[1] == 'ACTIVE' and CLA1_SCLI[0]  and CLA1_SCLI[1] == 'ACTIVE'):
    verdict(VERDICT_PASS)
else:
    verdict(VERDICT_FAIL)
