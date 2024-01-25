#!/bin/bash

clear
clear

echo "=========================================================================================================="


unset LD_LIBRARY_PATH
source /opt/sfc/CV/environment-setup-sfc CV
export LD_LIBRARY_PATH="/opt/sfc/CV/lib:$LD_LIBRARY_PATH"
export CCOS_LIB_DIR="/opt/sfc/CV"


# export ALTON_SFC_CONFIGURATION=${HOME}/900_Code/620_SFC/model/SFC/compiled/config
# export ALTON_SFC_MODEL_DIR=${HOME}/900_Code/620_SFC/model/SFC/compiled
# export ALTON_EOL_VEHICLETYPE=ICV
# export ALTON_SFC_CONFIGURATION_FILENAME=CV
# export ALTON_ENG_SKIP_DUMMYCALL=ON


# echo
# echo "ALTON_SFC_CONFIGURATION          = $ALTON_SFC_CONFIGURATION"
# echo "ALTON_EOL_VEHICLETYPE            = $ALTON_EOL_VEHICLETYPE"
# echo "ALTON_SFC_CONFIGURATION_FILENAME = $ALTON_SFC_CONFIGURATION_FILENAME"
# echo "ALTON_ENG_SKIP_DUMMYCALL         = $ALTON_ENG_SKIP_DUMMYCALL"
# echo "LD_LIBRARY_PATH                  = $LD_LIBRARY_PATH"
# echo "CCOS_LIB_DIR                     = $CCOS_LIB_DIR"


echo "=========================================================================================================="
echo
echo
