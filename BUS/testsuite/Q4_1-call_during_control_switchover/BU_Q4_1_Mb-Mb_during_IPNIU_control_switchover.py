#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.24. change hard code, select one IPNIU RG from config file.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import random
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_sw = boaimport('BU_FU_switchover_lib')
lib = boaimport('BU_BUS_Lib')

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

#Lock other IPNIU RG instances, make just MGW_IPNIURG-0 in service
wait_trigger()
IPNIURGList = []
filename = lib.get_paraFileName_for_different_cluster()
for i in lib.get_line("IPNIURG", filename).strip().split("\n"):
    IPNIURGList.append(i.split('/')[1].split(',')[0])
random.shuffle(IPNIURGList)
selected_IPNIURG = IPNIURGList[0]
selected_IPNIURG_switchover = lib_sw.switchoverILRG(selected_IPNIURG)
lib_sw.lib_comm.lockList(selected_IPNIURG_switchover.otherRGInstances) 
send_trigger()

wait_trigger()
#do first switchover
if not selected_IPNIURG_switchover.doSwitchover():   
        stop_tc_execution()        
send_trigger()

#do switchover one more time
wait_trigger()
if not selected_IPNIURG_switchover.doSwitchover():
        stop_tc_execution()
send_trigger()

#unlock other IPNIU RG instances
wait_trigger()
lib_sw.lib_comm.unlockList(selected_IPNIURG_switchover.otherRGInstances)
send_trigger()

