#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.11.23
# 2013.5.15 improve it by YHJ
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import time
import re
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_re = boaimport('BU_FU_restart_lib')

if g_debug:
    VERDICT_FAIL = 0 
    VERDICT_PASS = 1 
    def verdict(v):   #simulate funcation of BOA, Verdict case pass or fail
        if v == VERDICT_FAIL:
            print 'FAIL'
        else:
            print 'PASS'

    def log(str):
        return lib_re.log(str)

    def cli(str):
        return lib_re.cli(str)

    def stop_tc_execution():
        return lib_re.stop_tc_execution()

    def wait_trigger():
        return lib_re.wait_trigger()

    def send_trigger():
         return lib_re.send_trigger()

wait_trigger()
i = 0
while i<3:
    out = cli("hwcli -r -n -o off AHUB3-A-1-8")
    if re.search("ok", out):
        time.sleep(3)
        log("sleep 3s to wait multi-homing take effect")
        log("AHUB8 reset successfully, go to run test cases")
        break
    else:
        i = i + 1
if i == 3:
    log("AHUB8 reset failed after 3 try")
    verdict(VERDICT_FAIL)
    stop_tc_execution()

i = 0  #set i back to 0
#run test case after AHUB8 reset 
send_trigger()

wait_trigger()
log("sleep 210s to wait AHUB8 start up, then reset AHUB9")
time.sleep(210)
while i<3:
    out = cli("hwcli -r -n -o off AHUB3-A-1-9")
    if re.search("ok", out):
        time.sleep(3)
        log("sleep 3s to wait multi-homing take effect")
        log("AHUB9 reset successfully, go to run test cases")
        break
    else:
        i = i + 1
if i == 3:
    log("AHUB9 reset failed after 3 try")
    verdict(VERDICT_FAIL)
    stop_tc_execution()
#run test case after AHUB9 reset
send_trigger()
