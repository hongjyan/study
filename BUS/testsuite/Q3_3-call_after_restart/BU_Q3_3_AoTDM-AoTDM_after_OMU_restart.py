#!/usr/bin/python
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_re = boaimport('BU_FU_restart_lib')

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

wait_trigger()
OMURG_restart = lib_re.restartILRG('MGW_OMURG')
if not OMURG_restart.doRestart():  #do restart 
        stop_tc_execution()        
send_trigger()
