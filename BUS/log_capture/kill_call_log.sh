#!/bin/bash
PID_PATH="pid"
log_path="tmp_log"

get_available_nodes()
{
    local ip_nodes=(`hascli -s /$1-* | xargs | awk '{gsub(/\//,"\n",$0); print $0}' | grep UNLOCKED | grep ENABLED | awk -F ":" '{print $1}'`)
    echo ${ip_nodes[*]}
}

kill_ip_log_process()
{
    echo "kill ip log process ..."
    local ip_nodes=(`get_available_nodes "IPNI1P"`)
    if [ ! -z $ip_nodes ]
    then
	for nd in ${ip_nodes[*]}
	do
	    echo "kill all tcpdump and monster@$nd: result :"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "killall tcpdump;killall monster"
        echo $? 
        echo "wait for a while..."
	    sleep 3
        echo $?       

	    echo "scp tcpdump capture from $nd"
	    scp root@$nd:/root/$nd*.cap  ./$log_path/
	    echo $?
  
        echo "Clear tcpdump capture on $nd"
        ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd*.cap"
        echo $?

	    echo "scp DMX message from $nd"
	    scp root@$nd:/root/$nd*.bin  ./$log_path/
	    echo $?

        echo "Clear DMX message file on $nd"
        ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd*.bin"
        echo $?
	done
    fi
}

kill_tdm_log_process()
{
    echo "kill tdm log process ..."
    local tdm_nodes=(`get_available_nodes "TDMNIP"`)
    echo $tdm_nodes
    if [ ! -z $tdm_nodes ]
    then
	for nd in ${tdm_nodes[*]}
	do
	    echo "kill all tcpdump and monster@$nd: result :"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "killall tcpdump;killall monster"
	    echo $?
	    
        echo "wait for a while..."
        sleep 3
        echo $? 

	    echo "scp tcpdump capture from $nd"
	    scp root@$nd:/root/$nd*.cap  ./$log_path/
	    echo $?

        echo "Clear tcpdump capture on $nd"
        ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd*.cap"
        echo $?
	    
	    echo "scp message from $nd"
	    scp root@$nd:/root/$nd*.bin  ./$log_path/
	    echo $?

        echo "Clear DMX message file on $nd"
        ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd*.bin"
        echo $?
	done
    fi
}

kill_tcu_log_process()
{
    echo "kill tcu log process ..."
    local tcu_nodes=(`get_available_nodes "TCU"`)
    if [ ! -z $tcu_nodes ]
    then
	for nd in ${tcu_nodes[*]}
	do
	    echo "kill all tcpdump@$nd: result :"
	    ssh  -q -o "StrictHostKeyChecking no" $nd "killall tcpdump"
	    sleep 2
        echo $?

	    echo "scp tcpdump capture from $nd"
	    scp root@$nd:/root/$nd@fi*.cap ./$log_path/
	    echo $?

        echo "Clear tcpdump capture on $nd"
        ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd*.cap"        
	done
    fi
}

kill_common_log_process()
{
    echo "kill monster and log process on CLA-0"
    cat ./$PID_PATH/debug_log.pid  | xargs kill -9
    echo $?

    cat ./$PID_PATH/debug_bin_log.pid  | xargs kill -9
    echo $?

    echo "kill CLA-1 monster..."
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "killall monster"
    echo $?

    echo "scp monster from CLA-1 as CLA-1.bin"
    scp root@CLA-1:/root/CLA-1.bin  ./$log_path/CLA-1.bin
    echo $?

    echo "clear Monster capture file in CLA-1..."
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "rm -rf /root/CLA-1.bin"
    echo $?

    echo "stop ISU monster..."
    local isu_nodes=(`get_available_nodes "ISU"`)
    echo $isu_nodes
    if [ ! -z $isu_nodes ]
    then
        for nd in ${isu_nodes[*]}
        do
            echo "kill all monster@$nd: result :"
            ssh  -q -o "StrictHostKeyChecking no" $nd "killall monster"
            echo $?

            echo "scp monster from $nd as $nd.bin"
            scp root@$nd:/root/$nd.bin  ./$log_path/$nd.bin
            echo $?

            echo "Clear Monster files on $nd"
            ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd.bin"
            echo $?
        done
    fi

	
    cat ./$PID_PATH/h248_mon_0.pid  | xargs kill -9
    echo $?

    cat ./$PID_PATH/h248_mon_1.pid  | xargs kill -9
    echo $?

    echo "stop CLA-1 tcpdump capture..."
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "killall tcpdump"
    echo $?

    echo "scp tcpdump capture from CLA-1"
    scp root@CLA-1:/root/CLA-1-fi*.cap  ./$log_path/
    echo $?

    echo "Clear tcpdump files on CLA-1..."
    ssh  -q -o "StrictHostKeyChecking no" CLA-1 "rm -rf /root/CLA-1-fi*.cap"
    echo $?

    echo "kill ISU tcpdump..."
    local isu_nodes=(`get_available_nodes "ISU"`)
    echo $isu_nodes
    if [ ! -z $isu_nodes ]
    then
        for nd in ${isu_nodes[*]}
        do
            echo "kill all tcpdump@$nd: result :"
            ssh  -q -o "StrictHostKeyChecking no" $nd "killall tcpdump"
            echo $?

            echo "scp monster from $nd "
            scp root@$nd:/root/$nd-fi*.cap  ./$log_path/
            echo $?

            echo "Clear tcpdump files on $nd ..."
            ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf /root/$nd-fi*.cap"
            echo $?
        done
    fi
    
    echo "kill tail syslog process: result"
    cat ./$PID_PATH/msg_txt.pid  | xargs kill -9
    echo $?
    rm -rf $PID_PATH
}

tips()
{
    echo "usage: sh kill_call_log_process.sh all"
    echo "	 sh kill_call_log_process.sh ip"
    echo "	 sh kill_call_log_process.sh tdm"
    echo "	 sh kill_call_log_process.sh none"
}

main()
{
    case $1 in
    ip)
	kill_common_log_process
    	kill_ip_log_process
    	kill_tcu_log_process
    ;;
    tdm)
	kill_common_log_process
    	kill_tdm_log_process
    	kill_tcu_log_process
    ;;
    all)
	kill_common_log_process
    	kill_ip_log_process
    	kill_tdm_log_process
    	kill_tcu_log_process
    ;;
    none)
	kill_common_log_process
    	kill_tcu_log_process
    ;;
    *)
	tips
	exit 1
    ;;
    esac

    echo "running greppa"
    ./greppaa.sh $1

	echo "killall -9 monster"
	killall -9 monster

}

main $@
