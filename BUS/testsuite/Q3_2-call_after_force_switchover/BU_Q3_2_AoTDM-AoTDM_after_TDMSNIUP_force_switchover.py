#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.8 PCM ID is gotten from config file by YHJ
# 2013.7.3 show crct state after do switchover by YHJ
#=====================================================================
#!/usr/bin/python
import re
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *


lib = boaimport('BU_BUS_Lib')
lib_fsw = boaimport('BU_FU_forceSwitchover_lib')
lib_TDM = boaimport('BU_TDM_lib')

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

#find target TDMSNIUP RG via PCM index
wait_trigger()
TDMSNIUPRG = ''
filename = lib.get_paraFileName_for_different_cluster()
TDMPCM_line = lib.get_line("CRCT_PSTN", filename)
PCMID = int(TDMPCM_line.split(",")[1].strip().rstrip())

node = lib_TDM.get_nodeName_from_PCMID(PCMID) 
RGs = lib_TDM.lib_comm.get_node_RG(node[0])
for RG in RGs:
    if re.search('TDMSNIUP', RG):
        TDMSNIUPRG = RG
        break
TDMSNIUPRG2_switchover = lib_fsw.forceSwitchoverILRG(TDMSNIUPRG.strip('/'))
#do switchover
if not TDMSNIUPRG2_switchover.doSwitchover():
	stop_tc_execution()

lib.bus_fsclish("show tdm crct-state crct-pcm %s crct-tsl 1-31" %(PCMID))
send_trigger()
