#!/usr/bin/python
import re
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_sw = boaimport('BU_FU_switchover_lib')
lib_TDM = boaimport('BU_TDM_lib')

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

#find target TDMSNIUP RG via PCM index
wait_trigger()
TDMSNIUPRG = ''
node = lib_TDM.get_nodeName_from_PCMID(1) 
RGs = lib_TDM.lib_comm.get_node_RG(node[0])
for RG in RGs:
    if re.search('TDMSNIUP', RG):
        TDMSNIUPRG = RG
        break
TDMSNIUPRG2_switchover = lib_sw.switchoverILRG(TDMSNIUPRG.strip('/'))
#do switchover
if not TDMSNIUPRG2_switchover.doSwitchover():
	stop_tc_execution()
send_trigger()
