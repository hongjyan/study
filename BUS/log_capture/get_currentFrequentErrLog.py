#!/usr/bin/python
import re
import os
import sys
import time
def is_pattern_exist_in_patternFile(pattern, handle_patternFile):
	handle_patternFile.seek(0)
        if handle_patternFile.read().find(pattern) != -1:
                return 1
        else:
                return 0


def write_allErrPrbFromSyslog_toFile(regexp,syslog,errPrb):
	handle_syslog = open(syslog, 'r')
	handle_syslog.seek(0)
	handle_errPrb = open(errPrb, 'a+')
	for In in handle_syslog:
		match = re.search(regexp, In)
		if match:
			handle_errPrb.write(In.split()[5])
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
			
			
		
			
def write_frequentErrLog(frequentErrPrb, frequentErrLog, syslog, regexp_shell, Dic):
	handle_syslog = open(syslog, 'r')
	handle_syslog.seek(0)
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
	os.popen("mkdir /var/log/capture")
	tmpsyslog = "/var/log/capture/currentsyslog"
	errPrb = "/var/log/capture/currentErrPrb"
	critPrb = "/var/log/capture/curretCritPrb"
	frequentErrPrb = "/var/log/capture/currentFrequentErrPrb"
	frequentCritPrb = "/var/log/capture/currentFrequentCritPrb"
	frequentErrLog = "/var/log/capture/currentFrequentErrLog"
	regexp = '\\b' + ' err ' + '\\b' + '|' + '\\b' + ' warn ' + '\\b'
	regexp_shell = ' err | warn '
	regexp2 = '\\b' + ' crit ' + '\\b'
	regexp2_shell = ' crit '
	occurTime = 10
	occurTime2 = 3


	os.system("tail -f %s>%s&" %(syslog, tmpsyslog))

	while True:
		try:	
			open(errPrb, 'w').close()
                        open(critPrb, 'w').close()
                        open(frequentErrPrb, 'w').close()
                        open(frequentCritPrb, 'w').close()
			open(frequentErrLog, 'w').close()
			write_allErrPrbFromSyslog_toFile(regexp, tmpsyslog, errPrb)
			write_allErrPrbFromSyslog_toFile(regexp2, tmpsyslog, critPrb)
			Dic1 = fliter_frequentErrPrb(errPrb, occurTime, frequentErrPrb)
			Dic2 = fliter_frequentErrPrb(critPrb, occurTime2, frequentCritPrb)
			write_frequentErrLog(frequentCritPrb, frequentErrLog, tmpsyslog, regexp2_shell, Dic2)
			write_frequentErrLog(frequentErrPrb, frequentErrLog, tmpsyslog, regexp_shell, Dic1)
			print "\n\n"
			time.sleep(30)
		except:
			print "type keyboardInterrupt,exit(0)"
			open(tmpsyslog, 'w').close()
			pid = os.popen("ps -ef|grep tail").read().split()[1]
			if pid:
				os.system("kill %s" %pid)
			sys.exit(0)
	pid = os.popen("ps -ef|grep tail").read().split()[1]
	if pid:
        	os.system("kill %s" %pid)
	
