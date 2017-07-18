#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.9.19
#=====================================================================
#!/usr/bin/python
import re
import os
import sys

g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *

lib_comm=boaimport('BU_common_lib')


if g_debug:
        def log(str):
                return lib_comm.log(str)

        def cli(str):
                return lib_comm.cli(str)

        def stop_tc_execution():
                return lib_comm.stop_tc_execution()

        def wait_trigger():
                return lib_comm.wait_trigger()

        def send_trigger():
                return lib_comm.send_trigger()

def bus_cli(str):
    if g_debug:
        out = os.popen(str + " 2>&1").read()
        sys.stdout.write('\n'+str+ '\n' + out)
        return out
    else:
        return cli(str)

def bus_fsclish(str):
    if g_debug:
        cmd = 'fsclish -c "' + str + '"'
        out = os.popen(cmd + " 2>&1").read()
        sys.stdout.write('\n'+cmd + '\n' + out)
        return out
    else:
        return fsclish(str)



# get node name via SS7 link index
def get_nodeName_from_tdmLink(linkid):
    pcm = -1
    cmd = "fsclish -c \"" + "show signaling ss7 link id " + str(linkid) + "\"|grep PCM"
    log(cmd)
    output = cli(cmd)
    log(output)
    pcm = output.split(':')[1].strip()
    return get_nodeName_from_PCMID(pcm)

# get node name via PCM ID
def get_nodeName_from_PCMID(pcm):
    letgr = -1
    chassis = -1
    piu = -1
    amc = -1
    peer_chassis = -1
    peer_piu = -1
    peer_amc = -1
    cmd = "fsclish -c \"show tdm pdh letgr\""
    log(cmd)
    output = cli(cmd)
    log(output)
    for l in output.splitlines()[3:]:
	w = l.split()
	if int(w[4])<=int(pcm) and int(w[5])>=int(pcm):
	    letgr = w[0]
	    break

    cmd2 = "fsclish -c \"show tdm sdh hierarchy set-index " + letgr + "\""
    log(cmd2)
    output = cli(cmd2)
    log(output)
    s = output.split('peer')
    for l in s[0].splitlines():
	if re.search('chassis index', l):
	    chassis = l.split(':')[1].strip()
	if re.search('piu index', l):
	    piu = l.split(':')[1].strip()
	if re.search('amcslot index', l):
	    amc = l.split(':')[1].strip()
    for l in s[1].splitlines():
        if re.search('chassis index', l): 
            peer_chassis = l.split(':')[1].strip()
        if re.search('piu index', l): 
            peer_piu = l.split(':')[1].strip()
        if re.search('amcslot index', l): 
            peer_amc = l.split(':')[1].strip()
    
    node = lib_comm.change_amc_to_node(chassis, piu, amc)
    peer_node = lib_comm.change_amc_to_node(peer_chassis, peer_piu, peer_amc)
    return (node, peer_node)
    
def get_setIndexOfCircuit(ncgr):
    pcms = bus_cli("fsclish -c \"show tdm circuitgroup crct ncgr %s\"|grep -v -i \"Executing\"|awk '/[0-9]-[0-9]/{split($1,a,\"-\");print a[1]}'|sort|uniq" %(ncgr)).split()
   
    letgrs = [] #letgrs's element is list also:[letgr_index, first_et, last_et]
    sets = []   #set's element is set index
    for i in bus_cli("fsclish -c \"show tdm pdh letgr\"|grep -v -i \"Executing\"|awk '/[0-9]/{print $1,$5,$6}'").splitlines():
        letgrs.append(i.split())
    
    for pcm in pcms:
        for letgr in letgrs:
            if  int(pcm) >= int(letgr[1]) and int(pcm) <= int(letgr[2]):
                sets.append(letgr[0])
                continue

    return sets
         
    
    
	    
	 
if __name__ == '__main__':
    print get_nodeName_from_tdmLink(11)
    print get_setIndexOfCircuit('sgwrm_circuit')
