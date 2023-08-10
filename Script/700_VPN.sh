#!/bin/bash

clear
clear
clear

echo "==============================================================="

function vpn_start(){
	if [ "$1" = "start" ]; then
		echo "FortiCleintVPN - Start"
		sudo ipsec up fortinet
		sudo ipsec up hmc
	elif [ "$1" = "stop" ]; then
		echo "FortiCleintVPN - Stop"
		sudo ipsec down hmc
		sudo ipsec down fortinet
	else
		echo "[Error] Input String !!!!!!!!!!!!!!!"
		echo "	./VPN.sh start"
		echo "	./VPN.sh stop"
	fi
	
	echo "==============================================================="
}


#VPN Exec
case "$1" in
	start)
		vpn_start $1
		;;
	stop)
		vpn_start $1
		;;
	*)
		vpn_start Error
		;;
esac
		
