#!/usr/bin/python
import sys  # not used in BOA
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

wait_trigger()
RG = '/Directory'
#do first switchover
lib_RG.controlSwitchover_RG(RG)
send_trigger()

#do switchover one more time
wait_trigger()
lib_RG.controlSwitchover_RG(RG)
send_trigger()
