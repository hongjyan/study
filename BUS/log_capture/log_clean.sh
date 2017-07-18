get_available_nodes()
{
    local nodes=(`hascli -s /$1-* | xargs | awk '{gsub(/\//,"\n",$0); print $0}' | grep UNLOCKED | grep ENABLED | awk -F ":" '{print $1}'`)
    echo ${nodes[*]}
}

clear_log_in_other_nodes()
{
    local nodes=(`get_available_nodes "$1"`)
    echo "clean log in nodes: ${nodes[*]}"
    if [ ! -z $nodes ]
    then
	for nd in ${nodes[*]}
	do
	    ssh  -q -o "StrictHostKeyChecking no" $nd "rm -rf *.log;rm -rf *.cap"
	done
    fi
}


main()
{
    rm -rf *.txt
    rm -rf *.MON
    rm -rf *.cap
    rm -rf *.pid
    rm -rf *.bin
    rm -rf *.log
    rm -rf *.tar.gz
    rm -rf tmp_log

    clear_log_in_other_nodes "TCU"

    case $1 in
    ip)
	clear_log_in_other_nodes "IPNI1P"
    ;;
    tdm)
	clear_log_in_other_nodes "TDMNIP"
    ;;
    all)
	clear_log_in_other_nodes "IPNI1P"
	clear_log_in_other_nodes "TDMNIP"
    ;;
    none)
    ;;
    *)
	echo "unkown errors!"
    ;;
    esac

    echo "monster and tail process check"
    ps -ef |grep monster
    ps -ef |grep tail
}

main $@
