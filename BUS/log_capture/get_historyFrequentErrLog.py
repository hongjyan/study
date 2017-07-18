#!/usr/bin/python
import re
import os
import sys
def is_pattern_exist_in_patternFile(pattern, handle_patternFile):
	handle_patternFile.seek(0)
        if handle_patternFile.read().find(pattern) != -1:
                return 1
        else:
                return 0


def wirte_allErrPrbFromSyslog_toFile(regexp,syslog,errPrb):
	handle_syslog = open(syslog, 'r')
	handle_errPrb = open(errPrb, 'a+')
	for In in handle_syslog:
		match = re.search(regexp, In)
		if match:
			handle_errPrb.write(In.split(' ')[5])
			handle_errPrb.write('\n')
	handle_syslog.close()
	handle_errPrb.close()
	return errPrb

	

def fliter_frequentErrPrb(errPrb, occurTime, frequentErrPrb):
	handle_errPrb = open(errPrb, 'r')
	handle_frequentErrPrb = open(frequentErrPrb, 'a+')
	list_errPrb = handle_errPrb.readlines()
	list_errPrb.sort()
	count = 0
	Dic_frequentErrPrb = {}
	while len(list_errPrb):
		count = list_errPrb.count(list_errPrb[0])
		if count < occurTime:
			del list_errPrb[0:count]
		else:
			handle_frequentErrPrb.write(list_errPrb[0])
			Dic_frequentErrPrb[list_errPrb[0].strip('\n')] = count
			del list_errPrb[0:count]
	
	handle_errPrb.close()
	handle_frequentErrPrb.close()
	return Dic_frequentErrPrb
			
			
		
			
def write_frequentErrLog(frequentErrPrb, frequentErrLog, syslog, regexp_shell):
	handle_syslog = open(syslog, 'r')
	handle_frequentErrLog = open(frequentErrLog, 'a+')
	handle_frequentErrPrb = open(frequentErrPrb, 'r')
	list_frequentErrPrb = handle_frequentErrPrb.readlines()
	for prb in list_frequentErrPrb:
		prb = prb.strip('\n')
		print "%s: %d" %(prb,Dic[prb])
		handle_frequentErrLog.write(os.popen("awk '/%s/{if($6==\"%s\") print $0}' %s" %(regexp_shell,prb,syslog)).read().strip('\n'))
		handle_frequentErrLog.write("\n*****************%s err occurs %d time**********************\n\n\n" %(prb,Dic[prb]))
	handle_syslog.close()
	handle_frequentErrLog.close()
	handle_frequentErrPrb.close()
	return frequentErrLog
		 
		

			 
if __name__  == "__main__":
	syslog = "/var/log/master-syslog"
	errPrb = "/tmp/historyErrPrb"
	frequentErrPrb = "/tmp/historyFrequentErrPrb"
	frequentErrLog = "/var/log/historyFrequentErrLog"
	regexp = '\\b' + ' err ' + '\\b' + '|' + '\\b' + ' crit ' + '\\b'
	regexp_shell = ' err | crit '
	occurTime = 10

	open(errPrb, 'w').close() 
	open(frequentErrPrb, 'w').close()     
	open(frequentErrLog, 'w').close()
	
	wirte_allErrPrbFromSyslog_toFile(regexp, syslog, errPrb)
	Dic = fliter_frequentErrPrb(errPrb, occurTime, frequentErrPrb)
	write_frequentErrLog(frequentErrPrb, frequentErrLog, syslog, regexp_shell)	 
