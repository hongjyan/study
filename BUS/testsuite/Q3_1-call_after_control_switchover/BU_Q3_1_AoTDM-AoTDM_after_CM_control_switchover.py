#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA


g_debug = True

if g_debug:
    from boa_simulate import *

lib_fsw = boaimport('BU_FU_forceSwitchover_lib')

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
        
CMRG_forceSwitchover = lib_fsw.forceSwitchoverILRG('MGW_CMRG')
if not CMRG_forceSwitchover.doSwitchover():
        stop_tc_execution()
send_trigger()
