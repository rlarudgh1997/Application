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

#/opt/fsl-imx-xwayland/4.19-warrior
function setEnvironments(){
	unset LD_LIBRARY_PATH
	
	if [ "$1" = target ] || [ "$1" = t ]; then
		source $SDK_ENVIROMENT_PATH
	elif [ "$1" = host ] || [ "$1" = h ]; then
		SET_TARGET_BUILD=0
		SET_PLATFORM=x86
		export PATH=$QT_PATH:$PATH
	else
		echo "[setEnvironments] fail !!!!!!!!!!!!!!!!!!!!"
		exit
	fi
	
	SET_DEPLOY=deploy_$SET_PLATFORM
	SET_INSTALL_PATH=$PROJECT_DIR/$SET_DEPLOY

	echo
	echo "[setEnvironments]"
	echo "   PROJECT_NAME=$SET_PROJECT_NAME"
	echo "   PLATFORM=$SET_PLATFORM"
	echo "   TARGET_BUILD=$SET_TARGET_BUILD"
	echo "   INSTALL_PATH=$SET_INSTALL_PATH"
	echo "   QT_PATH=$QT_PATH"
	echo "   SDK_ENVIROMENT_PATH=$SDK_ENVIROMENT_PATH"
	echo "==================================================================================================================="
	echo

}

function platformClean(){
    if [ -f "Makefile" ]; then
        make distclean
    fi
	
    find -name "debug" -type d -exec rm -rfv {} \;
    find -name "release" -type d -exec rm -rfv {} \;
    find -name "build" -type d -exec rm -rfv {} \;
}

function cleanAll(){
    #find -name "deploy_target" -type d -exec rm -rfv {} \;
    #find -name "deploy_x86" -type d -exec rm -rfv {} \;	

	#find -name $PROJECT_DIR/deploy_target -type d -exec rm -rfv {} \;
	#find -name $PROJECT_DIR/deploy_x86 -type d -exec rm -rfv {} \;

	rm -rf $PROJECT_DIR/deploy_target
	rm -rf $PROJECT_DIR/deploy_x86
	

	platformClean
}

function buildAll(){
	echo 
	echo "==================================================================================================================="
	mkdir $PROJECT_DIR/$SET_DEPLOY

	#echo "Move $PROJECT_DIR/$SET_PROJECT_NAME"

	if [ "$1" = "debug" ] || [ "$1" = "d" ]; then
		CMD_TEXT="qmake CONFIG+=$1 CONFIG+=qml_debug TARGET_BUILD=$SET_TARGET_BUILD INSTALL_PATH=$SET_INSTALL_PATH PLATFORM=$SET_PLATFORM $SET_PROJECT_NAME.pro -recursive"
	else
		CMD_TEXT="qmake CONFIG+=$1 TARGET_BUILD=$SET_TARGET_BUILD INSTALL_PATH=$SET_INSTALL_PATH PLATFORM=$SET_PLATFORM  $SET_PROJECT_NAME.pro -recursive"
	fi

	echo CMD_TEXT=$CMD_TEXT
	echo 
	echo "==================================================================================================================="

	$CMD_TEXT
	make -j8
	make install
	
}



function build(){
	pushd $PROJECT_DIR/$SET_PROJECT_NAME
		if [ "$1" = "c" ] || [ "$1" = "clean" ]; then
			cleanAll
		else
			setEnvironments $1

			case "$2" in
				#clean
				c)
					platformClean
					;;
				clean)
					platformClean
					;;
				#debug
				d)
					buildAll $2 $3
					;;
				debug)
					buildAll $2 $3
					;;
				#relrese
				r)
					buildAll $2 $3
					;;
				release)
					buildAll $2 $3
					;;
				#default
				*)
					echo
					echo "==================================================================================================================="
					echo "[Command Error]"
					echo "   INPUT_CMD=$0 $1 $2 $3"
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
