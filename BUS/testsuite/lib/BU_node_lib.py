#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.9.17
#=====================================================================
#!/usr/bin/python
import random
g_debug = True
if g_debug:
    from boa_simulate import *

lib_MO = boaimport('BU_MOState_lib') #import BU_MOState_lib

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

#get all node instances. Eg: get_all_node_instances('TCU'), 
#then return value is a list which is consist of all TCU 
#nodes name like ['TCU-0', 'TCU-1',.....]
def get_all_node_instances(nodeType): 
    nodes = []
    cmd = 'hascli --bare node |grep -i ' + nodeType
    log(cmd)
    output = cli(cmd)
    log(output)
    for l in output.splitlines():
	nodes.append(l)
    return nodes

#get all enalbed fixed type node intances.    	
def get_enabled_node_instances(nodeType):
    enableNodes = []
    nodes = get_all_node_instances(nodeType)
    for n in nodes:
	if lib_MO.class_MOState(n).MO_is_OK()[0]:
	    enableNodes.append(n)
    return enableNodes

#select n element from a list randomly
def select_nNode_fromNodeList(NodeList,n):
        if len(NodeList) < n:
                log('Amount of Nodes is less than n, exit')
                stop_tc_execution()
        random.shuffle(NodeList) #shuffle RGList to select 2 random RGs
        return  NodeList[0:n]

if __name__ == "__main__":
    print get_enabled_node_instances("TCU")
    
