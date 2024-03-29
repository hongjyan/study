#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_RG = boaimport('BU_RG_lib')

if g_debug:
    def log(str):
        return lib_RG.log(str)

    def cli(str):
        return lib_RG.cli(str)

    def stop_tc_execution():
        return lib_RG.stop_tc_execution()

    def wait_trigger():
        return lib_RG.wait_trigger()

    def send_trigger():
         return lib_RG.send_trigger()

wait_trigger()
avaDSPMTCU2RG = lib_RG.get_all_available_RG_instances('DSPM')
twoDSPMTCU2RG = lib_RG.select_nRG_fromRGList(avaDSPMTCU2RG,2) #select two RG instances from ava RGs randomly
#lock all other RG instances
other_avaDSPMTCU2RG = avaDSPMTCU2RG[2:len(avaDSPMTCU2RG)]
lib_RG.lib_MO.lib_comm.lockList(other_avaDSPMTCU2RG)
#
#lock the second one of twoDSPMTCU2RG,leave the first one in service
lib_RG.lib_MO.lib_comm.lock(twoDSPMTCU2RG[1])
#
send_trigger()

#execute call,then do handover
wait_trigger()
lib_RG.lib_MO.lib_comm.unlock(twoDSPMTCU2RG[1])
lib_RG.lib_MO.lib_comm.lock(twoDSPMTCU2RG[0])
send_trigger()

#execute call, then unlock all RG instances,return environment to origin
wait_trigger()
lib_RG.lib_MO.lib_comm.unlockList(other_avaDSPMTCU2RG)
lib_RG.lib_MO.lib_comm.unlock(twoDSPMTCU2RG[0])
send_trigger()
 

