#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.27
#=====================================================================
#!/usr/bin/python
import time
import sys  # not used in BOA
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_MO = boaimport('BU_MOState_lib')

if g_debug:
    def log(str):
        return lib_MO.log(str)

    def cli(str):
        return lib_MO.cli(str)

    def stop_tc_execution():
        return lib_MO.stop_tc_execution()

    def wait_trigger():
        return lib_MO.wait_trigger()

    def send_trigger():
         return lib_MO.send_trigger()

wait_trigger()
pre_SS7State = lib_MO.class_MOState('/SS7SGU')
if not pre_SS7State.MO_is_OK()[0]:
	log("SS7SGU haven't start up, stop test case execution")
	stop_tc_execution()

cmd = 'hascli -ln /SS7SGU'
log(cmd)
output = cli(cmd)
log(output)
cmd = 'hascli -un /SS7SGU'
log(cmd)
output = cli(cmd)
log(output)

time.sleep(5) # wait 5s for SS7SGU start up 
cur_SS7State = lib_MO.class_MOState('/SS7SGU')
if not cur_SS7State.MO_is_OK()[0]:
        log("SS7SGU haven't start up after lock/unlock, stop test case execution")
        stop_tc_execution()
send_trigger()
