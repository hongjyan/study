#=====================================================================
# Copyright (c) Nokia Siemens Networks 2008. All rights reserved.
#
# Media Gateway Bringup Team script (does a lot more also..)
# Modified: 2013.1.4
# 2013.1.8 get_line(file, kw) added by yhj
# 2013.1.17 LogManager.call() is modifed, stop_tc_execution will be called if script execute fail.
# 2013.2.25 modify cluster_name() since \n are added in new FP By YHJ.
#=====================================================================
import os
import sys
import re
import time
import getopt
g_debug=True
#g_debug=False
if g_debug:
    from boa_simulate import *

lib_FS = boaimport('BU_FSetupVarParser_Lib')
lib_sgw_delete = boaimport('delete_sgw_lib')
lib_sgw_add = boaimport('add_sgw_lib')


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

        
	
class LogManager:
    def __init__(self,testcase):
        self.log_path = '/var/log/bus'
        self.master_syslog = '/var/log/master-syslog'
        self.setup(testcase)

    def setup(self,testcase):
        seed_time = time.strftime('%Y-%m-%d_%H-%M-%S',time.localtime(time.time()))
        self.current_output_path = self.log_path + '/' + seed_time + '_' + testcase 
        self.current_syslog_path = self.log_path + '/' + seed_time + '_' + testcase + '/syslog'
        os.popen('mkdir -p ' + self.current_output_path)	
        os.popen('mkdir -p ' + self.current_syslog_path)

    def stdout_redirect(self, str): 
        output = open(self.current_output_path + '/output.log', 'a') 
        output.write(str)
        output.close()

    def syslog_redirect(self, str): 
        output = open(self.current_syslog_path + '/' + self.tc.__name__ + '.log', 'a') 
        output.write(str)
        output.close()

    def write(self, str):	
        self.stdout_redirect(str)

    def call(self, tc, *p): # p is parameters for tc
        if g_debug:
            log_file = '/var/log/master-syslog'
            end_line = os.popen('tail -n 1 ' + log_file).read()
            now_time = end_line.split()[2]

            self.tc = tc
            old_stdout = sys.stdout 
            sys.stdout = self
            print ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%s start>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" %(self.tc.__name__)
            bus_cli('date')
            res = tc(*p)
            print "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>%s end>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n" %(self.tc.__name__)
            sys.stdout = old_stdout
            log_with_color(tc.__name__,res)

    	    cmd = "sed -n '/" + now_time + "/,//p'" + " " + log_file
    	    logs = os.popen(cmd).read()
            self.syslog_redirect(logs)
            return res
        else:
            if not tc(*p):
                verdict('VEDICT_FAIL')
                stop_tc_execution()

def bus_log(str):
    if g_debug:
	print str
    else:
	log(str)


def log_with_color(base_str, res):
    if g_debug:
        print base_str,
        if res:
	   print "\033[1;32;40m%s\033[0m" % "	PASS"
        else:
	   print "\033[1;31;40m%s\033[0m" % "	FAILED"
    else:
	log(base_str + '	' + str(res))


# find word equal to s from l.
def find_equal(s, l):
    for i in l:
        if s == i:
            return True
    return False

def add_vlan(name, interface, vlanid, invlan):
    bus_fsclish("add networking vlan %s vlaniface %s vid %s realiface %s" %(name, interface, vlanid, invlan))
    out = bus_fsclish("show networking vlan iface %s vid %s realiface %s" %(interface, vlanid, invlan))  # need improve in future
    if re.search(vlanid, out):
        return True
    else:
        print "*************************"
        print "vlanid = %s, out = %s" %(vlanid, out)
        return False

def delete_vlan(name, interface, vlanid, invlan): #other won't be used.
    bus_fsclish("delete networking vlan %s iface %s" %(name, interface))
    out = bus_fsclish("show networking vlan iface %s vid %s realiface %s" %(interface, vlanid, invlan))
    if re.search(name, out):
        return False
    else:
        return True

def add_ip(name, interface, ip, mask, other=""):
    bus_fsclish("add networking address dedicated %s iface %s ip-address %s/%s %s" %(name, interface, ip, mask, other))
    out = bus_fsclish("show networking address owner %s iface %s ip-address %s/%s" %(name, interface, ip, mask))  # need improve in future
    if re.search(ip, out):
        return True
    else:
        print "*************************"
        print "ip = %s, out = %s" %(ip, out)
        return False

def add_instance(vrf, name, interface, ip, mask):
    bus_fsclish("add networking instance %s address dedicated %s iface %s ip-address %s/%s" %(vrf, name, interface, ip, mask))
    out = bus_fsclish("show networking instace %s address owner %s iface %s ip-address %s/%s" \
                       %(vrf, name, interface, ip, mask))  
    if re.search(ip, out):
        return True
    else:
        return False

