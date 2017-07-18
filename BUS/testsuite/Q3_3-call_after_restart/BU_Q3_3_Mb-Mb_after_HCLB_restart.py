#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

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

#Lock other HCLB RG instances, make just MGW_HCLBRG-0 in service
wait_trigger()
HCLBRG0_restart = lib_re.restartILRG('MGW_HCLBRG-0')
lib_re.lib_comm.lockList(HCLBRG0_restart.otherRGInstances) 
send_trigger()

#do restart
wait_trigger()
if not HCLBRG0_restart.doRestart():   
        stop_tc_execution()        
send_trigger()


#unlock other HCLB RG instances
wait_trigger()
lib_re.lib_comm.unlockList(HCLBRG0_restart.otherRGInstances)
send_trigger()

