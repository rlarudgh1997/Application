#!/bin/bash

#[Description]
#    Ticket No : EXNCP-40685
#    Issue Type : Spec
#    Module : High Performance Gauge [PT280]
#    HMC : https://jira.ccos.dev/browse/EXNCP-40685
#    Info : 40685 - NX4E_Bar조건(0~1.3)에서 가속시 약 1.0bar로 바로 올라가고 감속시 0로 바로 떨러지는 현상과 천천히 감속시 900rpm까지도 터보바가 1.0bar에서 holding되는 현상이 나타남


#[PowerTrain]
#    EV


#[AltonClient]
ALTON_CLIENT="$1"
if [ -z "$1" ]; then
    ALTON_CLIENT=/opt/sfc/PV/bin/alton_client
fi
echo "ALTON_CLIENT=$ALTON_CLIENT"
sleep 0.3


#[Precondition]
$ALTON_CLIENT set SFC.Extension.AEM.Inter_AEMInitializeComplete bool true
$ALTON_CLIENT inject HardWire_HardWireIgnition1 1
$ALTON_CLIENT set SFC.Extension.Settings.Inter_TurboPressureUnitSetStatus uint64_t 2
$ALTON_CLIENT set SFC.Input_Output_Processing.Inter_EngineOnStatus uint64_t 1

for i in `seq 1 20`
do
    $ALTON_CLIENT inject Vehicle.PT.High_Performance_Gauge.Input_TurboBoostPressureValue 1000
    $ALTON_CLIENT inject Vehicle.PT.High_Performance_Gauge.Input_AtmosphericPressureValue 10
    sleep 0.01
done


#[Listen]
TAV_FILE=/home/ivis/900_Code/610_Application/tc_creator/TAV/EXNCP-40685.AltonListen.info
rm -f $TAV_FILE
$ALTON_CLIENT listen SFC.High_Performance_Gauge.Constant.TurboGauge.Value >> $TAV_FILE &


#[DateTime]
DATE_TIME_FILE=/home/ivis/900_Code/610_Application/tc_creator/TAV/EXNCP-40685.DateTime.info
rm -f $DATE_TIME_FILE
CURRENT_DATE_TIME=$(date +"%Y-%m-%d %H:%M:%S.%3N")
echo "CURRENT_DATE_TIME=$CURRENT_DATE_TIME"
echo -e "$CURRENT_DATE_TIME" >> $DATE_TIME_FILE


#[Step]
sleep 1
$ALTON_CLIENT set SFC.Extension.Settings.Inter_TurboPressureUnitSetStatus uint64_t 3


#[Expected Result]
#    SFC.High_Performance_Gauge.Constant.TurboGauge.Value 0.57
