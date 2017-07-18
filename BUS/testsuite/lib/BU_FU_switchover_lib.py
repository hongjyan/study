#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2013.1.4
# 2013.1.9 bug fix 
# 2013.1.9 control switchover OMU command changed from cli(cmd) to cli(cmd, wait_prompt=False) and no echo $? after OMU switchover
#=====================================================================
###########################################################################################
#Funcation is the same as BU_FU_forceSwitchover_lib.py, the only difference
#is that doSwitchover() here is do control switchover, but in BU_FU_forceSwitchover_lib.py,
#doSwitchover() is do force switchover.
###########################################################################################
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

class switchoverILRG:
        def __init__(self,RG):
                self.RG =  RG
                self.__FU = RG[0:RG.find('RG')].replace('MGW_','')
                self.FUState = {}
                self.otherRGInstances = []
                switchoverILRG.__getOtherRGInstances(self)

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


        def __getFUState(self):
                cmd = 'ilclifunit -v -t ' + self.__FU + '|grep -w ' + self.RG
                log(cmd)
                output = cli(cmd)
                log(output)
                ls = output.splitlines()
                for l in ls:
                        s = l.split()
                        self.FUState[s[0]] = s[3]

        def canRGSwitchover(self):
                switchoverILRG.__getFUState(self)
                if self.FUState.items()[0][1] == 'WO-EX' and self.FUState.items()[1][1] == 'SP-EX':
                        return True
                elif self.FUState.items()[0][1] == 'SP-EX' and self.FUState.items()[1][1] == 'WO-EX':
                        return True
                else:
                        return False
                log("CanRGSwitchover:unexpected error, exit(-1)")
                sys.exit(-1)



        def doSwitchover(self):
                if not switchoverILRG.canRGSwitchover(self):
                        log("RG switchover condition not fullfilling")
                        return False
                pre_FUState = {}
                for k,v in self.FUState.items():
                    pre_FUState[k] = v
                if self.__FU == 'OMU':
                    cmd = 'hascli -wnF /' + self.RG
                    log(cmd)
                    output = cli(cmd, wait_prompt=False)
                    log(output)
                else:
                    cmd = 'hascli -wn /' + self.RG
                    log(cmd)
                    output = cli(cmd)
                    log(output)
                    rc = cli('echo $?')
                    if int(rc):
                        log("switchover failed")
                        return False
                time.sleep(3)
                switchoverILRG.__getFUState(self)
                if self.FUState.items()[0][1] != pre_FUState.items()[1][1] or \
                                self.FUState.items()[1][1] != pre_FUState.items()[0][1]:
                        log("Control switchOver finished, but RU haven't gone to new ACTIVE/STANDBY state")
                        return False
                return True






if __name__ == "__main__":
        SISURG0_switchover = switchoverILRG('MGW_SISURG-0')
        SISURG0_switchover.doSwitchover()
