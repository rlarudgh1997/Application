#!/bin/bash

BASE_DIR="$(dirname $(realpath $0))"
APP_PATH="$BASE_DIR/.."
BIN_NAME="Application"

clear
clear
clear

echo "=========================================================================================================="

function setWayland(){
	usleep 60000
	while ! [ -f /tmp/wayland-0.lock ]; do
		echo "waiting for initializing weston..."
		usleep 10000
	done

	export XDG_RUNTIME_DIR=/tmp
	export $(cat /tmp/dbus_session)
	export QT_QPA_PLATFORM=wayland

	echo "setWayland : $setWayland"
	echo
}

function setMount(){
	mount -o remount,rw /
}

function setAuth(){
	chmod -R 755 $BASE_DIR
	sync
}

function setEnvironments(){
	clear
	clear

	if [ "$1" = target ] || [ "$1" = t ]; then
		export LANG=ko_KR.UTF-8
		export QT_GSTREAMER_CAMERABIN_VIDEOSRC=imxv4l2src
		APP_PATH=/home/root/Tractor
	elif [ "$1" = wayland ]; then
		setWayland
		APP_PATH=$APP_PATH/deploy_target
	else
		echo "[Host PC]"
		APP_PATH=$APP_PATH/deploy_x86
	fi

	export LD_LIBRARY_PATH=/usr/lib:/opt/GENIVI/lib/:$APP_PATH/lib:$LD_LIBRARY_PATH

	echo "GSTREAMER=$QT_GSTREAMER_CAMERABIN_VIDEOSRC"
	echo "APP_PATH=$APP_PATH"
	echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
	echo "=========================================================================================================="
	echo
}

function runProcess(){
	echo
	echo "=========================================================================================================="

	if [ "$3" = "" ]; then
		runProcess2 $1 $2
	else
		SERVICE="$1"
		FULL_PATH=""$2"/"$SERVICE""

		echo "FULL_PATH_01=$FULL_PATH"

		if pgrep -x $SERVICE > /dev/null
		then
			echo "$SERVICE is already running"
			killall -9 "$SERVICE"
		fi

		if [ -f $FULL_PATH ]
		then
			echo "Launch $SERVICE !!!"
			nohup $FULL_PATH > /dev/null 2>&1 &

			sleep 0.1

		if pgrep -x $SERVICE > /dev/null
		then
			echo "Launching $SERVICE is completed"
		fi
		else
			echo "$FULL_PATH not FOUND!!"
		fi
	fi


	echo
	echo "=========================================================================================================="
}


function runProcess2(){
	echo
	echo "=========================================================================================================="

	SERVICE="$1"
	FULL_PATH=""$2"/"$SERVICE""

	echo "FULL_PATH_02=$FULL_PATH"

	killall -9 "$SERVICE"

	$FULL_PATH &#> /dev/null 2>&1 &

	echo
	echo "=========================================================================================================="
}

function killProcess(){
	kill -9 `ps -ef | grep "Tractor" | awk '{print $2}'`
}

function setEthernet(){
	ifconfig eth0 192.168.200.100
}


####################################################################################
####################################################################################
#Environments Setting

#Partition Read/Write Setting
#setMount
setAuth


#Application Exec


CMD=$1

if [ "$1" = h ] || [ "$1" = "" ]; then
	CMD=host
elif [ "$1" = t ]; then
	CMD=target
elif [ "$1" = c ]; then
	CMD=host
	BIN_NAME="Cluster"
else
	CMD=$1
fi

case "$CMD" in
	eth)
		setEthernet
		;;
	app)
		setEnvironments
		killProcess
		runProcess $BIN_NAME $APP_PATH $2 $3
		;;
	host)
		setEnvironments
		killProcess
		runProcess $BIN_NAME $APP_PATH $2 $3
		;;
	target)
		setEnvironments target
		killProcess
		runProcess $BIN_NAME $APP_PATH $2 $3
		;;
	demo)
		setEnvironments
		killProcess
		runProcess Demo $APP_PATH $2 $3
		;;
	*)
		killProcess
		;;
esac
