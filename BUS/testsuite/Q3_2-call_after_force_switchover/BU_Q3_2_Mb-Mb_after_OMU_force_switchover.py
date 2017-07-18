#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.17
# 2013.1.24 use SSH to do OMU switchover in UI50
# 2013.1.24 USE BU_RG_lib to do force switchover
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_RG = boaimport('BU_RG_lib')

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

wait_trigger()
if re.search("UI50", cli("fsswcli -c")):
    rg = '/SSH'
else:
    rg = '/MGW_OMURG'

lib_RG.forceSwitchover_RG(rg)
send_trigger()
