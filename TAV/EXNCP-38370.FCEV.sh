#!/bin/bash

#[Description]
#    Ticket No : EXNCP-38370
#    Issue Type : Human
#    Module : ISG [PT300]
#    HMC : https://jira.ccos.dev/browse/EXNCP-EXNCP-38370
#    Info : 39804 - [PT300]ISG 페이지 ISG 동작시간 최대값 도달 이후 0:00부터 표시 안됨

#[PowerTrain]
#    ICV, EV, FCEV, PHEV, HEV

#[Precondition]
./alton_client set SFC.Private.IGNElapsed.ElapsedOn0ms uint64_t 1
./alton_client inject Vehicle.System.Config.Inter_ConfigISG 1
./alton_client inject Vehicle.System.Config.Inter_PTLogicControlType 1
./alton_client set SFC.Extension.Settings.Inter_AutoStopTimerSetStatus uint64_t 1
./alton_client inject Vehicle.PT.ISG.Input_EngineISGStatus1 1
./alton_client inject Vehicle.PT.ISG.Input_EngineISGStatus2 0
./alton_client inject Vehicle.PT.ISG.Input_IsgOperationTimeHourValue 99
./alton_client inject Vehicle.PT.ISG.Input_IsgOperationTimeMinValue 99
./alton_client inject Vehicle.PT.ISG.Input_IsgOperationTimeSecValue 59

#[Listen]
./alton_client listen SFC.ISG.Constant.IsgAbleTimeHour.Value SFC.ISG.Constant.IsgAbleTimeMin.Value SFC.ISG.Constant.IsgAbleTimeSec.Value

#[Step]
./alton_client inject Vehicle.System.Config.Inter_PTLogicControlType 0
sleep 60000

#[Expected Result]
#    SFC.ISG.Constant.IsgAbleTimeHour.Value 0
#    SFC.ISG.Constant.IsgAbleTimeMin.Value 0
#    SFC.ISG.Constant.IsgAbleTimeSec.Value 59

