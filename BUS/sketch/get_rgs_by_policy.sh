#!/bin/bash
#-----------------------------------------------------------------------------------------
# filename : list rgs by given the recovery policy type
#
# Initial version : Zhou, Tong (NSN - CN/Shanghai); Huang, Yikun (EXT-Other - CN/Hangzhou)
#-----------------------------------------------------------------------------------------

tips()
{
    echo "Usage: ./get_rgs_by_policy.sh all"
    echo "	    list all rgs' recovery policy "
    echo "Usage: ./get_rgs_by_policy.sh ColdActiveStandby"
    echo "	    list all rgs whose recovery policy is ColdActiveStandby"
    echo "Usage: ./get_rgs_by_policy.sh HotActiveStandy"
    echo "	    list all rgs whose recovery policy is HotActiveStandby"
    echo "Usage: ./get_rgs_by_policy.sh ColdOnePlusM"
    echo "	    list all rgs whose recovery policy is ColdOnePlusM"
    echo "Usage: ./get_rgs_by_policy.sh NoRedundancy"
    echo "	    list all rgs whose recovery policy is NoRedundancy"
    echo "Usage: ./get_rgs_by_policy.sh --help"
    echo "	    print this message"
}

get_rg_type()
{
    rg=$1
    type=`fsclish -c "show config fsClusterId=ClusterRoot fsFragmentId=HA fsFragmentId=RecoveryGroups fshaRecoveryGroupName=$1" | grep fshaRecoveryPolicy | awk -F ": " '{print $2}'`
    echo $type
}

setup_rgs()
{
    rgs=(`fshascli --bare rg | awk -F "/" '{print $2}' | xargs`)
    plc=$1
    case $plc in
        ColdActiveStandby)
	    for i in "${rgs[@]}"
    	    do
    	        policy=`get_rg_type $i`
    	        if [ "$plc" == "$policy" ]
    	        then
		    echo -e "${policy}\t\t\t${i}\t"
	        fi
    	    done
        ;;
        ColdOnePlusM)
	    for i in "${rgs[@]}"
    	    do
    	        policy=`get_rg_type $i`
    	        if [ "$plc" == "$policy" ]
    	        then
		    echo -e "${policy}\t\t\t${i}\t"
	        fi
    	    done
        ;;
        HotActiveStandby)
	    for i in "${rgs[@]}"
    	    do
    	        policy=`get_rg_type $i`
    	        if [ "$plc" == "$policy" ]
    	        then
		    echo -e "${policy}\t\t\t${i}\t"
	        fi
    	    done
        ;;
        NoRedundancy)
	    for i in "${rgs[@]}"
    	    do
    	        policy=`get_rg_type $i`
    	        if [ "$plc" == "$policy" ]
    	        then
		    echo -e "${policy}\t\t\t${i}\t"
	        fi
    	    done
        ;;
        all)
	    for i in "${rgs[@]}"
    	    do
    	        policy=`get_rg_type $i`
		echo -e "${policy}\t\t\t${i}\t"
    	    done
        ;;
        *)
	   tips
        ;;
    esac
}

main()
{
    setup_rgs $1
}

main $@


