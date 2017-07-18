#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.17
# 2013.1.9 bug fix
# 2013.1.9 control switchover OMU command changed from cli(cmd) to cli(cmd, *other)
# 2013.1.24 change cli(cmd, *other) to cli(cmd, wait_prompt=True)
# 2013.6.20 prevent space line added into "hascli --bare|grep SISU" in get_all_RG_instances()
#=====================================================================
#!/usr/bin/python
import random
import time
g_debug = True
if g_debug:
    from boa_simulate import *
lib_MO = boaimport('BU_MOState_lib')


if g_debug:
    def log(str):
        return lib_MO.log(str)
 
    def cli(str, wait_prompt=True):
        return lib_MO.cli(str, wait_prompt)
 
    def stop_tc_execution():
        return lib_MO.stop_tc_execution()
 
    def wait_trigger():
        return lib_MO.wait_trigger()
 
    def send_trigger():
         return lib_MO.send_trigger()

#Get all RG instances belonging to one RG Type 
def get_all_RG_instances(RGType):
        RGList = []
        cmd = 'hascli --bare rg' + '|grep ' + RGType 
        log(cmd)
        output = cli(cmd)
        log(output)
        ls = output.splitlines() 
        RGList = [l for l in ls if l.startswith("/")]
        return RGList

#Get all enabled RG instances belonging to one RG Type
def get_all_available_RG_instances(RGType):
        avaRGList = []
        RGList = get_all_RG_instances(RGType)
        for e in RGList:
                eState = lib_MO.class_MOState(e)
                if eState.MO_is_OK()[0]:
                        avaRGList.append(e)
                else:
                        pass
        return avaRGList

#select n element from a list randomly
def select_nRG_fromRGList(RGList,n):
        if len(RGList) < n:
                log('Amount of RGs is less than n, exit')
                stop_tc_execution()
        random.shuffle(RGList) #shuffle RGList to select 2 random RGs
        return  RGList[0:n]

#get all RU which are belonged to one RG 
def get_RU(RG):
    RU = []
    cmd = 'hascli --children ' + RG + '|awk -F / \'{if(NF==3)print $0}\''
    log(cmd)
    output = cli(cmd)
    log(output)
    for U in output.splitlines():
        RU.append(U)
    return RU

def get_nodes(RG):
    nodes = []
    cmd = "hascli --children %s |awk -F / '{print $2}'|uniq" %(RG)
    log(cmd)
    output = cli(cmd)
    log(output)
    for i in output.splitlines():
        nodes.append(i)
    return nodes

def get_redundancy_OK_RGInstances(RGType):
    avaRGList = get_all_available_RG_instances(RGType)
    RGInstances = []
    for RG in avaRGList:
        RUs = get_RU(RG)
        for RU in RUs:
            if lib_MO.class_MOState(RU).MO_is_OK()[0]:
                RGInstances.append(RG)
    RGInstances = list(set(RGInstances))
    return RGInstances


        


def redundancy_is_OK(rg):
    try:
        redundancyType = get_RG_redundancyType(rg)
        if 'NoRedundancy' == redundancyType:
            raise Exception
    except:
        log('NoRedundancy RG')
    
    RUState = RUState_of_RG(rg)
    if 'ColdActiveStandby'== redundancyType or 'HotActiveStandby' == redundancyType or 'ColdOnePlusM' == redundancyType:
        for key in RUState.keys(): 
            if not RUState[key][0]: #if RU's operation & administrative & procedural & alarm state & dynamic attritutes are not OK, return False
                return False
            if not  RUState[key][2]: # if RU's FUNCTIONAL_LEVEL isn't 100, return False
                return False
        if ('ACTIVE' == RUState[RUState.keys[0]][1] and 'COLDSTANDBY' == RUState[RUState.keys[1]][1]) or \
           ('COLDSTANDBY' == RUState[RUState.keys[0]][1] and 'ACTIVE' == RUState[RUState.keys[1]][1]) or \
           ('ACTIVE' == RUState[RUState.keys[0]][1] and 'HOTSTANDBY' == RUState[RUState.keys[1]][1]) or  \
           ('HOTSTANDBY' == RUState[RUState.keys[0]][1] and 'ACTIVE' == RUState[RUState.keys[1]][1]):
            return True
        return False
    
    return False  #To-do, waiting N+m Redundancy.
        
    
  
        
        

