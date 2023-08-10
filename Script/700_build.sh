#!/bin/bash -e

BASE_DIR="$(dirname $(realpath $0))"
PROJECT_DIR="$BASE_DIR/.."
MODULE_DIR=

SET_ENVIRONMENT_PATH_BASE=/opt/pony/2.10
SET_ENVIRONMENT_PATH=$SET_ENVIRONMENT_PATH_BASE/environment-setup-aarch64-gnu-linux

USE_MODULE_DISPLAY="display"
USE_MODULE_VARIANT="variant"
USE_MODULE_POWER="power"
USE_MODULE_SYSTEM="system"
USE_MODULE_VEHICLE="vehicle"

SET_MODULE=
SET_NAME_HLIB=
SET_NAME_SERVICE=
SET_NAME_HAL=
SET_NAME_HAL_MOBIS=
SET_NAME_HAL_STUB=
SET_NAME_HLIB_NEW=

SET_CMAKE_MODULE_HLIB=
SET_CMAKE_MODULE_SERVICE=
SET_CMAKE_MODULE_HAL=
SET_CMAKE_MODULE_HAL_MOBIS=
SET_CMAKE_MODULE=

SET_MAKE_MODULE=


clear
clear
clear

echo "==================================================================================================================="
echo "   =>  $PROJECT_DIR"
echo "   =>  ./build.sh [c/clean]"
echo "   =>  ./build.sh [display/variant/power/system/vehicle] [api/service/hal/hal-mobis/hal-stub] [clean/debug]"
echo "   =>  ./build.sh [d/v/p/s/ve] [a/s/h/hm/hs] [c/d]"
echo "==================================================================================================================="
echo


function setEnvironments(){
	#echo "==================================================================================================================="
	#echo "   =>  setEnvironments()"
	echo

	unset LD_LIBRARY_PATH
	source $SET_ENVIRONMENT_PATH	
}

