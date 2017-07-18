#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.23. change hard code, select one SISU RG from system.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_re = boaimport('BU_FU_restart_lib')
lib_RG = boaimport('BU_RG_lib')

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

#Lock other SISU RG instances, make just MGW_SISURG-0 in service
wait_trigger()
avaSISURG = lib_RG.get_all_available_RG_instances('SISU')
if 0 == len(avaSISURG):
    log("No available SISU RG, stop_tc_execution")
    stop_tc_execution()
else:
    selected_SISURG = lib_RG.select_nRG_fromRGList(avaSISURG, 1)[0].strip('/')
    SISURG_restart = lib_re.restartILRG(selected_SISURG)
    lib_re.lib_comm.lockList(SISURG_restart.otherRGInstances) 
send_trigger()

#do restart
wait_trigger()
if not SISURG_restart.doRestart():   
        stop_tc_execution()        
send_trigger()


#unlock other SISU RG instances
wait_trigger()
lib_re.lib_comm.unlockList(SISURG_restart.otherRGInstances)
send_trigger()

