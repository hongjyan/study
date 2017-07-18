#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_fsw = boaimport('BU_FU_forceSwitchover_lib')

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
HCLBRG0_forceSwitchover = lib_fsw.forceSwitchoverILRG('MGW_HCLBRG-0')
lib_fsw.lib_comm.lockList(HCLBRG0_forceSwitchover.otherRGInstances)
send_trigger()

#do switchover
wait_trigger()
if not HCLBRG0_forceSwitchover.doSwitchover():
        lib_fsw.lib_comm.unlockList(HCLBRG0_forceSwitchover.otherRGInstances)
        stop_tc_execution()
send_trigger()

#unlock all RG
wait_trigger()
lib_fsw.lib_comm.unlockList(HCLBRG0_forceSwitchover.otherRGInstances)
send_trigger()