function setBuildInfo(){
	#echo "==================================================================================================================="
	#echo "   =>  setBuildInfo()"
	#echo
	
	# Module Type - Set Directory
	if [ "$1" = $USE_MODULE_DISPLAY ] || [ "$1" = "d" ]; then
		SET_MODULE=$USE_MODULE_DISPLAY
		MODULE_DIR="200_Display"
	elif [ "$1" = $USE_MODULE_VARIANT ] || [ "$1" = "v" ]; then
		SET_MODULE=$USE_MODULE_VARIANT
		MODULE_DIR="210_Variant"
	elif [ "$1" = $USE_MODULE_POWER ] || [ "$1" = "p" ]; then
		SET_MODULE=$USE_MODULE_POWER
		SET_NAME_HLIB_NEW="lifecycle"
		MODULE_DIR="220_Power"
	elif [ "$1" = $USE_MODULE_SYSTEM ] || [ "$1" = "s" ]; then
		SET_MODULE=$USE_MODULE_SYSTEM
		MODULE_DIR="230_System"
	elif [ "$1" = $USE_MODULE_VEHICLE ] || [ "$1" = "ve" ]; then
		SET_MODULE=$USE_MODULE_VEHICLE
		MODULE_DIR="240_Vehicle"
	fi
	
	#echo "==================================================================================================================="
	if [ "$SET_MODULE" = "" ]; then
		echo "  SET_MODULE            = Error !!!!!!!!!!!!!!!!!!!!!"
		exit 0
	else
		# Enviroment Setting
		if [ "$SET_NAME_HLIB_NEW" != "" ]; then
			SET_NAME_HLIB=ccos.api.h"$SET_NAME_HLIB_NEW"
		else
			SET_NAME_HLIB=ccos.api.h"$SET_MODULE"
		fi
		
		SET_NAME_SERVICE=ccos.service."$SET_MODULE"service
		SET_NAME_HAL=ccos.hal.h"$SET_MODULE"hal
		SET_NAME_HAL_MOBIS=ccos.hal."$SET_MODULE"hal-mobis
		SET_NAME_HAL_STUB=ccos.hal."$SET_MODULE"hal-stub

		if [ "$2" = "a" ] || [ "$2" = "api" ]; then
			SET_MAKE_MODULE=$SET_NAME_HLIB
			SET_CMAKE_MODULE="-DCCOS_LIB_VERSION=2.0.8 -DCCOS_LIB_DIR=/ccos/lib -DCCOS_INC_DIR=/ccos/include/h"$SET_MODULE" -DCMAKE_INSTALL_PREFIX=/ccos/interfaces/h"$SET_MODULE""
		elif [ "$2" = "s" ] || [ "$2" = "service" ]; then
			SET_MAKE_MODULE=$SET_NAME_SERVICE
			SET_CMAKE_MODULE=-"DCCOS_LIB_VERSION=2.0.8 -DCCOS_SERVICE_DIR=/ccos/services/"$SET_MODULE"service -DCCOS_LIB_DIR=/ccos/lib -DCCOS_INC_DIR=/ccos/include/"$SET_MODULE"service"
		elif [ "$2" = "h" ] || [ "$2" = "hal" ]; then
			SET_MAKE_MODULE=$SET_NAME_HAL
			SET_CMAKE_MODULE="-DCCOS_LIB_VERSION=2.0.8 -DCCOS_LIB_DIR=/ccos/lib -DCCOS_INC_DIR=/ccos/include/h"$SET_MODULE"hal"
		elif [ "$2" = "hm" ] || [ "$2" = "hal-mobis" ]; then
			SET_MAKE_MODULE=$SET_NAME_HAL_MOBIS
			SET_CMAKE_MODULE="-DCCOS_LIB_VERSION=2.0.8 -DCCOS_MACHINE_NAME=mst6g -DCCOS_LIB_DIR=/ccos/lib -DCCOS_INC_DIR=/ccos/include/"$SET_MODULE"hal-mobis -DCMAKE_INSTALL_PREFIX=/ccos/interfaces/h"$SET_MODULE""
		elif [ "$2" = "hs" ] || [ "$2" = "hal-stub" ]; then
			SET_MAKE_MODULE=$SET_NAME_HAL_STUB
			SET_CMAKE_MODULE="-DCCOS_LIB_VERSION=2.0.8 -DCCOS_MACHINE_NAME=mst6g -DCCOS_LIB_DIR=/ccos/lib -DCCOS_INC_DIR=/ccos/include/"$SET_MODULE"hal-stub -DCMAKE_INSTALL_PREFIX=/ccos/interfaces/h"$SET_MODULE""
		else
			SET_MAKE_MODULE=""
			exit 0
		fi
	fi


	echo "==================================================================================================================="
	if [ "$3" = "c" ] || [ "$3" = "clean" ]; then
		# Clean
		rm -rf $PROJECT_DIR/$MODULE_DIR/$SET_MAKE_MODULE/build
		echo "  Clean                 = Complete"
		echo "==================================================================================================================="
		echo
		exit 0
	else
		# Debug Type
		if [ "$3" = "d" ] || [ "$3" = "debug" ]; then
			SET_CMAKE_MODULE=$SET_CMAKE_MODULE" -DCCOS_BUILD_TYPE=debug"
		else
			SET_CMAKE_MODULE=$SET_CMAKE_MODULE" -DCCOS_BUILD_TYPE=release"		
		fi

		echo "  SET_ENVIRONMENT_PATH  = $SET_ENVIRONMENT_PATH"
		echo "  MODULE_DIR            = $MODULE_DIR"
		echo "  SET_MODULE            = $SET_MODULE"
		echo "  SET_NAME_HLIB         = $SET_NAME_HLIB"
		echo "  SET_NAME_SERVICE      = $SET_NAME_SERVICE"
		echo "  SET_NAME_HAL          = $SET_NAME_HAL"
		echo "  SET_NAME_HAL_MOBIS    = $SET_NAME_HAL_MOBIS"
		echo "  SET_NAME_HAL_STUB     = $SET_NAME_HAL_STUB"
		echo "  SET_MAKE_MODULE       = $SET_MAKE_MODULE"
		echo "  SET_CMAKE_MODULE      = $SET_CMAKE_MODULE"

		
		cd $PROJECT_DIR/$MODULE_DIR/$SET_MAKE_MODULE
		if [ -e $PROJECT_DIR/$MODULE_DIR/$SET_MAKE_MODULE/build ]; then
			echo "  BuildPath             = Exists"
		else
			echo "  BuildPath             = Create !!!!!!!!!!!!!!!!!!!!!"
			mkdir $PROJECT_DIR/$MODULE_DIR/$SET_MAKE_MODULE/build
		fi
		
		echo "==================================================================================================================="
		cd $PROJECT_DIR/$MODULE_DIR/$SET_MAKE_MODULE/build
		if [ -e $PROJECT_DIR/$MODULE_DIR/$SET_MAKE_MODULE/build/Makefile ]; then
			echo "  Makefile              = Exists"
		else
			cmake .. $SET_CMAKE_MODULE
			echo
			echo "  Makefile              = Create !!!!!!!!!!!!!!!!!!!!!"
		fi
	fi
}


function platformClean(){
	echo "==================================================================================================================="
	rm -rf /ccos/*
	echo "  Platform Clean                 = Complete"
	echo "==================================================================================================================="
	echo
}

function exeFileConfiguration(){
	echo "==================================================================================================================="
	tar -cvf ~/CCNC_Image/image.tar.gz /ccos
	echo
	echo "  Tar(~/CCNC_Image/image.tar.gz) = Complete"
	echo "==================================================================================================================="
	echo
}

function checkCpplit(){
	if [ "$1" = "a" ] || [ "$1" = "api" ] || [ "$1" = "hm" ] || [ "$1" = "hal-mobis" ]; then
		cd ..
		cpplint.sh
	fi
	echo
	echo "  Cpplint               = Complete"
	echo "==================================================================================================================="
	echo
}

function buildAll(){
	#echo "==================================================================================================================="
	#echo "   =>  buildAll()"
	#echo

	setBuildInfo $1 $2 $3
	
	echo "==================================================================================================================="
	make -j8
	echo
	echo "  Make                  = Complete"
	echo "==================================================================================================================="

	make install
	echo
	echo "  Install               = Complete"
	echo "==================================================================================================================="

	sudo cp -apr /ccos $SET_ENVIRONMENT_PATH_BASE/sysroots/aarch64-gnu-linux
	echo "  Copy                  = Complete"
	echo "==================================================================================================================="
	
}



function build(){
pushd $PROJECT_DIR/$MODULE_DIR
	if [ "$1" = "c" ] || [ "$1" = "clean" ]; then
		platformClean
	elif [ "$1" = "t" ] || [ "$1" = "tar" ]; then
		exeFileConfiguration
	else
		setEnvironments
		buildAll $1 $2 $3
		checkCpplit $2
	fi
popd
}

build $1 $2 $3
echo "==================================================================================================================="
echo
echo
