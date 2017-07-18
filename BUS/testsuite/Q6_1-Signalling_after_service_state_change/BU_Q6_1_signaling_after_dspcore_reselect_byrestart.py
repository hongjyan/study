#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.27
#=====================================================================
#/usr/bin/python
import random
import sys
sys.path.append("../lib")
g_debug = True
if g_debug:
    from boa_simulate import *

lib_dsp = boaimport('BU_dspcore_lib')

if g_debug:
    VERDICT_FAIL = 0
    VERDICT_PASS = 1
    def verdict(v):
        if v:
            print "PASS"
        else:
            print "FAIL"

    def log(str):
        return lib_dsp.log(str)

    def cli(str):
        return lib_dsp.cli(str)

    def stop_tc_execution():
        return lib_dsp.stop_tc_execution()

    def wait_trigger():
        return lib_dsp.wait_trigger()

    def send_trigger():
         return lib_dsp.send_trigger()

wait_trigger()
coreTCU = lib_dsp.get_signallingOK_cores()
if len(coreTCU) < 2:
    verdict(VERDICT_FAIL)
    log("OK dsp core for signalling is less than 2, exit")
    stop_tc_execution()
random.shuffle(coreTCU)
twoTCU = coreTCU[0:2]
otherTCU = coreTCU[2:]
#leave just 2 signalling core in service, lock the other signalling cores
for core in otherTCU:
    lib_dsp.change_sigcores_state('lock', core[0], core[1])
#restart the second sig core, make just the first core in service
lib_dsp.change_sigcores_state('restart', twoTCU[1][0], twoTCU[1][1])
send_trigger()
#execute TW case

wait_trigger()
#restart the first sig core, leave just the second core in service
lib_dsp.change_sigcores_state('restart', twoTCU[0][0], twoTCU[0][1])
send_trigger()
#execure TW case

wait_trigger()
#unlock the other sig cores
for core in otherTCU:
    lib_dsp.change_sigcores_state('unlock', core[0], core[1])
send_trigger()
