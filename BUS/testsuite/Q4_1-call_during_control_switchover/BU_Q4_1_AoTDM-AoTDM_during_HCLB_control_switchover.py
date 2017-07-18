#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.2.28. unlock already locked HCLB RG if do switchover failed.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_sw = boaimport('BU_FU_switchover_lib')

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

#Lock other HCLB RG instances, make just MGW_HCLBRG-0 in service
wait_trigger()
HCLBRG0_switchover = lib_sw.switchoverILRG('MGW_HCLBRG-0')
lib_sw.lib_comm.lockList(HCLBRG0_switchover.otherRGInstances) 
send_trigger()

wait_trigger()
#do first switchover
if not HCLBRG0_switchover.doSwitchover():
        lib_sw.lib_comm.unlockList(HCLBRG0_switchover.otherRGInstances)
        stop_tc_execution()        
send_trigger()

#do switchover one more time
wait_trigger()
if not HCLBRG0_switchover.doSwitchover():
        lib_sw.lib_comm.unlockList(HCLBRG0_switchover.otherRGInstances)
        stop_tc_execution()
send_trigger()

#unlock other HCLB RG instances
wait_trigger()
lib_sw.lib_comm.unlockList(HCLBRG0_switchover.otherRGInstances)
send_trigger()

