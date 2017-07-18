#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.27
# 2013.7.9 In current BU environments, no ADX was used, and hit can't do
# APS, so do workaround to select TDM node whose index is less.
#=====================================================================
#/usr/bin/python
import re
import sys
sys.path.append("../lib")
g_debug = True
if g_debug:
    from boa_simulate import *

lib_TDM = boaimport('BU_TDM_lib')
lib_RG = boaimport('BU_RG_lib')
lib_MO = boaimport('BU_MOState_lib')

if g_debug:
    def log(str):
        return lib_TDM.log(str)

    def cli(str):
        return lib_TDM.cli(str)

    def stop_tc_execution():
        return lib_TDM.stop_tc_execution()

    def wait_trigger():
        return lib_TDM.wait_trigger()

    def send_trigger():
         return lib_TDM.send_trigger()

wait_trigger()
node = ()
activeNode = ''
standbyNode = ''
RGs = []
TDMSNIUPRG = ''
TDMSNIUPRU = []

node = lib_TDM.get_nodeName_from_tdmLink(11) 
RGs = lib_TDM.lib_comm.get_node_RG(node[0])
for RG in RGs:
    if re.search('TDMSNIUP', RG):
        TDMSNIUPRG = RG
        break
TDMSNIUPRU = lib_RG.get_RU(TDMSNIUPRG)


for u in TDMSNIUPRU:
    temp = lib_MO.class_MOState(u)
    if temp.MO_is_OK()[1] == 'ACTIVE':
        activeNode = '/' + u.split('/')[1]
    elif temp.MO_is_OK()[1] == 'HOTSTANDBY':
        standbyNode = '/' + u.split('/')[1]
    else:
        log('Unexpected error, exit')
        stop_tc_execution()

#if node with big index is active one, do switchover to make TDM node with small index take active role
if activeNode == node[1]:
    lib_RG.controlSwitchover_RG(TDMSNIUPRG)
    
lib_TDM.lib_comm.lock(node[1]) #lock TDM node with big index to prevent switchover
send_trigger()

#lock/unlock TDM node
wait_trigger()
cmd = 'hascli -rn ' + node[0]
log(cmd)
output = cli(cmd)
log(output)
send_trigger()

wait_trigger()
if g_debug:
    import time
    time.sleep(450) #wait 60s for TDM node start up

activeTDMNIP = lib_MO.class_MOState(node[0])
if not activeTDMNIP.MO_is_OK()[0]:
    log("TDMNIP node haven't started up, stop test case")
    cli('hascli -un %s') %(node[1])
    stop_tc_execution()
send_trigger()

#unlock TDM node with small index to recover environment
wait_trigger()
cmd = 'hascli -un ' + node[1]
log(cmd)
output = cli(cmd)
log(output)
send_trigger()
