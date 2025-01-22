#!/bin/bash

BASE_DIR="$(dirname $(realpath $0))"
APP_PATH="$BASE_DIR/.."
BIN_NAME="Application"

# SDK_HOST=/opt/sfc/PV/environment-setup-sfc
SDK_HOST=$CCOS_LIB_DIR/environment-setup-sfc

ARGUMENTS=""

clear
clear
clear

echo "=========================================================================================================="


function setAuth(){
	chmod -R 755 $BASE_DIR
}

function setEnvironments(){
	clear
	clear

	if [ "$1" = target ] || [ "$1" = t ]; then
		echo "[Target]"
		APP_PATH=/home/root/App
	elif [ "$1" = xserver ] || [ "$1" = xs ]; then
		if [ "$2" = "" ]; then
			echo "[Host - XServer : WSL]"
			export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0.0 # in WSL 2
			#export DISPLAY=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}'):0
			#export DISPLAY=$(grep -oP "(?<=nameserver ).+" /etc/resolv.conf):0
		else
			echo "[Host - XServer : $2]"
			#export DISPLAY=10.45.143.71:0
			export DISPLAY=$2:0
		fi

		APP_PATH=$APP_PATH/deploy_x86
	elif [ "$1" = host ] || [ "$1" = h ]; then
		echo "[Host]"
		if [ "$BIN_NAME" = altonservice ]; then
			source $SDK_HOST CV
			APP_PATH=$CCOS_LIB_DIR/bin
			# APP_PATH=$../Alton
		elif [ "$BIN_NAME" = Cluster ]; then
			source $SDK_HOST PV
			unset $ALTON_HAL_SCRIPT_PATH
			APP_PATH=$APP_PATH/deploy_x86
		else
			APP_PATH=$APP_PATH/deploy_x86
		fi
	else
		echo "[setEnvironments] fail - $1"
		exit
	fi


	echo "SDK_HOST  = $SDK_HOST"
	echo "APP_PATH  = $APP_PATH"
	echo "=========================================================================================================="
	echo
}

function runProcess(){
	echo
	echo "=========================================================================================================="

	SERVICE="$1"
	FULL_PATH=""$2"/"$SERVICE""

	echo "SERVICE   = $SERVICE"
	echo "FULL_PATH = $FULL_PATH"
	echo "ARGUMENTS = $ARGUMENTS"

	if pgrep -x $SERVICE > /dev/null
	then
		echo "$SERVICE is already running"
		killall -9 "$SERVICE"
	fi

	if [ -f $FULL_PATH ]
	then
		echo "Start Launching [$SERVICE]"

		if [ "$3" = "" ]; then
			# $FULL_PATH &
			$FULL_PATH $ARGUMENTS &
		else
			# nohup $FULL_PATH > /dev/null 2>&1 &
			nohup $FULL_PATH $ARGUMENTS > /dev/null 2>&1 &
		fi

		sleep 0.1

		if pgrep -x $SERVICE > /dev/null
		then
			echo "Launching [$SERVICE] is completed"
		fi
	else
		echo "[$FULL_PATH] not FOUND!!"
	fi


	echo
	echo "=========================================================================================================="
}


function killProcess(){
	kill -9 `ps -ef | grep $BIN_NAME | awk '{print $2}'`
}


####################################################################################
####################################################################################
#Environments Setting

#Partition Read/Write Setting
#setAuth


#Application Exec
RUN_BACKGROND=""
if [ "$1" = target ] || [ "$1" = t ]; then
	PLATFORM=target
	ENV=target
elif [ "$1" = xserver ] || [ "$1" = xs ]; then
	PLATFORM=host
	ENV=xserver
	IP_ADDRESS=$2
elif [ "$1" = host ] || [ "$1" = h ]; then
	PLATFORM=host
	ENV=host

	# shift	# remove $1
	# ARGUMENTS="$*"	# set $2, $3, ...
	ARGUMENTS=("$@")

elif [ "$1" = xserverc ] || [ "$1" = xsc ]; then
	PLATFORM=host
	ENV=xserver
	IP_ADDRESS=$2
	BIN_NAME="Cluster"
elif [ "$1" = cluster ] || [ "$1" = c ]; then
	PLATFORM=host
	ENV=host
	BIN_NAME="Cluster"
elif [ "$1" = altonservice ] || [ "$1" = a ]; then
	PLATFORM=host
	ENV=host
	BIN_NAME="altonservice"
	RUN_BACKGROND="true"
else
	PLATFORM=$1
	ENV=$1
fi

case "$PLATFORM" in
	host)
		setEnvironments $ENV $IP_ADDRESS $BIN_NAME
		killProcess
		runProcess $BIN_NAME $APP_PATH $2 $RUN_BACKGROND
		;;
	target)
		setEnvironments $ENV
		killProcess
		runProcess $BIN_NAME $APP_PATH $2
		;;
	*)
		killProcess
		;;
esac

echo "./run.sh xs 10.45.143.71"
echo "./run.sh xsc 10.45.143.71"
echo
CURRENT_DATE_TIME=$(date +"%Y-%m-%d %H:%M:%S.%3N")
echo "CURRENT_DATE_TIME = $CURRENT_DATE_TIME"
echo "=========================================================================================================="
echo
echo
