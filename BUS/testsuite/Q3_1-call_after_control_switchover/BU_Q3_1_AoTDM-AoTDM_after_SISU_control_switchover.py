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

lib_sw = boaimport('BU_FU_switchover_lib')
lib_RG = boaimport('BU_RG_lib')

if g_debug:
    def log(str):
        return lib_sw.log(str)
 
    def cli(str):
        return lib_sw.cli(str)
 
    def stop_tc_execution():
        return lib_sw.stop_tc_execution()
 
    def wait_trigger():
        return lib_sw.wait_trigger()
 
    def send_trigger():
         return lib_sw.send_trigger()

#leave just one RG instance in service
wait_trigger()
avaSISURG = lib_RG.get_all_available_RG_instances('SISU')
if 0 == len(avaSISURG):
    log("No available SISU RG, stop_tc_execution")
    stop_tc_execution()
else:
    selected_SISURG = lib_RG.select_nRG_fromRGList(avaSISURG, 1)[0].strip('/')
    selected_SISURG_switchover = lib_sw.switchoverILRG(selected_SISURG)
    lib_sw.lib_comm.lockList(selected_SISURG_switchover.otherRGInstances)
send_trigger()

#do switchover
wait_trigger()
if not selected_SISURG_switchover.doSwitchover():
	lib_sw.lib_comm.unlockList(selected_SISURG_switchover.otherRGInstances)
	stop_tc_execution()
send_trigger()

#unlock all RG instances
wait_trigger()
lib_sw.lib_comm.unlockList(selected_SISURG_switchover.otherRGInstances)
send_trigger()
