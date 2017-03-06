import subprocess
import sys            
if __name__ == "__main__":
    cmd = 'rm -rf /tmp/pet/MO/*'
    subprocess.call(cmd,shell=True)
    ip = sys.argv[1]
    start_port = int(sys.argv[2])
    end_port = int(sys.argv[3])
    while start_port < end_port:
#        cmd = 'python /home/automation/BoA/miscfiles/tools/test_perfomance/MO_1.py "%s" "%s"&'% (ip,str(start_port))
        cmd = './msrp_server.out "%s" "%s" "100" "msrp_server.cpp"&'% (ip,str(start_port))
        subprocess.call(cmd,shell=True)
        #port = port+1  #solution-1
        start_port = start_port+100 #solution-2
        print cmd
