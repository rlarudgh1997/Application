#!/bin/bash

BASE_DIR="$(dirname $(realpath $0))"
PROJECT_DIR="$BASE_DIR/.."

QT_VERSION=5.12.4
QT_PATH=~/Qt_$QT_VERSION/$QT_VERSION/gcc_64/bin/
#QT_PATH=~/Qt$QT_VERSION/$QT_VERSION/gcc_64/bin/
SDK_ENVIROMENT_PATH=/opt/fsl-imx-xwayland/4.19-warrior/environment-setup-cortexa9hf-neon-poky-linux-gnueabi


SET_PLATFORM=target
SET_TARGET_BUILD=1
SET_DEPLOY=""
SET_PROJECT_NAME=Application
SET_INSTALL_PATH=""



clear
clear
clear

echo "==================================================================================================================="
echo "[How To Compile]"
echo "   ./build.sh [target/host/clean] [debug/release/clena] [n/new]"
echo "   ./build.sh [t/h/c] [d/r/c] [n/new]"
echo "==================================================================================================================="
echo
echo

function setEnvironments(){
	unset LD_LIBRARY_PATH

	if [ "$1" = target ] || [ "$1" = t ]; then
		source $SDK_ENVIROMENT_PATH
	elif [ "$1" = host ] || [ "$1" = h ]; then
		SET_TARGET_BUILD=0
		SET_PLATFORM=x86
		# export PATH=$QT_PATH:$PATH
	else
		echo "[setEnvironments] fail !!!!!!!!!!!!!!!!!!!!"
		exit
	fi

	SET_DEPLOY=deploy_$SET_PLATFORM
	SET_INSTALL_PATH=$PROJECT_DIR/$SET_DEPLOY

	echo
	echo "[Set Environments]"
	echo "   PROJECT_NAME        = $SET_PROJECT_NAME"
	echo "   PLATFORM            = $SET_PLATFORM"
	echo "   TARGET_BUILD        = $SET_TARGET_BUILD"
	echo "   INSTALL_PATH        = $SET_INSTALL_PATH"
	echo "   QT_PATH             = $QT_PATH"
	echo "   SDK_ENVIROMENT_PATH = $SDK_ENVIROMENT_PATH"
	echo "==================================================================================================================="
	echo
}

function platformClean(){
	if [ -f "Makefile" ]; then
		make distclean
	fi

	COMPILE_PATH_ARR=("debug" "release" "build" "Build")
	for COMPILE_PATH in "${COMPILE_PATH_ARR[@]}"; do
		find -name $COMPILE_PATH -type d -exec rm -rfv {} \;
	done
}

function cleanAll(){
	find "$PROJECT_DIR" -name "deploy_target" -type d -exec rm -rfv {} \;
	find "$PROJECT_DIR" -name "deploy_x86" -type d -exec rm -rfv {} \;

	platformClean
}

function buildAll(){
	START_TIME=`date +%s`

	echo
	echo "==================================================================================================================="
	mkdir $PROJECT_DIR/$SET_DEPLOY

	#echo "Move $PROJECT_DIR/$SET_PROJECT_NAME"

	if [ "$1" = "debug" ] || [ "$1" = "d" ]; then
		CMD_TEXT="qmake CONFIG+=$1 CONFIG+=qml_debug TARGET_BUILD=$SET_TARGET_BUILD INSTALL_PATH=$SET_INSTALL_PATH PLATFORM=$SET_PLATFORM $SET_PROJECT_NAME.pro -recursive"
	else
		CMD_TEXT="qmake CONFIG+=$1 TARGET_BUILD=$SET_TARGET_BUILD INSTALL_PATH=$SET_INSTALL_PATH PLATFORM=$SET_PLATFORM  $SET_PROJECT_NAME.pro -recursive"
	fi

	echo "CMD_TEXT = $CMD_TEXT"
	echo
	echo "==================================================================================================================="

	$CMD_TEXT
	make -j8
	make install

	echo
	echo
	echo "==================================================================================================================="
	END_TIME=`date +%s`
	ELAPSED_TIME=`expr $END_TIME - $START_TIME`

	echo "[Compile Time]"
	echo "   START   = $START_TIME"
	echo "   END     = $END_TIME"
	echo "   ELAPSED = $ELAPSED_TIME""s"""
	echo "==================================================================================================================="
	echo
	echo
}



function build(){
	pushd $PROJECT_DIR/$SET_PROJECT_NAME
		if [ "$1" = "c" ] || [ "$1" = "clean" ]; then
			cleanAll
		else
			# 환경 설정
			ENV=$1

			if [ "$ENV" = "" ]; then
				ENV=host
			fi

			setEnvironments $ENV


			# 빌드 설정
			CMD=$2

			if [ "$2" = c ]; then
				CMD=clean
			elif [ "$2" = d ]; then
				CMD=debug
			elif [ "$2" = r ] || [ "$1" = "" ]; then
				CMD=release
			else
				CMD=$2
			fi

			case "$CMD" in
				clean)
					platformClean
					;;
				debug)
					buildAll $CMD
					;;
				release)
					buildAll $CMD
					;;
				*)	#default
					echo
					echo "==================================================================================================================="
					echo "[Command Error]"
					echo "   INPUT_CMD=$0 $1 $2"
					echo "==================================================================================================================="
					echo
					;;
			esac
		fi
	popd
}

echo "==================================================================================================================="

build $1 $2 $3

echo
echo
echo "==================================================================================================================="
echo
echo
echo
