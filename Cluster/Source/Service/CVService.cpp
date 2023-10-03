#include "CVService.h"

#include "Logger.h"

CVService::CVService(QObject* parent) : QObject(parent), mAppService(nullptr) {
}

CVService::~CVService() {
}

void CVService::setAppService(AppService* appService) {
    if (appService == nullptr) {
        HDebug() << "AppService is nullptr";
        return;
    }
    mAppService = appService;

    subscribeToCluster();
}

void CVService::subscribeToCluster() {
    if (mAppService == nullptr) {
        HDebug() << "Fail subscribeToCluster, AppsService is nullptr";
        return;
    }
    subscribeTelltale();
    subscribeEvent();
}

void CVService::subscribeTelltale() {
#ifndef X86_64
    /* 1. Dual Power */
    auto dualPowerStatHandler = std::bind(&CVService::onTelltaleDualPowerStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Dual_Power.Telltale.Dual_Power.Stat, dualPowerStatHandler);

    /* 2. LiftGate */
    auto liftGateStatHandler = std::bind(&CVService::onTelltaleLiftgateStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Special_Vehicle.Telltale.Liftgate.Stat, liftGateStatHandler);

    /* 3. BEQ */
    auto beqStatHandler = std::bind(&CVService::onTelltaleBEQStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.BEQ.Telltale.BEQ.Stat, beqStatHandler);

    /* 4. Tachograph */
    auto tachoGraphStatHandler = std::bind(&CVService::onTelltaleTachographStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Tachograph.Telltale.Tachograph.Stat, tachoGraphStatHandler);

    /* 5. Heated Mirror */
    auto heatedMirrorHandler = std::bind(&CVService::onTelltaleHeatedMirrorStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Heated_Mirror.Telltale.Heated_Mirror.Stat, heatedMirrorHandler);

    /* 6. Working Lamp */
    auto workingLampHandler = std::bind(&CVService::onTelltaleWorkingLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Working_Lamp.Telltale.Working_Lamp.Stat, workingLampHandler);

    /* 7. ASR */
    auto asrTelltaleHandler = std::bind(&CVService::onTelltaleASRChanged, this, std::placeholders::_1);
    std::vector<std::string> asrTelltaleNodes{SFC.ASR.Telltale.ASR.Stat, SFC.ASR.Telltale.ASR.BlinkValueA,
                                              SFC.ASR.Telltale.ASR.BlinkValueB};
    mAppService->addSubscriptions(asrTelltaleNodes, asrTelltaleHandler);

    /* 8. EBS */
    auto ebsAmberLampStatHandler = std::bind(&CVService::onTelltaleEBSAmberLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.EBS.Telltale.EBSAmberLamp.Stat, ebsAmberLampStatHandler);

    auto ebsRedLampStatHandler = std::bind(&CVService::onTelltaleEBSRedLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.EBS.Telltale.EBSRedLamp.Stat, ebsRedLampStatHandler);

    /* 9. LWS */
    auto lwsTelltaleHandler = std::bind(&CVService::onTelltaleLWSChanged, this, std::placeholders::_1);
    std::vector<std::string> lwsTelltaleNodes{SFC.LWS.Telltale.LWS.Stat, SFC.LWS.Telltale.LWS.BlinkValueA,
                                              SFC.LWS.Telltale.LWS.BlinkValueB};
    mAppService->addSubscriptions(lwsTelltaleNodes, lwsTelltaleHandler);

    /* 10. ECAS */
    auto ecasStatHandler = std::bind(&CVService::onTelltaleECASChanged, this, std::placeholders::_1);
    std::vector<std::string> ecasTelltaleNodes{SFC.ECAS.Telltale.ECAS.Stat, SFC.ECAS.Telltale.ECAS.StatOptional,
                                               SFC.ECAS.Telltale.ECAS.BlinkValueA, SFC.ECAS.Telltale.ECAS.BlinkValueAOptional,
                                               SFC.ECAS.Telltale.ECAS.BlinkValueB};
    mAppService->addSubscriptions(ecasTelltaleNodes, ecasStatHandler);

    auto ecasAxleMotionStatHandler = std::bind(&CVService::onTelltaleECASAxleMotionStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.ECAS.Telltale.AxleMotion.Stat, ecasAxleMotionStatHandler);

    auto ecasAxleMotionStatOptionalHandler =
        std::bind(&CVService::onTelltaleECASAxleMotionStatOptionalChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.ECAS.Telltale.AxleMotion.StatOptional, ecasAxleMotionStatOptionalHandler);

    /* 11. Brake Air */
    auto brakeAirLowLampStatHandler = std::bind(&CVService::onTelltaleBrakeAirLowLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Brake_Air.Telltale.BrakeAirLowLamp.Stat, brakeAirLowLampStatHandler);

    /* 12. EHS */
    auto ehsStatHandler = std::bind(&CVService::onTelltaleEHSAmberLampChanged, this, std::placeholders::_1);
    std::vector<std::string> ehsTelltaleNodes{SFC.EHS.Telltale.EHSLamp.Stat, SFC.EHS.Telltale.EHSLamp.BlinkValueA,
                                              SFC.EHS.Telltale.EHSLamp.BlinkValueB};
    mAppService->addSubscriptions(ehsTelltaleNodes, ehsStatHandler);

    /* 13. EAPU */
    auto eapuLampStatHandler = std::bind(&CVService::onTelltaleEAPULampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.EAPU.Telltale.EAPULamp.Stat, eapuLampStatHandler);

    /* 14. Auto Grease */
    auto autoGreaseLowLampStatHandler =
        std::bind(&CVService::onTelltaleAutoGreaseLowLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Auto_Grease.Telltale.GreaseLowLamp.Stat, autoGreaseLowLampStatHandler);

    /* 15. Cab Tilting */
    auto cabTiltingStatHandler = std::bind(&CVService::onTelltaleCabTiltingStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Cab_Tilting.Telltale.Cab_Tilting.Stat, cabTiltingStatHandler);

    /* 16. TM PTO */
    auto tmPtoStatHandler = std::bind(&CVService::onTelltaleTMPTOStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.TM_PTO.Telltale.TM_PTO.Stat, tmPtoStatHandler);

    /* 17. Differencial Lock */
    auto differencialLockStatHandler = std::bind(&CVService::onTelltaleDifferencialLockStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Differencial_Lock.Telltale.Differencial_Lock.Stat, differencialLockStatHandler);

    /* 18. TM High */
    auto tmHighStatHandler = std::bind(&CVService::onTelltaleTMHighStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.TM_High_Low_Range.Telltale.TM_High.Stat, tmHighStatHandler);

    /* 19. Wheel Lock */
    auto wheelLockStatHandler = std::bind(&CVService::onTelltaleWheelLockStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Wheel_Lock.Telltale.Wheel_Lock.Stat, wheelLockStatHandler);

    /* 20. Retarder */
    auto retarderLampStatHandler = std::bind(&CVService::onTelltaleRetarderLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Retarder.Telltale.RetarderLamp.Stat, retarderLampStatHandler);

    auto retarderMalLampStatHandler = std::bind(&CVService::onTelltaleRetarderMalLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Retarder.Telltale.RetarderMalLamp.Stat, retarderMalLampStatHandler);

    /* 21. HIgh Voltage Battery Charge */
    auto highVoltBatChrLampStatHandler =
        std::bind(&CVService::onTelltaleHighVoltageBatteryChargeStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.High_Voltage_Battery_Charge.Telltale.HiVoltBatChrLamp.Stat, highVoltBatChrLampStatHandler);

    /* 22. TPMS CV */
    auto tpmsStatHandler = std::bind(&CVService::onTelltaleTPMSCVStatChanged, this, std::placeholders::_1);
    std::vector<std::string> tpmsTelltaleNodes{SFC.TPMS_CV.Telltale.TPMSCV.Stat, SFC.TPMS_CV.Telltale.TPMSCV.BlinkValueA,
                                               SFC.TPMS_CV.Telltale.TPMSCV.BlinkValueB};
    mAppService->addSubscriptions(tpmsTelltaleNodes, tpmsStatHandler);

    /* 23. ABS, No ABS Trailer */
    auto absTrailerLampStatHandler = std::bind(&CVService::onTelltaleABSTrailerLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.ABS_NO_ABS_Trailer.Telltale.ABSTrailerLamp.Stat, absTrailerLampStatHandler);

    auto noABSTrailerLampStatHandler = std::bind(&CVService::onTelltaleNoABSTrailerLampChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.ABS_NO_ABS_Trailer.Telltale.NoABSTrailerLamp.Stat, noABSTrailerLampStatHandler);

    /* 24. ABS CV */
    auto absCVStatHandler = std::bind(&CVService::onTelltaleABSCVStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.ABS_CV.Telltale.ABS_CV.Stat, absCVStatHandler);

    /* 25. VDC */
    auto vdcStatHandler = std::bind(&CVService::onTelltaleVDCChanged, this, std::placeholders::_1);
    std::vector<std::string> vdcTelltaleNodes{SFC.VDC.Telltale.VDC.Stat, SFC.VDC.Telltale.VDC.BlinkValueA,
                                              SFC.VDC.Telltale.VDC.BlinkValueB};
    mAppService->addSubscriptions(vdcTelltaleNodes, vdcStatHandler);

    auto vdcOffStatHandler = std::bind(&CVService::onTelltaleVDCOFFStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.VDC.Telltale.VDC_OFF.Stat, vdcOffStatHandler);

    /* 26. Steering Warning Lamp */
    auto steeringWarningLampStatHandler =
        std::bind(&CVService::onTelltaleSteeringWarningLampStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Steering_Warning_Lamp.Telltale.Steering_Warning_Lamp.Stat, steeringWarningLampStatHandler);

    /* 27. Parking Brake CV */
    auto parkingBrakeCVHandler = std::bind(&CVService::onTelltaleParkingBrakeCVStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.Parking_Brake_CV.Telltale.Parking_Brake_CV.Stat, parkingBrakeCVHandler);

    /* xEV_Service_Lamp_CV */
    auto telltalexEVServiceLampCVSrvLmpEvHandler =
        std::bind(&CVService::onTelltalexEVServiceLampCVSrvLmpEvChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.xEV_Service_Lamp_CV.Telltale.SrvLmpEv.Stat, telltalexEVServiceLampCVSrvLmpEvHandler);

    auto telltalexEVServiceLampCVSrvLmpFcevHandler =
        std::bind(&CVService::onTelltalexEVServiceLampCVSrvLmpFcevChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.xEV_Service_Lamp_CV.Telltale.SrvLmpEv.Stat, telltalexEVServiceLampCVSrvLmpFcevHandler);

    /* xEV_Power_Limit */
    auto telltalexEVPowerLimitHandler = std::bind(&CVService::onTelltalexEVPowerLimitCVChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.xEV_Power_Limit.Telltale.EvPwrDn.Stat, telltalexEVPowerLimitHandler);

    /* GFD_Waring */
    auto telltaleGFDWarningHandler = std::bind(&CVService::onTelltaleGFDWarningChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.GFD_Warning.Telltale.GFD.Stat, telltaleGFDWarningHandler);

#endif
}

