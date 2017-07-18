#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.12.18
# 2013.2.18 expand TDMSNIUP recover time to 40s by YHJ.
# 2013.2.19 expand CM recover time to 30s in case multi-shelf are used by YHJ.
# 2013.7.3 add SCLI command "show functional-unit unit-info" after restart
#=====================================================================
#!/usr/bin/python
import time
g_debug = True

if g_debug:
    from boa_simulate import *

lib_comm = boaimport('BU_common_lib')

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

class restartILRG:
        #init funcation to initialize data member:self.__FU and self.otherRGInstatnce. 
        #Eg: SISURG = forceSwitchoverILRG("MGW_SISURG-0")
        #Then self.__FU is SISU and self.otherInstances = [MGW_SISURG-1, MGW_SISURG-2, MGW_SISURG-3]
        def __init__(self,RG):
                self.RG =  RG
                self.__FU = RG[0:RG.find('RG')].replace('MGW_','')
                self.FUState = {}
                self.otherRGInstances = []
                restartILRG.__getOtherRGInstances(self)

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

        #At least one RU of this RG should be 'WO-EX'
        def canRGRestart(self):
            restartILRG.__getFUState(self)
            for item in self.FUState.items():
                if item[1] == 'WO-EX':
                    return True
            return False


        #Restart RG and check if RG start up or not after fixed interval
        def doRestart(self):
                if not restartILRG.canRGRestart(self):
                        log("RG restart condition not fullfilling")
                        return False
                pre_FUState = {}
                for k,v in self.FUState.items():
                    pre_FUState[k] = v
                if self.__FU == 'OMU':
                    cmd = 'hascli -rnF /' +self.RG
                else:
                    cmd = 'hascli -rn /' + self.RG
                log(cmd)
                output = cli(cmd)
                log(output)
                rc = cli('echo $?')
                if  int(rc):
                        log("restart Failed")
                        return False
                if self.__FU =='OMU':
                        time.sleep(40)  #OMU will recover in 40s
                        log('sleep 40s for OMU recover')
                elif self.__FU == 'DSPM':
                        time.sleep(120) #DSPM will recover in 120s
                        log('sleep 120s for DSPM recover')
                elif self.__FU == 'TDMSNIUP':
                        time.sleep(40)  #TDMSNIUP will recover in 30s
                        log('sleep 40s for TDMSNIUP recover')
                elif self.__FU == 'TDMMGU':
                        time.sleep(60)  #TDMMGU will recover in 40s
                        log('sleep 60s for TDMMGU recover')
                elif self.__FU == 'CM':
                        time.sleep(30)
                        log("sleep 30s for CM recover in case 3 shelves are used")
                else:
                        time.sleep(5)
                        log('sleep 5s for RG recover')

                restartILRG.__getFUState(self)
                lib_comm.bus_fsclish("show functional-unit unit-info")

                for item in self.FUState.items():
                    if item[1] == 'WO-EX':
                        return True
                log("Restart finished, but RG haven't been in service")
                return False






if __name__ == "__main__":
    SISURG0_restart = restartILRG('MGW_SISURG-0')
    SISURG0_restart.doRestart()
    TDMMGU_restart = restartILRG('MGW_TDMMGURG')
    TDMMGU_restart.doRestart()
