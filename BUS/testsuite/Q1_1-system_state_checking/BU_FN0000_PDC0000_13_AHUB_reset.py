#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# 2013.5.14 created.
# BI&FI config check/VMGW multi-homing/SyncMgr functionality/HPI daemon functionality 
# will be check in this script
# 2013.5.21 strip leading/tailing whitespace for BI config.
#=====================================================================
#!/usr/bin/python
import sys  # not used in BOA
import time
import re
sys.path.append("../lib")  # not used in BOA

g_debug = True
if g_debug:
    from boa_simulate import *

lib_re = boaimport('BU_FU_restart_lib')

if g_debug:
    VERDICT_FAIL = 0 
    VERDICT_PASS = 1 
    def verdict(v):   #simulate funcation of BOA, Verdict case pass or fail
        if v == VERDICT_FAIL:
            print 'FAIL'
        else:
            print 'PASS'

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

def check_inputReference():
    cmd ="fsclish -c \"show mgw synchronization inputreference\""
    output = cli(cmd)
    for l in output.splitlines():
        if re.search("SET-", l) and re.search("YES", l): 
            return True
    return False 

def get_acar_clock_state():
    cmd = "fsclish -c \"show mgw synchronization clock-distribution\""
    output = cli(cmd)
    state = {}
    for l in output.splitlines():
        if re.search("lmp-",l):
            lmp_list = l.split()
            state[l[0]] = l[3]
    return state
    
def get_HW_inventory():
    cmd = "fshwicli -b"
    output = cli(cmd)
    return output

switch_list = ["Switch-1-8", "Switch-1-9"]

#check input reference
if check_inputReference():
    log("input reference OK before HUBs reset")
else:
    log("Attendtion, No input reference or input reference is not in locked.!!!!!!!!!")
    verdict(VERDICT_FAIL)


#get acar clock state before AHUB reset
clockStateBefore = get_acar_clock_state()


#get bi and fi configuraton before switch reset 
switch_config_before = []
for i in range(len(switch_list)):
    cmd1 = "ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no %s \"cat  /etc/fastpath/fpconfig0.txt|grep -v \'System Up Time\'|sed -e \'s/[ ]*$//g\'\"" %(switch_list[i])
    cmd2 = "ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no %s \"cat  /etc/fastpath/fpconfig1.txt|grep -v \'System Up Time\'\"" %(switch_list[i])
    switch_config_before.append(cli(cmd1))
    switch_config_before.append(cli(cmd2))

#Check multiHoming take effect when AHUB reset. multi-homing VMGW configuration should be done before this script run
#suppose primary control ip address is via fi0(this kind configuration applys to all BU environment currently),so this
#primary address is via AHUB8, primary address should be the address via fi1 after AHUB8 reset about 1~3 seconds later.
#And after about 3 minutes, AHUB8 start up again, this primary address should switch back via fi0.
#get control primary address, control second address, current control address.
cmd = "fsclish -c \"show vmgw mgw vid 0 mod 0\""
output = cli(cmd)
control_primary_address = re.search(re.compile("control primary IP address.*?:(.*)"), output).group(1)
control_secondary_address = re.search(re.compile("control secondary IP address.*?:(.*)"), output).group(1)
control_address = re.search(re.compile("control IP address.*?:(.*)"), output).group(1)

#get HW inventory
HWs_before = get_HW_inventory()

