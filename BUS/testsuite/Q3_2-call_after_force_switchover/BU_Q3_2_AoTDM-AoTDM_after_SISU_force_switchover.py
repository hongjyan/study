#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.24. change hard code, select one DSPM RG from system.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_fsw = boaimport('BU_FU_forceSwitchover_lib')
lib_RG = boaimport('BU_RG_lib')

if g_debug:
    def log(str):
        return lib_fsw.log(str)

    def cli(str):
        return lib_fsw.cli(str)

    def stop_tc_execution():
        return lib_fsw.stop_tc_execution()

    def wait_trigger():
        return lib_fsw.wait_trigger()

    def send_trigger():
         return lib_fsw.send_trigger()

#leave just one RG in service
wait_trigger()
avaSISURG = lib_RG.get_all_available_RG_instances('SISU')
if 0 == len(avaSISURG):
    log("No available SISU RG, stop_tc_execution")
    stop_tc_execution()
else:
    selected_SISURG = lib_RG.select_nRG_fromRGList(avaSISURG, 1)[0].strip('/')
    selected_SISURG_forceSwitchover = lib_fsw.forceSwitchoverILRG(selected_SISURG)
    lib_fsw.lib_comm.lockList(selected_SISURG_forceSwitchover.otherRGInstances)
send_trigger()

#do switchover
wait_trigger()
if not selected_SISURG_forceSwitchover.doSwitchover():
        lib_fsw.lib_comm.unlockList(selected_SISURG_forceSwitchover.otherRGInstances)
        stop_tc_execution()
send_trigger()

#unlock all RG
wait_trigger()
lib_fsw.lib_comm.unlockList(selected_SISURG_forceSwitchover.otherRGInstances)
send_trigger()
