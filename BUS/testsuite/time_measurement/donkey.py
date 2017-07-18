import re
import time
import datetime

g_debug=True
#g_debug=False

if g_debug:
    import sys
    sys.path.append('..')
    sys.path.append('../lib')
    from lib.boa_simulate import * 	

lib_Base = boaimport('BU_Base_lib')


def log(str):
    return lib_Base.log(str)

def cli(str):
    return lib_Base.cli(str)


def clear_tmp_files(dir):
    cmd = 'rm -rf '+ dir
    cli(cmd)
    time.sleep(1)

def mk_tmp_dir(dir):
    cmd = 'mkdir '+dir
    cli(cmd)

def kill_process():
    cmd = 'killall -9 tail'
    cli(cmd)

def remove_node_from_list(node, nodes):
    if node in nodes:
	nodes.remove(node)

def another_pair_of_node(node):
    if node == 'IPNI1P-0':
	return 'IPNI1P-1'
    if node == 'IPNI1P-1':
	return 'IPNI1P-0'
    if node == 'IPNI1P-3':
	return 'IPNI1P-4'
    if node == 'IPNI1P-4':
	return 'IPNI1P-3'
    if node == 'CLA-0':
	return 'CLA-1'
    if node == 'CLA-1':
	return 'CLA-0'
    if node == 'TDMNIP-4':
	return 'TDMNIP-5'
    if node == 'TDMNIP-5':
	return 'TDMNIP-4'
    if node == 'TDMNIP-6':
	return 'TDMNIP-7'
    if node == 'TDMNIP-7':
	return 'TDMNIP-6'

def get_restartable_rus(node):
    rus_a = lib_Base.GetRUsByNodeWithActiveRole(node)
    rus_a = lib_Base.shorten(rus_a)
    print rus_a
    if re.search('TCU-', node):
	return rus_a
    other_node = another_pair_of_node(node)
    rus_h = lib_Base.GetRUsByNodeWithHotStandByRole(other_node)
    rus_h = lib_Base.shorten(rus_h)
    rus_c = lib_Base.GetRUsByNodeWithColdStandByRole(other_node)
    rus_c = lib_Base.shorten(rus_c)
    for h in rus_h:
        match = re.search('-\d+', h)
        if match:
            s_h = h.rstrip(match.group(0))
            for ru_a in rus_a:
                    if re.search(s_h, ru_a):
                        rus_a.remove(ru_a)
    for c in rus_c:
        if c in rus_a:
            rus_a.remove(c)
    print rus_a

    return rus_a


def log_time(start_time_text, end_time_text, mo):
    start_time = time.strptime(start_time_text,'%H:%M:%S')
    end_time = time.strptime(end_time_text,'%H:%M:%S')

    d = time.mktime(end_time) - time.mktime(start_time)
    duration_text = time.strftime('%H:%M:%S', time.gmtime(d))
    log(mo + ' restart duaration:' + str(duration_text))
    #log('from ' + start_time_text +' to ' + end_time_text)


def restart_node(nd, tmp_log):
    if not lib_Base.IsNodeAvailable(nd):
	log(nd + ' is not available')
	return

    if not lib_Base.IsMOEnabled('/'+nd):
	log('/'+nd + ' is not ENABLED')
	return

    rus = get_restartable_rus(nd)

    #cmd = 'tail -f /var/log/master-syslog| grep '+nd+' > '+tmp_log+' &'
    cmd = 'tail -f /var/log/master-syslog > '+tmp_log+' &'
    cli(cmd) 

    cmd = 'hascli -rn /' + nd
    output = cli(cmd)
    if not re.search('successfully', output):   
        log(output)

    keywords_start = 'Managed object \\\"restart\\\" operation initiated. Target=/' + nd
    start_time_text = ''
    end_time_text = ''

    d1 = datetime.datetime.now()
    d2 = datetime.datetime.now()
    start_time_already_get = False
    while (d2-d1).seconds < 60*20:
	#get the start timestamp
	if not start_time_already_get:
	    cmd = 'cat ' + tmp_log + ' | grep "' + keywords_start + '"'
	    output = cli(cmd) 
	    if output != "":
		cmd = 'cat '+ tmp_log + ' | grep "' + keywords_start + '"' + "  | awk '{print $3}' | awk -F '.' '{print $1}'"
		start_time_text = cli(cmd).rstrip()
		start_time_already_get = True
	    

	#get the end time
	go_out = False
	for ru in rus:
	    keywords_end_pass = 'Recovery unit \\/'+nd+'\\/'+ru+' is now ACTIV'
	    keywords_end_failed = 'Recovery unit \\/'+nd+'\\/'+ru+' is not currently startable as it requires services from'

	    cmd = 'cat ' + tmp_log + ' | grep "' + keywords_end_pass + '"'
	    output = cli(cmd) 
