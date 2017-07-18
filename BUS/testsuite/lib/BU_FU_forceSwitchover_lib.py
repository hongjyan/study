#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.4
# 2013.1.9 force switchover OMU command changed from cli(cmd) to cli(cmd, wait_prompt=False) and no echo $? after OMU switchover
# 2013.2.19 Expand forceswitchover recover time for TDMSNIUP to 15s by YHJ
# 2013.5.21 expand forceswitchover recover time for TDMSNIUP to 75s by YHJ.
# 2013.6.5 expand forceswitchover recover time for CM to 30s by YHJ.
#=====================================================================
#!/usr/bin/python
import time
g_debug = True

if g_debug:
    from boa_simulate import *

lib_comm = boaimport('BU_common_lib')  #import BU_common_lib

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

def uniqueList(l):
        return lib_comm.uniqueList(l)

class forceSwitchoverILRG:
    #init funcation to initialize data member:self.__FU and self.otherRGInstatnce. 
        #Eg: SISURG = forceSwitchoverILRG("MGW_SISURG-0")
        #Then self.__FU is SISU and self.otherInstances = [MGW_SISURG-1, MGW_SISURG-2, MGW_SISURG-3]
        def __init__(self,RG): 
                self.RG =  RG
                self.__FU = RG[0:RG.find('RG')].replace('MGW_','')
                self.FUState = {}
                self.otherRGInstances = []
                forceSwitchoverILRG.__getOtherRGInstances(self)

        #get otherRG instances,Eg: if one object of this class is SISURG0 = forceSwitchoverILRG("MGW_SISURG-0"),
        #then return value of this funcation should be ['MGW_SISURG-1', 'MGW_SISURG-2', 'MGW_SISURG-3']
        def __getOtherRGInstances(self):
                cmd = 'ilclifunit -v -t ' + self.__FU + '|grep ' + self.__FU  + '|grep -v ' + self.RG
                log(cmd)
                output = cli(cmd)
                log(output)
                ls = output.splitlines()
                for l in ls:
                        s = l.split()
                        self.otherRGInstances.append(s[6])
                self.otherRGInstances = uniqueList(self.otherRGInstances)
                return self.otherRGInstances


        #get RG's FUs's state 
        def __getFUState(self):
                cmd = 'ilclifunit -v -t ' + self.__FU + '|grep ' + self.RG
                log(cmd)
                output = cli(cmd)
                log(output)
                ls = output.splitlines()
                for l in ls:
                        s = l.split()
                        self.FUState[s[0]] = s[3]

        #conclude this RG switchover or not
        def canRGSwitchover(self):
                forceSwitchoverILRG.__getFUState(self)
                if self.FUState.items()[0][1] == 'WO-EX' and self.FUState.items()[1][1] == 'SP-EX':
                        return True
                elif self.FUState.items()[0][1] == 'SP-EX' and self.FUState.items()[1][1] == 'WO-EX':
                        return True
                else:
                        return False
                log("CanRGSwitchover:unexpected error, exit(-1)")
                sys.exit(-1)


        #force switchover this RG
        def doSwitchover(self):
                if not forceSwitchoverILRG.canRGSwitchover(self):
                        log("RG switchover condition not fullfilling")
                        return False
                pre_FUState = {}
                for k,v in self.FUState.items():
                    pre_FUState[k] = v
                if self.__FU == 'OMU':  
                    cmd = 'hascli -WnF /' + self.RG
                    log(cmd)
                    output = cli(cmd, wait_prompt=False)
                    log(output)
                else:
                    cmd = 'hascli -Wn /' + self.RG
                    log(cmd)
                    output = cli(cmd)
                    log(output)
                    rc = cli('echo $?')
                    if int(rc): #RC is 0 if operation succeed.
                        log("switchover Failed")
                        return False

                if "TDMSNIUP" == self.__FU:
                    time.sleep(75) #sleep 75 since pre active TDMSNIUP will be reset after force switchover 
                if "CM" == self.__FU:
                    time.sleep(30)
                else:
                    time.sleep(5)
                forceSwitchoverILRG.__getFUState(self)
                if self.FUState.items()[0][1] != pre_FUState.items()[1][1] or \
                                self.FUState.items()[1][1] != pre_FUState.items()[0][1]:
                        log("Force switchOver finished, but RU haven't gone to new ACTIVE/STANDBY state")
                        return False
                return True






if __name__ == "__main__":
        SISURG0_switchover = forceSwitchoverILRG('MGW_SISURG-0')
        SISURG0_switchover.doSwitchover()


