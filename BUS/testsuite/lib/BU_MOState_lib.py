#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
#
# Modified: 2012.9.17
# 2013.1.24 modify cli(str) to cli(str, wait_prompt=True)
#=====================================================================
#########################################################################################################################################################
#Argument for this class: MO name. Eg: /CLA-0
#
#@Data member: self.MO, self.MOState, self.MODic. 
#self.MO is MO name Eg: /CLA-0. 
#self.MOState is output of "hascli -s /MO". 
#self.MODic is a dictionary of this MO, Eg: {'oper': 'ENABLED', 'admin': 'UNLOCKED', 'unknown': 'FALSE', 'alarm': '-', 'role': 'ACTIVE', 'usage': 'ACTIVE', 'availability': '-', 'procedural': '-'}
#
#@Function member: MO_is_OK(). 
#This function will return a tuple, first element of this tuple indicts this MO's state, second indicts MO's role including /ACTIVE/COLDSTANDBY/HOTSTANDBY.
#########################################################################################################################################################
import re

g_debug=True
#g_debug=False

if g_debug:
    from boa_simulate import *

lib_comm=boaimport('BU_common_lib')


if g_debug:
	def log(str):
		return lib_comm.log(str)

	def cli(str, wait_prompt=True):
		return lib_comm.cli(str, wait_prompt)

	def stop_tc_execution():
		return lib_comm.stop_tc_execution()

	def wait_trigger():
		return lib_comm.wait_trigger()

	def send_trigger():
		return lib_comm.send_trigger()

def report_fail():
    return lib_comm.report_fail()

def report_pass():
    return lib_comm.report_pass()

