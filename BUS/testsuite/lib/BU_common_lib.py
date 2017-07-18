#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.27
# 2013.1.9 re-define cli(str) to cli(str, *other) to support more opition for cli command by yhj
# 2013.1.24 re-define cli(str, *other) to cli(str, wait_prompt=True)
#=====================================================================
import re
import sys
import time
import os


g_debug = True   #The switch which simulate funcation which were defined by BOA
#g_debug = False

if g_debug:
        VERDICT_FAIL = 0
        VERDICT_PASS = 1
        def verdict(v):   #simulate funcation of BOA, Verdict case pass or fail
                if v == VERDICT_FAIL:
                        print 'FAIL'
                else:
                        print 'PASS'
 
        def cli(str, wait_prompt=True):     #simulate funcation of BOA, issue command:str on MGW
                out = os.popen(str).read()
                sys.stdout.write('\n'+str+ '\n' + out) 
                return out

        def log(str):     #simulate funcation of BOA, log anything what you want 
                print str


        #simulate funcation of BOA,BOA will run corresponding code between wait_trigger
		#and send_trigger when there is a test procedure named TpAutomationTrigger in 
		#test case,More question, please ask yikun.huang@nsn.com
        def wait_trigger(): 
                print "end call execution"
                print "start script execution"
                print '.'
                print '.'
                print '.'

        def send_trigger():
                print "end script execution"
                print "start call execution"
                print '*'
                print '*'
                print '*'

        def stop_tc_execution():       #simulate funcation of BOA, stop test case execution, if there is some critical error happened.
                verdict(VERDICT_FAIL)
                sys.exit(-1)

def report_pass():   #verdict test case passed
    verdict(VERDICT_PASS)

def report_fail():   #verdict test case fail
    verdict(VERDICT_FAIL)

def bus_cli(str):
    if g_debug:
        out = os.popen(str + " 2>&1").read()
#        sys.stdout.write('\n'+str+ '\n' + out)
        return out
    else:
        return cli(str)

def bus_fsclish(str):
    if g_debug:
        cmd = 'fsclish -c "' + str + '"'
        out = os.popen(cmd + " 2>&1").read()
#        sys.stdout.write('\n'+cmd + '\n' + out)
        return out
    else:
        return fsclish(str)


def uniqueList(l):   #remove duplicate element in list, return unique list.
    return list(set(l))

def lock(s):        #lock MO
    if not s:
        pass
    else:
        cmd = 'hascli -lnE ' + s
        log(cmd)
        output = cli(cmd)
        log(output)


def unlock(s):     #unlock MO
    if not s:
        pass
    else:
        cmd = 'hascli -unE ' + s
        log(cmd)
        output = cli(cmd)
        log(output)

def lockList(l):    #lock multi MO
    for s in l:
        lock(s)

def unlockList(l): #unlock multi MO
    for s in l:
        unlock(s)

def change_piu_to_node(chassis, piu):  #get node name from chassis index and slot index
    cmd = 'hwcli -o off -e ^[^d]|grep chassis-' + str(chassis) + '|grep piu-' + str(piu)
    log(cmd)
    output = cli(cmd)
    node = '/' + output[0:output.find(':')].rstrip()
    return node

def change_node_to_piu(node):         #return a tuple whose first element is chassis index and second element is slot index from node name.  
    cmd = 'hwcli -e ' + node.strip('/')
    log(cmd)
    output = cli(cmd)
    log(output)
    chassis = output.split('/')[2].split('-')[1]
    piu = output.split('/')[3].split('-')[1]
    return (chassis, piu)

def change_amc_to_node(chassis, piu, amc):  #get node name from chassis index and slot index and amc index
    cmd = 'hwcli -o off -e ^[^d]|grep chassis-' + str(chassis) + '|grep piu-' + str(piu) \
	   + '|grep AMC-B' + str(amc)
    log(cmd)
    output = cli(cmd)
    node = '/' + output[0:output.find(':')].rstrip()
    return node

def change_node_to_amc(node):      #return a tuple whose first element is chassis index and second element is slot index and third element 
                                   # is amc index from node name.
    cmd = 'hwcli -e ' + node.strip('/')
    log(cmd)
    output = cli(cmd)
    log(output)
    chassis = output.split('/')[2].split('-')[1]
    piu = output.split('/')[3].split('-')[1]
    amc = output.split('/')[4].split('-')[1].strip('B')
    return (chassis, piu, amc)

def get_node_RG(nd):  # return a list which consist of all RGs name whose RU runs on this node.
    rgs = []
    cmd = 'hascli --children ' + nd + ' | awk -F/ \'NF<=3\' | \
xargs hascli --parent | sort | uniq | grep -v ' + nd
    log(cmd)
    output = cli(cmd)
    log(output)
    ls=output.split('\n')
    for l in ls:
        rg = '/' + l.rstrip().lstrip('/')
        if rg == '':
            continue
        rgs.append(rg) 
    #uniq the rgs
    rgs = list(set(rgs))
    return rgs

if __name__ == "__main__":
    print change_piu_to_node(1,4)
    print change_node_to_piu("/TCU-1")
    print change_amc_to_node(1,7,3)
    print change_node_to_amc("/TDMNIP-4")
    print get_node_RG("/TDMNIP-4")
