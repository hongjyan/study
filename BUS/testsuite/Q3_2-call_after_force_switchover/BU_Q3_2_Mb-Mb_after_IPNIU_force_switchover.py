#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.24. change hard code, select one IPNIURG from system config file.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import random
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_fsw = boaimport('BU_FU_forceSwitchover_lib')
lib = boaimport('BU_BUS_Lib')

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
IPNIURGList = []
filename = lib.get_paraFileName_for_different_cluster()
for i in lib.get_line("IPNIURG", filename).strip().split("\n"):
    IPNIURGList.append(i.split('/')[1].split(',')[0])
random.shuffle(IPNIURGList)
selected_IPNIURG = IPNIURGList[0]
selected_IPNIURG_forceSwitchover = lib_fsw.forceSwitchoverILRG(selected_IPNIURG)
lib_fsw.lib_comm.lockList(selected_IPNIURG_forceSwitchover.otherRGInstances)
send_trigger()

#do switchover
wait_trigger()
if not selected_IPNIURG_forceSwitchover.doSwitchover():
        lib_fsw.lib_comm.unlockList(selected_IPNIURG_forceSwitchover.otherRGInstances)
        stop_tc_execution()
send_trigger()

#unlock all RG
wait_trigger()
lib_fsw.lib_comm.unlockList(selected_IPNIURG_forceSwitchover.otherRGInstances)
send_trigger()
