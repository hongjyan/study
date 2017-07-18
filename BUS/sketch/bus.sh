#!/bin/sh
STEP_0="Quit"
STEP_1="Setup ip"
STEP_2="Add license"
STEP_3="Prepare call"
STEP_4="Register"

NAME_0="Not in the list"
NAME_1="Anna"
NAME_2="Catherine"
NAME_3="Kaka"
NAME_4="Romeo"


get_mgw_name()
{
    echo "select the mgw to config:"
    select var in "$NAME_1" "$NAME_2" "$NAME_3" "$NAME_4" "$NAME_0"
    do
        case $var in
            $NAME_0)
		echo "plz check the mgw_setting.cnf"
		break;
            ;;
            $NAME_1)
		MGW_NAME=$NAME_1
		echo "using ${MGW_NAME} "
		break;
            ;;
            $NAME_2)
		MGW_NAME=$NAME_2
		echo "using ${MGW_NAME} "
		break;
            ;;
            $NAME_3)
		MGW_NAME=$NAME_3
		echo "using ${MGW_NAME} "
		break;
            ;;
            $NAME_4)
		MGW_NAME=$NAME_4
		echo "using ${MGW_NAME} "
		break;
            ;;
            *)
		MGW_NAME=""
		echo "plz select the MGW name"
            ;;
        esac
    done
}

bus_bot()
{
    select var in "$STEP_1" "$STEP_2" "$STEP_3" "$STEP_4" "$STEP_0"
    do
    break;
    done
    case $var in
        $STEP_0)
    break
        ;;
        $STEP_1)
    	sh 1_bu-quick-setting.sh $MGW_NAME
    	echo "------"
    	echo 'IP had been setted, you can go to Step 2.'
        ;;
        $STEP_2)
    	sh 2_add_license.sh $MGW_NAME
    	echo "------"
    	echo 'License had been added, you can go to Step 3.'
        ;;
        $STEP_3)
    	sh 5_prepare_call.sh $MGW_NAME
    	echo "------"
    	echo 'Call prepared, you can go to Step 4.'
        ;;
        $STEP_4)
    	sh 6_register.sh $MGW_NAME
    	echo "------"
    	echo 'Finished.'
    	break;
        ;;
        *)
    	echo "plz select sth"
        ;;
    esac
}

main()
{
    echo "---------------Welcome to BUS------------------"
    echo ""
    get_mgw_name
    while true
    do
	bus_bot
    done
}

main $@