#control switchover RG and conclude if RG switchover successfully or not.
def controlSwitchover_RG(RG):
    RU = get_RU(RG)
    critical_RG = ["/SSH", "/MGW_OMURG","/Directory"]
    if critical_RG.count(RG):
        cmd = 'hascli -wnF ' + RG
    else:
        cmd = 'hascli -wn ' + RG
    RU0State = lib_MO.class_MOState(RU[0]).MO_is_OK()
    RU1State = lib_MO.class_MOState(RU[1]).MO_is_OK()
    if RU0State[0] != True or RU1State[0] != True:
        log('RG can not switchover')
        stop_tc_execution()
    elif (RU0State[1] == 'ACTIVE' and RU1State[1].find('STANDBY')) or \
         (RU1State[1] == 'ACTIVE' and RU0State[1].find('STANDBY')):
        log(cmd)
        if ("/MGW_OMURG" == RG) or ("/SSH" == RG):
            output = cli(cmd, wait_prompt=False)
        else:
            output = cli(cmd)
        log(output)
        time.sleep(3)  #wait 5s for RU switchover done
        newRU0State = lib_MO.class_MOState(RU[0]).MO_is_OK()
        newRU1State = lib_MO.class_MOState(RU[1]).MO_is_OK()
        if (newRU0State == RU1State) and (newRU1State == RU0State):
            log('switchover successfully')
            return True
        else:
            log('switchover done, but RU state have not been ready')
            stop_tc_execution()


#Force switchover RG and conclude if RG switchover successfully or not.
def forceSwitchover_RG(RG):
    RU = get_RU(RG)
    critical_RG = ["/SSH", "/MGW_OMURG"]
    if critical_RG.count(RG):
        cmd = 'hascli -WnF ' + RG
    else:
        cmd = 'hascli -Wn ' + RG
    RU0State = lib_MO.class_MOState(RU[0]).MO_is_OK()
    RU1State = lib_MO.class_MOState(RU[1]).MO_is_OK()
    if RU0State[0] != True or RU1State[0] != True:
        log('RG can not switchover')
        stop_tc_execution()
    elif (RU0State[1] == 'ACTIVE' and RU1State[1].find('STANDBY')) or \
         (RU1State[1] == 'ACTIVE' and RU0State[1].find('STANDBY')):
        log(cmd)
        if ("/MGW_OMURG" == RG) or ("/SSH" == RG):
            output = cli(cmd, wait_prompt=False)
        else:
            output = cli(cmd)
        log(output)
        newRU0State = lib_MO.class_MOState(RU[0]).MO_is_OK()
        newRU1State = lib_MO.class_MOState(RU[1]).MO_is_OK()
        time.sleep(3)  #wait 5s for RU switchover done
        if (newRU0State == RU1State) and (newRU1State == RU0State):
            log('switchover successfully')
            return True
        else:
            log('switchover done, but RU state have not been ready')
            stop_tc_execution()

                 
if __name__ == '__main__':
    SISUList = get_all_available_RG_instances('SISU')
    print SISUList
    print select_nRG_fromRGList(SISUList,2)
    print SISUList
    print select_nRG_fromRGList(SISUList,2)
    print SISUList
    print select_nRG_fromRGList(SISUList,2)
    print get_RU('/SSH')
    #print controlSwitchover_RG('/MGW_HCLBRG-1')
    print get_redundancy_OK_RGInstances('SISU')
    print forceSwitchover_RG("/MGW_OMURG")
    print get_nodes("/MGW_HCLBRG-0")
