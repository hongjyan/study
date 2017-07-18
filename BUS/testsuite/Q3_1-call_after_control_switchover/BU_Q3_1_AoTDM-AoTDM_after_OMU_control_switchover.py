#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.17
# 2013.1.24 SSH RG should be used instead of OMU in UI5.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import re
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_RG = boaimport('BU_RG_lib')

if g_debug:
    def log(str):
        return lib_RG.log(str)
 
    def cli(str):
        return lib_RG.cli(str)
 
    def stop_tc_execution():
        return lib_RG.stop_tc_execution()
 
    def wait_trigger():
        return lib_RG.wait_trigger()
 
    def send_trigger():
         return lib_RG.send_trigger()

if re.search("UI50", cli("fsswcli -c")): 
    rg = '/SSH'
else:
    rg = '/MGW_OMURG'

wait_trigger()  #First line in BOA
lib_RG.controlSwitchover_RG(rg)
send_trigger()

wait_trigger()
lib_RG.controlSwitchover_RG(rg)  #switch back /MGW_OMURG
send_trigger()