def add_instance_ip(vrf, node, interface, vid, vlanif, name, ip, mask):
    bus_fsclish("add networking instance %s vlan %s realiface %s vid %s vlaniface %s" %(vrf, node, interface, vid, vlaniface))
    out = bus_fsclish("show networking instance %s vlan owner %s realiface %s vid %s" %(vrf, node, interface, vid))
    if not re.search(vlanif, out):
        return False
    return add_instace(vrf, name, interface, ip, mask)

def delete_ip(name, interface, ip, mask, other=""): #other won't be used.
    bus_fsclish("delete networking address dedicated %s iface %s ip-address %s" %(name, interface, ip))
    out = bus_fsclish("show networking address owner %s iface %s ip-address %s" %(name, interface, ip))  # need improve in future
    if re.search(ip, out):
        return False
    else:
        return True

def delete_instance(vrf, name, interface, ip, mask):
    bus_fsclish("delete networking instance %s address dedicated %s iface %s ip-address %s" %(vrf, name, interface, ip))
    out = bus_fsclish("show networking instace %s deleteress owner %s iface %s ip-deleteress %s/%s" \
                       %(vrf, name, interface, ip, mask))
    if re.search(ip, out):
        return False
    else:
        return True

def delete_instance_ip(vrf, node, interface, vid, vlanif, name, ip, mask):
    bus_fsclish("delete networking instance %s vlan %s iface %s" %(vrf, node, vlaniface))
    out = bus_fsclish("show networking instance %s vlan owner %s realiface %s vid %s" %(vrf, node, interface, vid))
    if not re.search(vlanif, out):
        return False
    return delete_instace(vrf, name, interface, ip, mask)

def staticRoute_OK(node, subnet, gw):
    if re.search("\.", gw):
        cmd = "show routing node %s static-route config" %(node)
    else:
        if subnet != "default":
            subnet = ":".join(i.rjust(4,"0") for i in subnet.split("/")[0].split(":")) + "/" + subnet.split("/")[1] 
        gw = ":".join(i.rjust(4,"0") for i in gw.split(":"))  # SCLI command: "show routing" will expand 0
        cmd = "show routing node %s ipv6 static-route config" %(node)
        print "subnet is %s, gw is %s" %(subnet, gw)
    out = bus_fsclish(cmd)
    if re.search(subnet, out):
        out = out.split(subnet)[1]
        if re.search(gw, out):
            s1 = out.split(gw)[0]
            if -1 != s1.find("Static route"):
                return False
            else:
                return True
    return False



def add_CLAroute(subnet,gw): # for simple purpose to config OAM route
    if re.search("\.", gw):
        bus_fsclish("set routing node CLA-0 static-route %s nexthop gateway address %s on" %(subnet, gw))
        bus_fsclish("set routing node CLA-1 static-route %s nexthop gateway address %s on" %(subnet, gw))
    else:
        bus_fsclish("set routing node CLA-0 ipv6 static-route %s nexthop gateway  %s on" %(subnet, gw))
        bus_fsclish("set routing node CLA-1 ipv6 static-route %s nexthop gateway  %s on" %(subnet, gw))

    if staticRoute_OK("CLA-0", subnet, gw) and staticRoute_OK("CLA-1", subnet, gw):
        return True
    else:
        return False

def delete_CLAroute(subnet,gw):# for simple purpose to config OAM route
    if re.search("\.", gw):
        bus_fsclish("set routing node CLA-0 static-route %s nexthop gateway address %s off" %(subnet, gw))
        bus_fsclish("set routing node CLA-1 static-route %s nexthop gateway address %s off" %(subnet, gw))
    else:
        bus_fsclish("set routing node CLA-0 ipv6 static-route %s nexthop gateway  %s off" %(subnet, gw))
        bus_fsclish("set routing node CLA-1 ipv6 static-route %s nexthop gateway  %s off" %(subnet, gw))

    if staticRoute_OK("CLA-0", subnet, gw) or staticRoute_OK("CLA-1", subnet, gw):
        return False
    else:
        return True

#Since the fourth parameter is useless for CP route but usable for L3 UP route, there are 2 add_route funcations.
def add_route(node, subnet, gw, *other): #other for BFD parameter when UP L3 route is added
    other_opition = ""
    for i in other:
        other_opition += i

    if re.search("\.", gw):
        cmd = "set routing node %s static-route %s nexthop gateway address %s %s on" %(node, subnet, gw, other_opition)
    else:
        cmd = "set routing node %s ipv6 static-route %s nexthop gateway  %s %s on" %(node, subnet, gw, other_opition)

    bus_fsclish(cmd)

    if staticRoute_OK(node, subnet, gw):
        return True
    else:
        return False