void CVService::subscribeEvent() {
    // Event Group Subscribe to Appservice
}

void CVService::onTelltaleDualPowerStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Dual_Power.Telltale.Dual_Power.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleDualPowerStat,
                static_cast<ccos::HUInt64>(SFC.Dual_Power.Telltale.Dual_Power.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleLiftgateStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Special_Vehicle.Telltale.Liftgate.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleLiftgateStat,
                static_cast<ccos::HUInt64>(SFC.Special_Vehicle.Telltale.Liftgate.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleBEQStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.BEQ.Telltale.BEQ.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleBEQStat, static_cast<ccos::HUInt64>(SFC.BEQ.Telltale.BEQ.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleTachographStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Tachograph.Telltale.Tachograph.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleTachographStat,
                static_cast<ccos::HUInt64>(SFC.Tachograph.Telltale.Tachograph.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleHeatedMirrorStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Heated_Mirror.Telltale.Heated_Mirror.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleHeatedMirrorStat,
                static_cast<ccos::HUInt64>(SFC.Heated_Mirror.Telltale.Heated_Mirror.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleWorkingLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Working_Lamp.Telltale.Working_Lamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleWorkingLampStat,
                static_cast<ccos::HUInt64>(SFC.Working_Lamp.Telltale.Working_Lamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define ASR_VALUE_COUNT 3
void CVService::onTelltaleASRChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ASR.Telltale.ASR.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleASRStat, static_cast<ccos::HUInt64>(SFC.ASR.Telltale.ASR.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ASR.Telltale.ASR.BlinkValueA) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleASRBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.ASR.Telltale.ASR.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.ASR.Telltale.ASR.BlinkValueB) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleASRBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.ASR.Telltale.ASR.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == ASR_VALUE_COUNT) {
            return;
        }
    }
}

void CVService::onTelltaleEBSAmberLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.EBS.Telltale.EBSAmberLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEBSAmberLampStat,
                static_cast<ccos::HUInt64>(SFC.EBS.Telltale.EBSAmberLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleEBSRedLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.EBS.Telltale.EBSRedLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEBSRedLampStat,
                static_cast<ccos::HUInt64>(SFC.EBS.Telltale.EBSRedLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define LWS_VALUE_COUNT 3
void CVService::onTelltaleLWSChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.LWS.Telltale.LWS.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleLWSStat, static_cast<ccos::HUInt64>(SFC.LWS.Telltale.LWS.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.LWS.Telltale.LWS.BlinkValueA) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleLWSBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.LWS.Telltale.LWS.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.LWS.Telltale.LWS.BlinkValueB) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleLWSBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.LWS.Telltale.LWS.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == LWS_VALUE_COUNT) {
            return;
        }
    }
}

