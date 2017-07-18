#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.23. change hard code, select one DSPM RG from system.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_RG = boaimport('BU_RG_lib')
lib_re = boaimport('BU_FU_restart_lib')

if g_debug:
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

#Lock other DSPM RG instances, make just MGW_DSPMRG in service
wait_trigger()
avaDSPMRG = lib_RG.get_all_available_RG_instances('DSPM')
if 0 == len(avaDSPMRG):
    log("No available DSPM RG, stop_tc_execution")
    stop_tc_execution()
else:
    selected_DSPMRG = lib_RG.select_nRG_fromRGList(avaDSPMRG, 1)[0].strip('/')
    DSPMRG_restart = lib_re.restartILRG(selected_DSPMRG)
    lib_re.lib_comm.lockList(DSPMRG_restart.otherRGInstances) 
send_trigger()

#do restart
wait_trigger()
if not DSPMRG_restart.doRestart():   
        stop_tc_execution()        
send_trigger()


#unlock other DSPM RG instances
wait_trigger()
lib_re.lib_comm.unlockList(DSPMRG_restart.otherRGInstances)
send_trigger()
