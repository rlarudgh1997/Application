#!/bin/bash -e

BASE_DIR="$(dirname $(realpath $0))"
PROJECT_DIR="$BASE_DIR/.."
MODULE_DIR=

USE_MODULE_DISPLAY="display"
USE_MODULE_VARIANT="variant"
USE_MODULE_POWER="power"
USE_MODULE_SYSTEM="system"
USE_MODULE_VEHICLE="vehicle"
USE_MODULE_META="300_Meta"
USE_MODULE_META_CLUSTER="400_Meta_Cluster"

SET_MODULE=
SET_NAME_HLIB=
SET_NAME_SERVICE=
SET_NAME_HAL=
SET_NAME_HAL_MOBIS=
SET_NAME_HAL_STUB=
SET_NAME_HLIB_NEW=

SET_NAME_META_BSP_MOBIS="meta-bsp-mobis"
SET_NAME_META_CCOS="meta-ccos"
SET_NAME_META_CCOS_AVN="meta-ccos-avn"


MAIN_BRANCH=@s6mobis
MAIN_BRANCH_CLUSTER=@mars

clear
clear
clear

echo "==================================================================================================================="
echo "   =>  $PROJECT_DIR"
echo "   =>  ./update.sh [display/variant/power/system/vehicle/meta/meta_cluster] [c/p/a]"
echo "   =>  ./update.sh [d/v/p/s/ve/m/mc] [c/p/a]"
echo "==================================================================================================================="
echo



function set_module_name(){
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
	elif [ "$1" = $USE_MODULE_META ] || [ "$1" = "m" ]; then
		SET_MODULE=$USE_MODULE_META
		MODULE_DIR="300_Meta"
	elif [ "$1" = $USE_MODULE_META_CLUSTER ] || [ "$1" = "mc" ]; then
		SET_MODULE=$USE_MODULE_META_CLUSTER
		MODULE_DIR="400_Meta_Cluster"
		
	fi
	

	if [ "$SET_MODULE" = "" ]; then
		echo "[Error] ModuleName=$2 -> [Valid = $USE_MODULE_DISPLAY/$USE_MODULE_VARIANT/$USE_MODULE_POWER]"
		exit 0
	elif [ "$SET_MODULE" = $USE_MODULE_META ]; then
		SET_NAME_META_BSP_MOBIS="meta-bsp-mobis"
		SET_NAME_META_CCOS="meta-ccos"
		SET_NAME_META_CCOS_AVN="meta-ccos-avn"
		
		echo "MODULE_DIR              = $MODULE_DIR"
		echo "MAIN_BRANCH             = $MAIN_BRANCH"
		echo "SET_NAME_META_BSP_MOBIS = $SET_NAME_META_BSP_MOBIS"
		echo "SET_NAME_META_CCOS      = $SET_NAME_META_CCOS"
		echo "SET_NAME_META_CCOS_AVN  = $SET_NAME_META_CCOS_AVN"
	elif [ "$SET_MODULE" = $USE_MODULE_META_CLUSTER ]; then
		SET_NAME_META_BSP_MOBIS="clu_gen2.mobis.meta-bsp-mobis"
		SET_NAME_META_CCOS_CLU="meta-ccos-clu"
		
		echo "MODULE_DIR              = $MODULE_DIR"
		echo "MAIN_BRANCH_CLUSTER     = $MAIN_BRANCH_CLUSTER"
		echo "SET_NAME_META_BSP_MOBIS = $SET_NAME_META_BSP_MOBIS"
		echo "SET_NAME_META_CCOS_CLU  = $SET_NAME_META_CCOS_CLU"
	else
		if [ "$SET_NAME_HLIB_NEW" != "" ]; then
			SET_NAME_HLIB=ccos.api.h"$SET_NAME_HLIB_NEW"
		else
			SET_NAME_HLIB=ccos.api.h"$SET_MODULE"
		fi
		
		SET_NAME_SERVICE=ccos.service."$SET_MODULE"service
		SET_NAME_HAL=ccos.hal.h"$SET_MODULE"hal
		SET_NAME_HAL_MOBIS=ccos.hal."$SET_MODULE"hal-mobis
		SET_NAME_HAL_STUB=ccos.hal."$SET_MODULE"hal-stub
		
		echo "MODULE_DIR              = $MODULE_DIR"
		echo "MAIN_BRANCH             = $MAIN_BRANCH"
		echo "SET_MODULE              = $SET_MODULE"
		echo "SET_NAME_HLIB           = $SET_NAME_HLIB"
		echo "SET_NAME_SERVICE        = $SET_NAME_SERVICE"
		echo "SET_NAME_HAL            = $SET_NAME_HAL"
		echo "SET_NAME_HAL_MOBIS      = $SET_NAME_HAL_MOBIS"
		echo "SET_NAME_HAL_STUB       = $SET_NAME_HAL_STUB"
	fi
	
	echo
}

function get_checkout(){
	CMD="git checkout $MAIN_BRANCH"
	
	if [ "$1" = $USE_MODULE_META ] || [ "$1" = "m" ]; then
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_AVN"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_AVN
		$CMD
	elif [ "$1" = $USE_MODULE_META_CLUSTER ] || [ "$1" = "mc" ]; then
		CMD="git checkout $MAIN_BRANCH_CLUSTER"
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_CLU"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_CLU
		$CMD
	else
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HLIB"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HLIB
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_SERVICE"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_SERVICE
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_MOBIS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_MOBIS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_STUB"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_STUB
		$CMD
	fi
}

function git_pull(){
	CMD="git pull"
	
	if [ "$1" = $USE_MODULE_META ] || [ "$1" = "m" ]; then
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_AVN"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_AVN
		$CMD
	elif [ "$1" = $USE_MODULE_META_CLUSTER ] || [ "$1" = "mc" ]; then
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_BSP_MOBIS
		$CMD
		
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_CLU"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_META_CCOS_CLU
		$CMD
	else
		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HLIB"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HLIB
		$CMD

		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_SERVICE"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_SERVICE
		$CMD

		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL
		$CMD

		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_MOBIS"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_MOBIS
		$CMD

		echo "==================================================================================================================="
		echo "** PATH = $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_STUB"
		cd $PROJECT_DIR/$MODULE_DIR/$SET_NAME_HAL_STUB
		$CMD
	fi
}

function execute(){
	#pushd $PROJECT_DIR
		case "$2" in
			a)
				set_module_name $1
				get_checkout $1
				echo "==================================================================================================================="
				echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
				git_pull $1
				;;
			c)
				set_module_name $1
				get_checkout $1
				;;
			p)
				set_module_name $1
				git_pull $1
				;;
			*)				
				echo "[Error] Input Script Command !!!!!!!!!!!!!!!!!!!!!!!"
				echo "[Error] ModuleName=$2 -> [Valid = $USE_MODULE_DISPLAY/$USE_MODULE_VARIANT/$USE_MODULE_POWER/$USE_MODULE_META/$USE_MODULE_META_CLUSTER]"
				echo "[Error] ./update.sh Valid c/p"
				;;
		esac
	#popd
}

execute $1 $2 $3

echo
echo "==================================================================================================================="




