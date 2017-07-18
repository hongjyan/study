import sys
sys.path.append('..')
from  lib.BU_Base_lib import *
from time import *

def all_nodes_should_be_available():
    all_objects_should_be_available("NODE", "ADMINISTRATIVE")
    
def all_nodes_should_be_unlock():
    all_objects_should_be_available("NODE", "OPERATIONAL")

def all_rgs_should_be_available():
    all_objects_should_be_available("RG", "ADMINISTRATIVE")

def all_rgs_should_be_unlock():
    all_objects_should_be_available("RG", "OPERATIONAL")

def all_rus_should_be_available():
    all_objects_should_be_available("RU", "ADMINISTRATIVE")

def all_rus_should_be_unlock():
    all_objects_should_be_available("RU", "OPERATIONAL")

def all_process_should_be_available():
    all_objects_should_be_available("PROCESS", "ADMINISTRATIVE")

def all_process_should_be_unlock():
    all_objects_should_be_available("PROCESS", "OPERATIONAL")

def all_nodes_should_be_ok():
 	all_nodes_should_be_available()

def all_rgs_should_be_ok():
 	all_rgs_should_be_available()

def all_rus_should_be_ok():
 	all_rus_should_be_available()

def important_rgs_should_be_ok():
    rgs = get_important_rgs()
    test_mo_state_from_object_list(rgs, "RG", "DEFAULT")

def important_rus_should_be_ok():
    rus = get_important_rus()
    test_mo_state_from_object_list(rus, "RU", "DEFAULT")

def important_rgs_should_be_ok_after_restarting():
    rgs = get_important_rgs()
    for rg in rgs:
        cli("hascli -rn /" + rg)
        sleep(3)
    sleep(10)
    important_rgs_should_be_ok()

def important_rgs_should_be_ok_after_switchovering():
    rgs = get_important_rgs()
    for rg in rgs:
        cli("hascli -Wn /" + rg)
        sleep(3)
    sleep(10)
    important_rgs_should_be_ok()

def important_rus_should_be_ok_after_restarting():
    rus = get_important_rus()
    for ru in rus:
        cli("hascli -rn /" + ru)
        sleep(1)
    sleep(10)
    important_rus_should_be_ok()

def important_rus_should_be_ok_after_switchovering():
    rus = get_important_rus()

    old_roles = {}
    for ru in rus:
	if re.search('CLA-0', ru):
	    old_roles[ru] = get_obj_role(ru)

    for ru in rus:
	if re.search('CLA-0', ru):
	    cli("hascli -Wn /" + ru)
	    sleep(1)
    sleep(10)

    important_rus_should_be_ok()
    for ru in rus:
	if re.search('CLA-0', ru):
	    new_role = get_obj_role(ru)
	    if not (old_roles[ru] == 'ACTIVE' and new_role == 'HOTSTANDBY' or old_roles[ru] == 'HOTSTANDBY' and new_role == 'ACTIVE'): 
		log(old_roles[ru] + '|' + new_role)
		log(ru + ' is not OK after switchovering, role is not exchange with each other')
		verdict(VERDICT_FAIL)

def HAS_checking():
    check_cmf_state()
    check_FSClusterStateServer()

def CLA_Nodes_checking():
    mos = GetAvailableNodesState(['CLA-0', 'CLA-1']) 
    for mo in mos:
	if not mo.MO_is_OK():
	    verdict(VERDICT_FAIL)


def Call_related_service_checking():
    check_function_unit('OMU')
    check_function_unit('CM')
    check_function_unit('HCLB')
    check_function_unit('SISU')
    check_function_unit('DSPM')
    check_function_unit('IPNIU')
    check_function_unit('TDMSNIUP')
    check_dsp_cores()

def Singaling_service_checking():
    check_single_obj('SS7SGU')
    check_single_obj('SGWNetMgr')

def WebUI_service_checking():
    check_single_obj('HTTPDPlat')
    check_single_obj('TomcatPlat')
    check_single_obj('Ne3sAgent')

if __name__ == "__main__":
    #all_nodes_should_be_available()
    ##all_nodes_should_be_unlock()
    #all_rgs_should_be_available()
    ##all_rgs_should_be_unlock()
    #all_rus_should_be_available()
    ##all_rus_should_be_unlock()
    #all_process_should_be_available()
    ##all_process_should_be_unlock()

	HAS_checking()
	#CLA_Nodes_checking()
	#check_FSDirectoryServer()
	#check_FSSCLIFwkServer()
	#check_FSClusterStateServer()

	#Call_related_service_checking()

	#Singaling_service_checking()
	#WebUI_service_checking()

 	#all_nodes_should_be_ok()
 	#all_rgs_should_be_ok()
 	#all_rus_should_be_ok()


#	location = '/root/BUS/cnf/mgw_setting.cnf'
#	try:
#	    FSetupParser.parsevars(location)
#	except FileNotFound, ff:
#	    print ff
#	    sys.exit()

#	important_rgs_should_be_ok()
#	important_rgs_should_be_ok_after_restarting()
#	important_rgs_should_be_ok_after_switchovering()
#
#	important_rus_should_be_ok()
#	important_rus_should_be_ok_after_restarting()
#	important_rus_should_be_ok_after_switchovering()
#