def add_CProute(node, subnet, gw, *other): #other is the extra info which is used to indicate this route in valid on fi0 or fi1, not used in the funcation
    if re.search("\.", gw):
    	bus_fsclish("set routing node %s static-route %s nexthop gateway address %s on" %(node, subnet, gw))
    else:
        bus_fsclish("set routing node %s ipv6 static-route %s nexthop gateway  %s on" %(node, subnet, gw))
    if staticRoute_OK(node, subnet, gw):
        return True
    else:
        return False
        
def delete_route(node, subnet, gw, *other): #other for the delete_route's extra parameter in add_CP
    if re.search("\.", gw): 
        bus_fsclish("set routing node %s static-route %s nexthop gateway address %s off" %(node, subnet, gw))
    else:
        bus_fsclish("set routing node %s ipv6 static-route %s nexthop gateway %s off" %(node, subnet, gw))
    if staticRoute_OK(node, subnet, gw):
        return False
    else:
        return True



def pingable_from_node(node, iface, ip):
    if re.search("\.", ip):
        cmd = "ssh %s -oUserKnownHostsFile=/dev/null -oStrictHostKeyChecking=no -oConnectTimeout=10 \"ping -I %s %s -c 3\"" \
              %(node, iface, ip)
    else:
        cmd = "ssh %s -oUserKnownHostsFile=/dev/null -oStrictHostKeyChecking=no -oConnectTimeout=10 \"ping6 -I %s %s -c 3\"" \
              %(node, iface, ip)
    out = bus_cli(cmd)
    if re.search('Unreachable', out):
        return False
    elif re.search('time=', out):
        return True
    else:
        return False


def pingable_from_ip(node, src, dst):
    if re.search("\.", src):
        cmd = "ssh %s -oUserKnownHostsFile=/dev/null -oStrictHostKeyChecking=no -oConnectTimeout=10 \"ping -I %s %s -c 3\"" %(node, src, dst)
    else:
        cmd = "ssh %s -oUserKnownHostsFile=/dev/null -oStrictHostKeyChecking=no -oConnectTimeout=10 \"ping6 -I %s %s -c 3\"" %(node, src, dst)
    out = bus_cli(cmd)
    if re.search('Unreachable', out):
        return False
    elif re.search('time=', out):
        return True
    else:
        return False

def pingable(iface, ip):
    if re.search("\.", ip):
        cmd = 'ping -I ' + iface + ' ' + ip + ' -c 3'
    else:
        cmd = 'ping6 -I ' + iface + ' ' + ip + ' -c 3'
    out = bus_cli(cmd)
    if re.search('Unreachable', out):
	return False
    elif re.search('time=', out):
	return True
    return False

def cluster_name():
    return bus_cli('cat /etc/cluster-id').strip()

def current_cla():
    out = bus_cli('hascli -s /CLA-0/FSSSHServer')
    if re.search('role\(ACTIVE\)', out):
	return 'CLA-0'
    return 'CLA-1'

def is_node_available(node):
    cmd = 'hwcli -e ' + node	
    out = bus_cli(cmd)
    if re.search('available', out):
	return True
    return False

def cut_log(start_time, log_file):
    cmd = "sed -n '/" + start_time + "/,//p'" + " " + log_file
    out = os.popen(cmd).read()
    return out

def cut_syslog(start_time):
    log_file = '/var/log/master-syslog'
    cmd = "sed -n '/" + start_time + "/,//p'" + " " + log_file
    out = os.popen(cmd).read()
    return out

def get_now_time_from_syslog():
    log_file = '/var/log/master-syslog'
    end_line = os.popen('tail -n 1 ' + log_file).read()
    now_time = end_line.split()[2]
    return now_time

def cut_syslog_from_now_to_end():
    log_file = '/var/log/master-syslog'
    now_time = get_now_time_from_syslog()
    out = cut_log(now_time, log_file)
    return out 

def clear_sgw():
    return lib_sgw_delete.clear_all()

def add_sgw():
    filename = get_paraFileName_for_different_cluster()
    return lib_sgw_add.main_add_sgw(filename)

def report_fail():
    if g_debug:
	bus_log('FAIL')
    else:
	verdict(VERDICT_FAIL)

def report_pass():
    if g_debug:
	bus_log('PASS')
    else:
	verdict(VERDICT_PASS)



#set parameter file name

def get_paraFileName_for_different_cluster():
    paraFileName = ""
    if g_debug:
        clusterID = os.popen('cat /etc/cluster-id ').read().strip()
        paraFileName = '/root/BUS/testsuite/Q1_2_configuration_add_and_delete/cnf/cnf_' + clusterID
    else:
        clusterID = cli('cat /etc/cluster-id ')
        paraFileName =  '/home/automation/cnf/cnf_' + clusterID
    return paraFileName     


def get_line(kw, file): #get the line including keyword from file
    cmd = "grep %s  %s|grep -v \"#\"" %(kw, file)
    line = os.popen(cmd).read()
    return line