class class_MOState(object):
    def __init__(self, MO):
        self.MO = MO
        #log("hascli -s %s" %self.MO)
        self.MOState = cli("hascli -s %s" %self.MO)
        #log(self.MOState)
        self.MODic = {'name':MO, 'admin':"", 'oper':"", 'usage':"", 'procedural':"",\
                     'availability':"", 'unknown':"", 'alarm':"", 'role':"", 'Dynamic attributes':{}}
        
        match = re.search("administrative.*",self.MOState)
        if match:
            self.MODic['admin'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['admin']:
                self.MODic['admin'] = '-'
        else:
            del self.MODic['admin']

        match = re.search("operational.*",self.MOState)
        if match:
            self.MODic['oper'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['oper']:
                self.MODic['oper'] = '-'
        else:
            del self.MODic['oper']

        match = re.search("usage.*",self.MOState)
        if match:
            self.MODic['usage'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['usage']:
                self.MODic['usage'] = '-'
        else:
            del self.MODic['usage']

        match = re.search("procedural.*",self.MOState)
        if match:
            self.MODic['procedural'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['procedural']:
                self.MODic['procedural'] = '-'
        else:
            del self.MODic['procedural']

        match = re.search("availability.*",self.MOState)
        if match:
            self.MODic['availability'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['availability']:
                self.MODic['availability'] = '-'
        else:
            del self.MODic['availability']

        match = re.search("unknown.*",self.MOState)
        if match:
            self.MODic['unknown'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['unknown']:
                self.MODic['unknown'] = '-'
        else:
            del self.MODic['unknown']

        match = re.search("alarm.*",self.MOState)
        if match:
            self.MODic['alarm'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['alarm']:
                self.MODic['alarm'] = '-'
        else:
            del self.MODic['alarm']

        match = re.search("role.*",self.MOState)
        if match:
            self.MODic['role'] = re.search("\(.*\)", match.group()).group(0).strip("\(")\
                              .strip("\)")
            if not self.MODic['role']:
                self.MODic['role'] = '-'
        else:
            del self.MODic['role']
        
        match = re.search("Dynamic attributes", self.MOState)
        if match:
            attribute_list = []
            temp_list = self.MOState.split('Dynamic attributes:')[1].split('=')
            for i in range(len(temp_list)):
                sl = temp_list[i].split('\n')
                for j in range(len(sl)):
                    if not '' == sl[j]:
                        attribute_list.append(sl[j].strip())
            j = 0
            try:   #needed in BOA
                attribute_list.remove('')  #needed in BOA
            except:   #needed in BOA
                #log("normal exception, no empty element in list") #needed in BOA
                pass

            while(j != len(attribute_list)):
                self.MODic['Dynamic attributes'][attribute_list[j]] = attribute_list[j+1]
                j = j+2
        else:
            del self.MODic['Dynamic attributes']
    
    def attributes_are_OK(self):
        state = [True, 'None']
        for key in self.MODic['Dynamic attributes'].keys():
            if 'RESOURCE_STATE' == key:
                if 'FUNCTIONAL' != self.MODic['Dynamic attributes'][key]:
                    state[0] = False
                continue
            if 'RESOURCE_LEVEL' == key:
                if '100' != self.MODic['Dynamic attributes'][key]:
                    state[1] = 'WARNING'
                continue
            if 'WAITING_SERVICE' == key:
                motherMOState = class_MOState(self.MODic['Dynamic attributes'][key])
                if motherMOState.MODic['role'] == 'ACTIVE' and motherMOState.MODic['procedural'] == '-':
                    print "%s is OK" %(motherMOState.MO)
                    state[0] = False
                continue
            if 'FAULTY_DSPS' == key:
                if -1 != self.MODic['Dynamic attributes'][key].find('1'):
                    state[0] = False
                continue
            if 'OOSYNCH_DSPS' == key:
                if -1 != self.MODic['Dynamic attributes'][key].find('1'):
                    state[1] = 'WARNING'
                continue

            log(self.MO + " has unexpected attribute: " + key) 
            
        return state



    def MO_is_OK(self):
        if not self.MODic.has_key('role'):
            if self.MODic['admin'] != 'UNLOCKED' or self.MODic['oper'] != 'ENABLED' or \
            self.MODic['usage'] != 'ACTIVE' or self.MODic['procedural'] != '-' or \
            self.MODic['availability'] != '-' or self.MODic['unknown'] != 'FALSE'or self.MODic['alarm'] != '-':
                return (False, 'None', 'None')
            if not self.MODic.has_key('Dynamic attributes'):
                return (True, 'None', 'None')
            else: 
                attState = self.attributes_are_OK()
                return(attState[0], 'None', attState[1])
                                     
        elif not self.MODic.has_key('Dynamic attributes'):
            if self.MODic['admin'] == 'UNLOCKED' and self.MODic['oper'] == 'ENABLED' and \
            self.MODic['usage'] == 'ACTIVE' and self.MODic['procedural'] == '-' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE'and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'ACTIVE':
                return (True, "ACTIVE", 'None')

            elif self.MODic['admin'] == 'UNLOCKED' and self.MODic['oper'] == 'ENABLED' and \
            self.MODic['usage'] == 'ACTIVE' and self.MODic['procedural'] == '-' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE' and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'HOTSTANDBY':
                return (True, "HOTSTANDBY", 'None')

            elif self.MODic['admin'] == 'UNLOCKED' and self.MODic['oper'] == 'ENABLED' and \
            self.MODic['usage'] == 'IDLE' and self.MODic['procedural'] == 'NOTINITIALIZED' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE'and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'COLDSTANDBY':
                return (True, "COLDSTANDBY", 'None')
            return(False, self.MODic['role'], 'None')

        else:
            attState = self.attributes_are_OK()
            if self.MODic['admin'] == 'UNLOCKED' and self.MODic['oper'] == 'ENABLED' and \
            self.MODic['usage'] == 'ACTIVE' and self.MODic['procedural'] == '-' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE'and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'ACTIVE' and attState[0]:
                return (True, "ACTIVE", 'None')

            elif self.MODic['admin'] == 'UNLOCKED' and self.MODic['oper'] == 'ENABLED' and \
            self.MODic['usage'] == 'ACTIVE' and self.MODic['procedural'] == '-' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE' and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'HOTSTANDBY' and attState[0]:
                return (True, "HOTSTANDBY", 'None')

            elif self.MODic['admin'] == 'UNLOCKED' and self.MODic['oper'] == 'ENABLED' and \
            self.MODic['usage'] == 'IDLE' and self.MODic['procedural'] == 'NOTINITIALIZED' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE'and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'COLDSTANDBY' and attState[0]:
                return (True, "COLDSTANDBY", 'None')
            
            elif self.MODic['Dynamic attributes'].has_key('WAITING_SERVICE') and \
            self.MODic['usage'] == 'IDLE' and self.MODic['procedural'] == 'NOTINITIALIZED' and \
            self.MODic['availability'] == '-' and self.MODic['unknown'] == 'FALSE'and \
            self.MODic['alarm'] == '-' and self.MODic['role'] == 'ACTIVE':
                return (attState[0], "ACTIVE", attState[1])
            
            return(False, self.MODic['role'], 'None')

if __name__=="__main__":


    SSHState = class_MOState("/SSH")
    CLA0State = class_MOState("/SSH")
    OMU1State = class_MOState("/CLA-1/MGW_OMUFU-1")
    IPNI1P_0_State = class_MOState("/IPNI1P-0")
    TCU1State = class_MOState("/TCU-1")
    TCU0State = class_MOState("/TCU-0")
    CM1State = class_MOState("/CLA-1/MGW_CMFU-1")
    HPISupportState = class_MOState("/CLA-0/FSHPISupportServer")

    print SSHState.MODic
    print SSHState.MO_is_OK()
    a = SSHState.MO_is_OK()
    print a
    print CLA0State.MODic
    print CLA0State.MO_is_OK()
    print OMU1State.MODic
    print OMU1State.MO_is_OK()
    print IPNI1P_0_State.MODic
    print IPNI1P_0_State.MO_is_OK()
    print TCU1State.MODic	
    print TCU0State.MODic
    print HPISupportState.MO_is_OK()
    print CM1State.MO_is_OK()