#define ECAS_VALUE_COUNT 5
void CVService::onTelltaleECASChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.ECAS.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASStat, static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.ECAS.Stat.value(vehicleSignal)));
            return;
        } else if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.ECAS.StatOptional) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASOptionalStat,
                static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.ECAS.StatOptional.value(vehicleSignal)));
            return;
        } else if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.ECAS.BlinkValueA) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASOptionalStat,
                static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.ECAS.StatOptional.value(vehicleSignal)));
            return;
        } else if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.ECAS.BlinkValueAOptional) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASOptionalStat,
                static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.ECAS.StatOptional.value(vehicleSignal)));
            return;
        } else if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.ECAS.BlinkValueB) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASOptionalStat,
                static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.ECAS.StatOptional.value(vehicleSignal)));
            return;
        }

        if (valueCnt == ECAS_VALUE_COUNT) {
            return;
        }
    }
}

void CVService::onTelltaleECASAxleMotionStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.AxleMotion.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASAxleMotionStat,
                static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.AxleMotion.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleECASAxleMotionStatOptionalChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ECAS.Telltale.AxleMotion.StatOptional) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleECASAxleMotionStatOptional,
                static_cast<ccos::HUInt64>(SFC.ECAS.Telltale.AxleMotion.StatOptional.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleBrakeAirLowLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Brake_Air.Telltale.BrakeAirLowLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleBrakeAirLowLampStat,
                static_cast<ccos::HUInt64>(SFC.Brake_Air.Telltale.BrakeAirLowLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define EHS_VALUE_COUNT 3
void CVService::onTelltaleEHSAmberLampChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.EHS.Telltale.EHSLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEHSAmberLampStat,
                static_cast<ccos::HUInt64>(SFC.EHS.Telltale.EHSLamp.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.EHS.Telltale.EHSLamp.BlinkValueA) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEHSAmberLampBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.EHS.Telltale.EHSLamp.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.EHS.Telltale.EHSLamp.BlinkValueB) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEHSAmberLampBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.EHS.Telltale.EHSLamp.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == EHS_VALUE_COUNT) {
            return;
        }
    }
}