#attemp to restart AHUB in 3 try. Maybe need restart other shelf's switch in future.
i = 0
AHUB8_flag = False
AHUB9_flag = False
while i<3:
    out1 = cli("hwcli -r -n -o off AHUB3-A-1-8")
    if re.search("ok", out1):
        AHUB8_flag = True

        time.sleep(8) #sleep 8s to see if muliti-homing take effect or not
        log("sleep 8s for multi-homing take effect")

    
        #check multi-homing take effect or not after AHUB8 reset 2s later
        cmd = "fsclish -c \"show vmgw mgw vid 0 mod 0\""
        output = cli(cmd)
        new_control_address = re.search(re.compile("control IP address.*?:(.*)"), output).group(1)
        if new_control_address == control_primary_address: #if control address is still equal to control primary address
            log("attention, multi-homing does not take effect, control address didn't switch to \
            secondary control address!!!!!!!!!!!!!!!!!")
            verdict(VERDICT_FAIL) #will go to next test item even multi-homing failed but test cases will be vedict as fail
        else:
            log("multi-homing OK")



        log("sleep 2 minutes to wait shelfmanager switch stable")
        time.sleep(120)

        #check HW inventory is the same as before reset after AHUB8 reset 2 minutes later
        HWs_afterAHUB8 = get_HW_inventory()
        if cmp(HWs_before, HWs_afterAHUB8) != 0:
            log("Attention, HW inventory change after AHUB8 reset 2 minutes later!!!!!!!!!!!!!!!!")
            verdict(VERDICT_FAIL)
        else:
            log("HW inventory OK after AHUB8 reset 123s later")
        
        time.sleep(90)  # sleep 90s to let AHUB8 start up.
        log("sleep 90s(123+90) to wait AHUB8 start up")

        #check if control address switch back after AHUB start up again
        output = cli(cmd)
        new_control_address = re.search(re.compile("control IP address.*?:(.*)"), output).group(1)
        if not new_control_address == control_primary_address:
            log("attention, multi-homing does not take effect, control addrese didn't switch back!!!!!!!!!!!!!!!!!!")
            verdict(VERDICT_FAIL) #will go to next test item even multi-homing failed but test cases will be vedict as fail
        else:
            log("multi-homing switch back OK")
        
        #check input reference after AHUB8 reset 3 minutes 
        if check_inputReference():
            log("input reference OK after AHUB8 reset")
        else:
            log("Attendtion, No input reference or input reference is in locked state after AHUB8 reset!!!!!!!!!")
            verdict(VERDICT_FAIL)


        #check if acar clock will be locked after AHUB8 start up again
        clockStateAfterAHUB8 = get_acar_clock_state()
        if cmp(clockStateAfterAHUB8, clockStateBefore) != 0:
            log("Attention, clock in ACAR doesn't recover after AHUB8 reset 3 minutes later!!!!!!!!!!!!!")
            verdict(VERDICT_FAIL)
        else:
            log("clock distribution OK ")


    else:
        AHUB8_flag = False

    out2 = cli("hwcli -r -n  -o off AHUB3-A-1-9")
    if re.search("ok", out2): # if both AHUB8 and AHUB9 were reseted successfully, then break. 
        AHUB9_flag = True

        #check input reference after AHUB8 reset
        if check_inputReference():
            log("input reference OK after AHUB9 reset at once")
        else:
            log("Attendtion, No input reference or input reference is in locked state after AHUB9 reset at once!!!!!!!!!")
            verdict(VERDICT_FAIL)
        
        log("sleep 120s to wait shelfmanager switch stable")
        time.sleep(120)

        #check HW inventory is the same as before reset 120s later
        HWs_afterAHUB9 = get_HW_inventory()
        if cmp(HWs_before, HWs_afterAHUB9) != 0:
            log("Attention, HW inventory change after AHUB9 reset 120s later!!!!!!!!!!!!!!!!!!")
            verdict(VERDICT_FAIL)
        else:
            log("HW inventory OK after AHUB9 reset 120s later")


        time.sleep(60) #sleep 1 minute to wait AHUB9 start up
        log("sleep 60s(120+60) to wait AHUB9 start up")

        #check if acar clock will be locked after AHUB9 start up again
        clockStateAfterAHUB9 = get_acar_clock_state()
        if cmp(clockStateAfterAHUB9, clockStateBefore) != 0:
            log("Attention, clock in ACAR doesn't recover after AHUB9 reset in 3 minutes!!!!!!!!!!!!!!!!!!!!!!!")
            verdict(VERDICT_FAIL)
        else:
            log("clock in ACAR OK after AHUB9 reset")

    else:
        AHUB9_flag = False

    #if both AHUB8 and AHUB9 reset successfully, then break out from loop.
    if AHUB8_flag and AHUB9_flag:
        log("AHUB8 and AHUB9 reset successfully")
        break
    i = i + 1

#check both AHUB8 and AHUB9 reset successfully or not after 3 times try
if not (AHUB8_flag and AHUB9_flag):
    log("Attention, AHUB8 or AHUB9 reset failed after 3 try!!!!!!!!!!!!!!")
    verdict(VERDICT_FAIL)
    stop_tc_execution()
     
#get bi and fi config after switch reset
switch_config_after = []
for i in range(len(switch_list)):
    cmd1 = "ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no %s \"cat  /etc/fastpath/fpconfig0.txt|grep -v \'System Up Time\'|sed -e \'s/[ ]*$//g\'\"" %(switch_list[i])
    cmd2 = "ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no %s \"cat  /etc/fastpath/fpconfig1.txt|grep -v \'System Up Time\'\"" %(switch_list[i])
    switch_config_after.append(cli(cmd1))
    switch_config_after.append(cli(cmd2))

#compare config of AHUB bi&fi config after AHUB reset.
for i in range(len(switch_config_before)):
    if cmp(switch_config_before[i], switch_config_after[i]) != 0:
        if i%2 ==0:
            log("Attention, bi config change after AHUB reset!!!!!!!!!!!!!")
        else:
            log("Attention, fi config change after AHUB reset!!!!!!!!!!!!!")
        verdict(VERDICT_FAIL)
    else:
        log("config for BI or FI OK after AHUB reset")
