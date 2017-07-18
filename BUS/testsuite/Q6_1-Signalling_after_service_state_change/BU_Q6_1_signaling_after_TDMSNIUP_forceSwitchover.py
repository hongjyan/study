#/usr/bin/python
import re
import sys
sys.path.append("../lib")
g_debug = True
if g_debug:
    from boa_simulate import *

lib_TDM = boaimport('BU_TDM_lib')
lib_fsw = boaimport('BU_FU_forceSwitchover_lib')

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
RGs = []
TDMSNIUPRG = ''

node = lib_TDM.get_nodeName_from_tdmLink(11) 
RGs = lib_TDM.lib_comm.get_node_RG(node[0])
for RG in RGs:
    if re.search('TDMSNIUP', RG):
	TDMSNIUPRG = RG
	break
    
TDMSNIUPRG_forceSwitchover = lib_fsw.forceSwitchoverILRG(TDMSNIUPRG.strip('/'))
if not TDMSNIUPRG_forceSwitchover.doSwitchover():
        stop_tc_execution()
send_trigger()