void CVService::onTelltaleEAPULampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.EAPU.Telltale.EAPULamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEAPULampStat,
                static_cast<ccos::HUInt64>(SFC.EAPU.Telltale.EAPULamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleAutoGreaseLowLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Auto_Grease.Telltale.GreaseLowLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleAutoGreaseLowLampStat,
                static_cast<ccos::HUInt64>(SFC.Auto_Grease.Telltale.GreaseLowLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleCabTiltingStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Cab_Tilting.Telltale.Cab_Tilting.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleCabTiltingStat,
                static_cast<ccos::HUInt64>(SFC.Cab_Tilting.Telltale.Cab_Tilting.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleTMPTOStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.TM_PTO.Telltale.TM_PTO.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleTMPTOStat,
                static_cast<ccos::HUInt64>(SFC.TM_PTO.Telltale.TM_PTO.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleDifferencialLockStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Differencial_Lock.Telltale.Differencial_Lock.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleDifferencialLockStat,
                static_cast<ccos::HUInt64>(SFC.Differencial_Lock.Telltale.Differencial_Lock.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleTMHighStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.TM_High_Low_Range.Telltale.TM_High.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleTMHighStat,
                static_cast<ccos::HUInt64>(SFC.TM_High_Low_Range.Telltale.TM_High.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleWheelLockStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Wheel_Lock.Telltale.Wheel_Lock.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleWheelLockStat,
                static_cast<ccos::HUInt64>(SFC.Wheel_Lock.Telltale.Wheel_Lock.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleRetarderLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Retarder.Telltale.RetarderLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleRetarderLampStat,
                static_cast<ccos::HUInt64>(SFC.Retarder.Telltale.RetarderLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleRetarderMalLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Retarder.Telltale.RetarderMalLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleRetarderMalLampStat,
                static_cast<ccos::HUInt64>(SFC.Retarder.Telltale.RetarderMalLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleHighVoltageBatteryChargeStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.High_Voltage_Battery_Charge.Telltale.HiVoltBatChrLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleHighVoltageBatteryChargeStat,
                static_cast<ccos::HUInt64>(SFC.High_Voltage_Battery_Charge.Telltale.HiVoltBatChrLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define TPMS_VALUE_COUNT 3
void CVService::onTelltaleTPMSCVStatChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.TPMS_CV.Telltale.TPMSCV.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleTPMSCVStat,
                static_cast<ccos::HUInt64>(SFC.TPMS_CV.Telltale.TPMSCV.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.TPMS_CV.Telltale.TPMSCV.BlinkValueA) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleTPMSCVBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.TPMS_CV.Telltale.TPMSCV.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.TPMS_CV.Telltale.TPMSCV.BlinkValueB) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleTPMSCVBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.TPMS_CV.Telltale.TPMSCV.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == TPMS_VALUE_COUNT) {
            return;
        }
    }
}

void CVService::onTelltaleABSTrailerLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ABS_NO_ABS_Trailer.Telltale.ABSTrailerLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleABSTrailerLampStat,
                static_cast<ccos::HUInt64>(SFC.ABS_NO_ABS_Trailer.Telltale.ABSTrailerLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleNoABSTrailerLampChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ABS_NO_ABS_Trailer.Telltale.NoABSTrailerLamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleNoABSTrailerLamp,
                static_cast<ccos::HUInt64>(SFC.ABS_NO_ABS_Trailer.Telltale.NoABSTrailerLamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleABSCVStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.ABS_CV.Telltale.ABS_CV.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleABSCVStat,
                static_cast<ccos::HUInt64>(SFC.ABS_CV.Telltale.ABS_CV.Stat.value(vehicleSignal)));
            return;
        }
    }
}

#define VDC_VALUE_COUNT 3
void CVService::onTelltaleVDCChanged(const HVehicleSignalList& signalList) {
    uint8_t valueCnt = 0;
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.VDC.Telltale.VDC.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleVDCStat, static_cast<ccos::HUInt64>(SFC.VDC.Telltale.VDC.Stat.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.VDC.Telltale.VDC.BlinkValueA) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleVDCBlinkValueA,
                static_cast<ccos::HUInt64>(SFC.VDC.Telltale.VDC.BlinkValueA.value(vehicleSignal)));
            valueCnt++;
        } else if (vehicleSignal.getNodePath() == SFC.VDC.Telltale.VDC.BlinkValueB) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleVDCBlinkValueB,
                static_cast<ccos::HUInt64>(SFC.VDC.Telltale.VDC.BlinkValueB.value(vehicleSignal)));
            valueCnt++;
        } else {
            // do nothing
        }

        if (valueCnt == VDC_VALUE_COUNT) {
            return;
        }
    }
}

void CVService::onTelltaleVDCOFFStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.VDC.Telltale.VDC_OFF.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleVDCOFFStat, static_cast<ccos::HUInt64>(SFC.VDC.Telltale.VDC_OFF.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleSteeringWarningLampStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Steering_Warning_Lamp.Telltale.Steering_Warning_Lamp.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleStreeingWarningLampStat,
                static_cast<ccos::HUInt64>(SFC.Steering_Warning_Lamp.Telltale.Steering_Warning_Lamp.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleParkingBrakeCVStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.Parking_Brake_CV.Telltale.Parking_Brake_CV.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleParkingBrakeCVStat,
                static_cast<ccos::HUInt64>(SFC.Parking_Brake_CV.Telltale.Parking_Brake_CV.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltalexEVServiceLampCVSrvLmpEvChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_Service_Lamp_CV.Telltale.SrvLmpEv.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltalexEVServiceLampCVSrvLmpEvStat,
                static_cast<ccos::HUInt64>(SFC.xEV_Service_Lamp_CV.Telltale.SrvLmpEv.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltalexEVServiceLampCVSrvLmpFcevChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_Service_Lamp_CV.Telltale.SrvLmpFcev.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltalexEVServiceLampCVSrvLmpFcevStat,
                static_cast<ccos::HUInt64>(SFC.xEV_Service_Lamp_CV.Telltale.SrvLmpFcev.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltalexEVPowerLimitCVChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_Power_Limit.Telltale.EvPwrDn.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltalexEVPowerLimitEvPwrDnStat,
                static_cast<ccos::HUInt64>(SFC.xEV_Power_Limit.Telltale.EvPwrDn.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void CVService::onTelltaleGFDWarningChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.GFD_Warning.Telltale.GFD.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleGFDWarningGFDStat,
                static_cast<ccos::HUInt64>(SFC.GFD_Warning.Telltale.GFD.Stat.value(vehicleSignal)));
            return;
        }
    }
}
