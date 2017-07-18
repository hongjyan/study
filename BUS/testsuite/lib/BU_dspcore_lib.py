#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.27
#=====================================================================
#!/usr/bin/python
import random
g_debug = True
if g_debug:
    from boa_simulate import *

lib_MO = boaimport('BU_MOState_lib')
lib_node = boaimport('BU_node_lib')

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

#check if specific dsp core starts up or not.
def dspcore_is_nofaulty(chassis, piu, dspfarm, dsp, core):
    node = lib_MO.lib_comm.change_piu_to_node(chassis, piu)
    if not lib_MO.class_MOState(node).MO_is_OK()[0]:
        log("TCU node isn't OK,exit")
        return -1
    cmd = 'hascli -s ' + node + '|grep FAULTY_DSPS'
    log(cmd)
    output = cli(cmd)
    log(output)
    FAULTY_DSPS = output.rstrip().split('=')[1].split('-')
    dsp_index = dspfarm*10 + dsp
    core_state = FAULTY_DSPS[dsp_index][core]
    if 'X' == core_state: #if core was locked or unknown
        log("core was in locked state or unknown")
        return False
    elif not int(core_state):
        return True
    else:
        return False


def dspcore_is_sync(chassis, piu, dspfarm, dsp, core):
    node = lib_MO.lib_comm.change_piu_to_node(chassis, piu)
    if not lib_MO.class_MOState(node).MO_is_OK()[0]:
        log("TCU node isn't OK,exit")
        return -1
    cmd = 'hascli -s ' + node + '|grep OOSYNCH_DSPS'
    log(cmd)
    output = cli(cmd)
    log(output)
    OOSYNCH_DSPS = output.rstrip().split('=')[1].split('-')
    dsp_index = dspfarm*10 + dsp
    core_state = OOSYNCH_DSPS[dsp_index][core]
    if not int(core_state):
        return True
    else:
        return False

def dspcore_is_ok(chassis, piu, dspfarm, dsp, core):
    if dspcore_is_nofaulty(chassis, piu, dspfarm, dsp, core)\
        and dspcore_is_sync(chassis, piu, dspfarm, dsp, core):
        return True
    else:
        return False


#get all TCU whose last DSP which is used for signalling starts up or not.
def get_signallingOK_cores():
    enabledTCU = lib_node.get_enabled_node_instances("TCU")
    OK_cores = []
    for nd in enabledTCU:
        piu = lib_MO.lib_comm.change_node_to_piu(nd)
        if dspcore_is_nofaulty(piu[0], piu[1], 1, 9, 5): #actually, 1,9,0-4 can be used by signalling also. Here, select the last one just.
            core = (piu[0], piu[1], 1, 9, 5)
            OK_cores.append(core)
    return OK_cores

#do operation on the last dsp device of specific TCU node. Operation includes lock, unlock, reset.
def change_sigcores_state(oper, chassis, piu):
    cmd = "fsclish -c \"set dsp has-state " + oper + " device dsp-" + chassis + "-" + piu \
            + "-19" + "\""
    log(cmd)
    output = cli(cmd)
    log(output)
     


    

if __name__ == "__main__":
    print dspcore_is_nofaulty(1, 4, 0, 5, 5)
    print dspcore_is_sync(1, 4, 0, 5, 5)  
    print dspcore_is_ok(1, 4, 0, 5, 5)
    print get_signallingOK_cores() 
