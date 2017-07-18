#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.27
# 2013.7.9 using lock RU to replace lock node to trigger switchover.
# lock RU on APS active node won't make call failed. 
#=====================================================================
#/usr/bin/python
import re
import sys
import time
sys.path.append("../lib")
g_debug = True
if g_debug:
    from boa_simulate import *

lib_TDM = boaimport('BU_TDM_lib')
lib_MO = boaimport('BU_MOState_lib')
lib_RG = boaimport('BU_RG_lib')

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
activeRU = ''
standbyRU = ''

node = lib_TDM.get_nodeName_from_tdmLink(11) # Notice 11 is the signalling link ID used for 64K Signalling test case 
RGs = lib_TDM.lib_comm.get_node_RG(node[0])
for RG in RGs:
    if re.search('TDMSNIUP', RG):
        TDMSNIUPRG = RG
        break
TDMSNIUPRU = lib_RG.get_RU(TDMSNIUPRG)

for n in node:
    if not lib_MO.class_MOState(n).MO_is_OK()[0]:
        log("At least one node isn't OK, stop test case")
        stop_tc_execution()

for u in TDMSNIUPRU:
    temp = lib_MO.class_MOState(u)
    if not temp.MO_is_OK()[0]:
        log("At least one TDMSNIUP RU isn't OK, stop test case")
        stop_tc_execution()
    if temp.MO_is_OK()[1] == 'ACTIVE':
        activeRU = u
    elif temp.MO_is_OK()[1] == 'HOTSTANDBY':
        standbyRU = u
    else:
        log('Unexpected error, exit')
        stop_tc_execution()

#lock active TDMSNIUP RU to make RU level switchover
cmd = 'hascli -ln ' + activeRU
log(cmd)
output = cli(cmd)
log(output)
send_trigger()

#check new ACTIVE RU state
wait_trigger()
if g_debug:
    import time
    time.sleep(5)
new_activeTDMSNIUP = lib_MO.class_MOState(standbyRU)
if not (new_activeTDMSNIUP.MO_is_OK()[0] and 'ACTIVE' == new_activeTDMSNIUP.MO_is_OK()[1]):
    log("Pre standby TDMSNIUP haven't gone to ACTIVE state, stop test case")
    cli("hascli -un %s" %(activeRU))
    stop_tc_execution()
send_trigger()

#unlock pre_active TDMSNIUP RU to recover environment
wait_trigger()
cmd = 'hascli -un ' + activeRU
log(cmd)
output = cli(cmd)
log(output)
send_trigger()
