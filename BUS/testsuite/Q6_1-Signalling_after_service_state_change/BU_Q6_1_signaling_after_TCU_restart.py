#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
# Modified: 2012.12.27
#=====================================================================
#/usr/bin/python
import sys
import time
sys.path.append("../lib")
g_debug = True
if g_debug:
    from boa_simulate import *

lib_node = boaimport('BU_node_lib')
lib_MO = boaimport('BU_MOState_lib')

if g_debug:
    def log(str):
        return lib_node.log(str)

    def cli(str):
        return lib_node.cli(str)

    def stop_tc_execution():
        return lib_node.stop_tc_execution()

    def wait_trigger():
        return lib_node.wait_trigger()

    def send_trigger():
         return lib_node.send_trigger()

wait_trigger()
enabledTCU = lib_node.get_enabled_node_instances('TCU')
oneTCU = lib_node.select_nNode_fromNodeList(enabledTCU,1)
otherTCU = enabledTCU[1:]  #select one enabled TCU
lib_node.lib_MO.lib_comm.lockList(otherTCU)  #lock the other enabled TCU
send_trigger()

#restart TCU node
wait_trigger()
cmd = 'hascli -rn ' + oneTCU[0]
log(cmd)
output = cli(cmd)
log(output)
send_trigger()

wait_trigger()
if g_debug:
    time.sleep(300)
cur_TCUState = lib_MO.class_MOState(oneTCU[0])
if not cur_TCUState.MO_is_OK()[0]:
    log("TCU haven't started up in waiting seconds, stop test case execution")
    lib_node.lib_MO.lib_comm.unlockList(otherTCU)
    stop_tc_execution()
send_trigger()

#unlock other TCU nodes
wait_trigger()
lib_node.lib_MO.lib_comm.unlockList(otherTCU)
send_trigger()