#	    log(cmd)
	    if output != "":
		if len(rus) == 1:
		    cmd = 'cat '+ tmp_log + ' | grep "' + keywords_end_pass + '"' + "  | awk '{print $3}' | awk -F '.' '{print $1}'"
		    end_time_text = cli(cmd).rstrip()
		    go_out = True
		    break;
		else:
		    cmd = 'cat '+ tmp_log + ' | grep "' + keywords_end_pass + '"' + "  | awk '{print $3}' | awk -F '.' '{print $1}'"
		    end_time_text = cli(cmd).rstrip()
		    log_time(start_time_text, end_time_text, '    /'+nd+'/'+ru)
		rus.remove(ru)
	    if re.search('CLA', nd):
	        cmd = 'cat ' + tmp_log + ' | grep "' + keywords_end_failed + '"'
	        output = cli(cmd) 
#	        log(cmd)
	        if output != "":
		   if len(rus) == 1:
	    	       cmd = 'cat '+ tmp_log + ' | grep "' + keywords_end_failed + '"' + "  | awk '{print $3}' | awk -F '.' '{print $1}'"
	    	       end_time_text = cli(cmd).rstrip()
	    	       go_out = True
	    	       break;
	    	   else:
	    	       cmd = 'cat '+ tmp_log + ' | grep "' + keywords_end_failed + '"' + "  | awk '{print $3}' | awk -F '.' '{print $1}'"
	    	       end_time_text = cli(cmd).rstrip()
	    	       log_time(start_time_text, end_time_text, '    /'+nd+'/'+ru)
	    	   rus.remove(ru)
	if go_out:
	    break

	time.sleep(10)
	d2 = datetime.datetime.now()
    else:
	log("node: "+nd+" restarting is out of time, more than 20 minuts, please check it manually!")
	return

    log_time(start_time_text, end_time_text, nd)


def restart_nodes():
    dir='.log'
    log_file = '.log/greped_syslog.log'

    nodes = lib_Base.GetAvailableNodes()
    if lib_Base.get_obj_role('CLA-0/FSSSHServer') == 'ACTIVE':
	remove_node_from_list('CLA-0',nodes)
    else:
	remove_node_from_list('CLA-1',nodes)

    for nd in nodes:
    	clear_tmp_files(dir)
    	mk_tmp_dir(dir)
    	restart_node(nd, log_file)
	kill_process()


def run():
    restart_nodes()
    return

    #rus = lib_Base.GetRUsByNodeWithActiveRole('IPNI1P-0')
    #log(rus)
    #log(len(rus))
    #rus = lib_Base.GetRUsByNodeWithActiveRole('IPNI1P-1')
    #log(rus)
    #log(len(rus))
    #rus = get_restartable_rus('IPNI1P-0')
    #log(rus)
    #log(len(rus))
    #rus = get_restartable_rus('IPNI1P-1')
    #log(rus)
    #log(len(rus))
    #return

    dir='.log'
    clear_tmp_files(dir)
    mk_tmp_dir(dir)
    restart_node('TCU-1', '.log/greped_syslog.log')
    kill_process()
    #clear_tmp_files(dir)
    #mk_tmp_dir(dir)
    #restart_node('TDMNIP-5', '.log/greped_syslog.log')
    #kill_process()

if __name__ == "__main__":
    #run()
    get_restartable_rus('CLA-0')
    #get_restartable_rus('TCU-1')
