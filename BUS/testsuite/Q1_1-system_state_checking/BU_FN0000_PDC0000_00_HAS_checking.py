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


cmd = "fscmfcli -s /CLA-0"
output = cli(cmd)
ls=output.split('\n')
s0 = ls[0].split()[1]
s1 = ls[1].split()[1]
if s0 == 'CMF-SERVING' and s1 == 'CMF-BACKUP' or s1 == 'CMF-SERVING' and s0 == 'CMF-BACKUP':
    verdict(VERDICT_PASS)
else:
    log('CMF is NOK')
    verdict(VERDICT_FAIL)

