#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.23. change hard code, select one DSPM RG from system.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import random
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib = boaimport('BU_BUS_Lib')
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

#Lock other RG instances, make just one RG instances in service
wait_trigger()

IPNIURGList = []
filename = lib.get_paraFileName_for_different_cluster()
for i in lib.get_line("IPNIURG", filename).strip().split("\n"):
    IPNIURGList.append(i.split('/')[1].split(',')[0])
random.shuffle(IPNIURGList)
selected_IPNIURG = IPNIURGList[0]
selected_IPNIURG_restart = lib_re.restartILRG(selected_IPNIURG)
lib_re.lib_comm.lockList(selected_IPNIURG_restart.otherRGInstances) 
send_trigger()

#do restart
wait_trigger()
if not selected_IPNIURG_restart.doRestart():   
        stop_tc_execution()        
send_trigger()


#unlock other  RG instances
wait_trigger()
lib_re.lib_comm.unlockList(selected_IPNIURG_restart.otherRGInstances)
send_trigger()
