#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.11
#2013.1.8 PCM ID is gotten from config file by YHJ
#=====================================================================
#!/usr/bin/python
import re
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib = boaimport('BU_BUS_Lib')
lib_re = boaimport('BU_FU_restart_lib')
lib_TDM = boaimport('BU_TDM_lib')
lib_MOState = boaimport('BU_MOState_lib')

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

#find target TDMSNIUP RG via PCM index
wait_trigger()
TDMMGURG = ''
filename = lib.get_paraFileName_for_different_cluster()
TDMPCM_line = lib.get_line("CRCT_PSTN", filename)
PCMID = int(TDMPCM_line.split(",")[1].strip().rstrip())

node = lib_TDM.get_nodeName_from_PCMID(PCMID)
RGs = lib_TDM.lib_comm.get_node_RG(node[0])
for RG in RGs:
    if re.search('TDMMGU', RG):
        TDMMGURG = RG
        break
TDMMGURG_restart = lib_re.restartILRG(TDMMGURG.strip('/'))
#do restart

TDMMGURG_restart.doRestart()

RU0 = "%s/MGW_TDMMGUFU-*" %(node[0])
RU1 = "%s/MGW_TDMMGUFU-*" %(node[1])
RU0MOState = lib_MOState.class_MOState(RU0)
RU1MOState = lib_MOState.class_MOState(RU1)
if not (RU0MOState.MO_is_OK()[0] or RU1MOState.MO_is_OK()[0]):
    stop_tc_execution()
        
send_trigger()
