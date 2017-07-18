#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

#lib=boaimport('BU_BUS_Lib')

g_debug = True
if g_debug:
    from boa_simulate import *

lib_sw = boaimport('BU_FU_switchover_lib')

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


#make a TDM call after ahub-8
wait_trigger()
cmd = 'hwcli -r AHUB3-A-1-8'
cli(cmd)
sleep(300)
send_trigger()

#make a TDM call after ahub-9 reset
wait_trigger()
cmd = 'hwcli -r AHUB3-A-1-9'
cli(cmd)
sleep(300)
send_trigger()

#make a TDM call after ahub-8/9 reset
wait_trigger()
cmd = 'hwcli -r AHUB3-A-1-8'
cli(cmd)
cmd = 'hwcli -r AHUB3-A-1-9'
cli(cmd)
sleep(400)
send_trigger()
