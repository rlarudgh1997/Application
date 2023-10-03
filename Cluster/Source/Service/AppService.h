#ifndef APPSERVICE_H
#define APPSERVICE_H
#include <QObject>

#ifndef X86_64
#include <HSubscription.h>
#include <HVehicleSignalModel.h>
#include <listener/ClusterSignalListener.h>
#endif

#include "common/CommonEnum.h"
#include "SFCHelper.h"

#ifndef X86_64
#include <vsm.h>
using ccos::vehicle::vsm::HSubscriptionType;
#endif

namespace ccos::vehicle::vsm {
class HSubscription;
class HVehicleSignal;
class HVehicleSignalModel;
}  // namespace ccos::vehicle::vsm

using HVehicleSignalList = std::vector<ccos::vehicle::vsm::HVehicleSignal>;
using ccos::vehicle::vsm::HSubscription;
using ccos::vehicle::vsm::HVehicleSignal;
using ccos::vehicle::vsm::HVehicleSignalModel;
typedef std::function<void(const std::vector<HVehicleSignal>&)> SignalHandlingFunc;

class AppService : public QObject {
    Q_OBJECT
public:
    explicit AppService(QObject* parent = Q_NULLPTR);
    virtual ~AppService();

    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    HVehicleSignalModel* getVehicleSignalModel() const;

private:
    void subscribeToCluster();
    void subscribeTelltale();
    void subscribeEvent();
    void subscribeGauge();
    void subscribeContents();
    void subscribeMainView();

    // Telltale
    void onTelltaleECallStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleSeatbeltWarningStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleOATChanged(const HVehicleSignalList& signalList);
    void onTelltaleAirBagStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampFailWarningStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleIFSStatChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVSystemReadyChanged(const HVehicleSignalList& signalList);
    void onTelltaleKeySystemChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorFrontFogChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorHighBeamChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorRearFogChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorTailLampChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorTurnSignalLeftChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorTurnSignalRightChanged(const HVehicleSignalList& signalList);
    void onTelltaleLampIndicatorLowBeamChanged(const HVehicleSignalList& signalList);
    void onTelltaleH2FaultChanged(const HVehicleSignalList& signalList);
    void onTelltaleH2LeakChanged(const HVehicleSignalList& signalList);
    void onTelltaleAFSDBLStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleLDWLKAChanged(const HVehicleSignalList& signalList);
    void onTelltaleFCAChanged(const HVehicleSignalList& signalList);
    void onTelltaleELKChanged(const HVehicleSignalList& signalList);
    void onTelltaleLowFuelWarnChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewLKAStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewFCAStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewELKStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewHBAStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewLCALeftStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewLCARightStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleADASDrivingNewLFAStatChanged(const HVehicleSignalList& signalList);

    // Event
    void onEventFunc(const HVehicleSignalList& signalList);
    void onEventLampFailWarningBiStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningDrlStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningFogStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningHighStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningLowStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningCornerStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningStopStatChanged(const HVehicleSignalList& signalList);
    void onEventLampFailWarningTurnStatChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteDistanceStatChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteDistanceValueChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteTimeStatChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteTimeValueChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteSpeedStatChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteSpeedValueChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteFuelEcoStatChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeDriveCompleteFuelEcoValueChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeResvChargeStatChanged(const HVehicleSignalList& signalList);
    void onEventIntroOutroGoodbyeResvClimateStatChanged(const HVehicleSignalList& signalList);
    void onEventIlluminationRheostatLevelChanged(const HVehicleSignalList& signalList);
    void onEventIlluminationClusterLightingLevelChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindDayStatChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindDayValueChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindDistKmStatChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindDistMiStatChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindDistKmValueChanged(const HVehicleSignalList& signalList);
    void onEventServiceRequiredWarningServiceRemindDistMiValueChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingBVMNEwLeftOutMirrorFoldStatChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingBVMNEwRightOutMirrorFoldStatChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkCarBodyDisplayChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkFrontLeftWarning3PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkFrontCenterWarning3PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRearLeftWarning3PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkFrontRightWarning3PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRearCenterWarning3PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRearRightWarning3PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRearLeftWarning2PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRearRightWarning2PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkLeftSideWarning1PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRightSideWarning1PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkLeftSideFrontWarning2PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRightSideFrontWarning2PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkLeftSideRearWarning2PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRightSideRearWarning2PartChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkFrontMasterWarningChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRearMasterWarningChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkLeftSideMasterWarningChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRightSideMasterWarningChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkPcaFrontBoundaryLaneChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkPcaRearBoundaryLaneChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkPcaleftBoundaryLaneChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkPcaRightBoundaryLaneChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkPCAWarningPhraseChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRCCWLeftSymbolChanged(const HVehicleSignalList& signalList);
    void onEventADASParkingNEWParkRCCWRightSymbolChanged(const HVehicleSignalList& signalList);

    // Contents
    void onContentsOATChanged(const HVehicleSignalList& signalList);
    void onContentsDISMainSpeedStatChanged(const HVehicleSignalList& signalList);
    void onContentsDISMainSpeedValueChanged(const HVehicleSignalList& signalList);
    void onContentsDISAddtnlSignChanged(const HVehicleSignalList& signalList);
    void onContentsDISNaviDisCountryChanged(const HVehicleSignalList& signalList);
    void onContentsSEASEALeftWarnSymbolStatChanged(const HVehicleSignalList& signalList);
    void onContentsSEASEARightWarnSymbolStatChanged(const HVehicleSignalList& signalList);
    void onContentsSOCGaugeValueChanged(const HVehicleSignalList& signalList);
    void onContentsSOCGaugeLowAlertStatChanged(const HVehicleSignalList& signalList);
    void onContentsCruiseHDASymbolStatChanged(const HVehicleSignalList& signalList);
    void onConstantSettingSpeedValueStatChanged(const HVehicleSignalList& signalList);
    void onConstantSettingSpeedValueChanged(const HVehicleSignalList& signalList);
    void onConstantSettingSpeedColorStatChanged(const HVehicleSignalList& signalList);
    void onConstantSettingSpeedUnitStatChanged(const HVehicleSignalList& signalList);
    void onConstantSettingSpeedBlinkingStatChanged(const HVehicleSignalList& signalList);
    void onConstantFrontVehicleStatChanged(const HVehicleSignalList& signalList);
    void onConstantDistanceFromFrontVehicleStatChanged(const HVehicleSignalList& signalList);
    void onConstantNaviSetSymbolStatChanged(const HVehicleSignalList& signalList);
    void onConstantISLADirectionStatChanged(const HVehicleSignalList& signalList);
    void onConstantLCAStatChanged(const HVehicleSignalList& signalList);
    void onConstantLFAStatChanged(const HVehicleSignalList& signalList);
    void onConstnatAdasOnStatusChanged(const HVehicleSignalList& signalList);
    void onContentsISLAMainSpeedStatChanged(const HVehicleSignalList& signalList);
    void onContentsISLAMainSpeedBlinkChanged(const HVehicleSignalList& signalList);
    void onContentsISLAMainSpeedBlinkColorChanged(const HVehicleSignalList& signalList);
    void onContentsISLAMainSpeedValueChanged(const HVehicleSignalList& signalList);
    void onContentsISLAFinalAddtnlSignChanged(const HVehicleSignalList& signalList);
    void onContentsISLAOverlapSignStatChanged(const HVehicleSignalList& signalList);
    void onContentsISLAOverlapSignBlinkChanged(const HVehicleSignalList& signalList);
    void onContentsADASDrivingNewISLAAddSignStatChanged(const HVehicleSignalList& signalList);
    void onContentsADASDrivingNewISLAMainChanged(const HVehicleSignalList& signalList);
    void onContentsADASDrivingNewFinalAddtnlSignChanged(const HVehicleSignalList& signalList);

    // MainView
    void onMainViewTBTNaviAliveStatusChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTConfigNaviMapCountryChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviTBTStatChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTDirectionTypeChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTSideStreetBitMaskChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTCombinedSideStreetChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviPointFlagTypeChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviDistToDestChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviDistToTurnPointChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTBarGraphChanged(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviDistToTurn1Changed(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviDistToTurn2Changed(const HVehicleSignalList& signalList);
    void onMainViewTBTNaviLaneRemainDistChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftSideLaneColorChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftSideLaneLateralValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftSideLaneLateralValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftSideLaneProgressInLaneChangeStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRightSideLaneColorChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRightSideLaneLateralValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRightSideLaneLateralValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRightSideLaneProgressInLaneChangeStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftRightSideLaneCurvationValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftRightSideLaneCurvationValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleFrontVehicleStatusStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleFrontLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleFrontLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleFrontLateralDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleFrontLateralDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftForegroundVehicleStatusStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftForegroundVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftForegroundLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftForegroundLateralDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftForegroundLateralDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleRightForegroundVehicleStatusStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleRightForegroundVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleRightForegroundLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleRightForegroundLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleRightForegroundLateralDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleRightForegroundLateralDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftRearwardVehicleStatusStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftRearwardVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftRearwardLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftRearwardLateralDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASVehicleLeftRearwardLateralDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRearRightHandSideVehicleVehicleStatusStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRearRightHandSideVehicleVehicleStatusFadeStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRearRightHandSideVehicleLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRearRightHandSideVehicleLateralDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRearRightHandSideVehicleLateralDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASTheRearLeftHandSideVehicleStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASTheRearRightHandSideVehicleStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftRearSideCrashAlarmWaveChanged(const HVehicleSignalList& signalList);
    void onMainViewADASLeftRearSideCrashAlarmMasterSymbolChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRightRearSideCrashAlarmWaveChanged(const HVehicleSignalList& signalList);
    void onMainViewADASRightRearSideCrashAlarmMasterSymbolChanged(const HVehicleSignalList& signalList);
    void onMainViewADASTargetIntervalDistanceStaChanged(const HVehicleSignalList& signalList);
    void onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueChanged(const HVehicleSignalList& signalList);
    void onMainViewADASTargetIntervalDistanceLongitudinalDistanceValueStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASCenterLineStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewFrontVehicleChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewHeadwayDistanceChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewSubVehicleColorStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewSetSpeedChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewISLAManualSetSpeedStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewHDAHDPStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewLaneCentralLineStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewLeftLaneChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewRightLaneChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewLaneCvrtLvlChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewHeadwayDistChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewCenterRoadChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewLeftRoadColorStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewRightRoadColorStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewFrontVehicleChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewFrontLeftVehicleChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewFrontRightVehicleChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewLeftVehicleChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewRightVehicleChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewRearLeftWarnStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewRearRightWarnStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewRearLeftArrowStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewRearRightArrowStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewHDPStatDisplayBarStatChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewCenterRoadLeftLatPosChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewCenterRoadRightLatPosChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewRoadCvrtLvlChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewLeftRoadLatPosChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewRightRoadLatPosChanged(const HVehicleSignalList& signalList);
    void onMainViewADASDrivingNewViewFrontWarnStatChanged(const HVehicleSignalList& signalList);

    // Gauge
    void onSpeedGaugeDisplaySpeedValueKPHChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeDisplaySpeedValueMPHChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedAnalogStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedAnalogValueChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedDigitalStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedDigitalValueChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedSubDigitalStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedSubDigitalValueChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedMainDisplayUnitStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedAuxDisplayUnitStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeSpeedSubDisplayStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeDisplaySpeedValueKPHRangeStatChanged(const HVehicleSignalList& signalList);
    void onSpeedGaugeDisplaySpeedUnitChanged(const HVehicleSignalList& signalList);
    void onFuelSystemFuelGaugeChanged(const HVehicleSignalList& signalList);

signals:
    void signalTelltaleValueChanged(const TelltaleType& type, const ccos::HUInt64& value);
    void signalPopupValueChanged(const QString& popupId, const ccos::HUInt64& stat, const ccos::HBool& isMasterWarn = false);
    void signalContentsValueChanged(const ContentsType& type, const ccos::HUInt64& value);
    void signalContentsOATValueChanged(const QString& value);
    void signalSEAWarnSymbolChanged(const SEAWarnSymbolType& type, const ccos::HUInt64& stat);
    void signalLampFailStatChanged(const LampFailWarningStatType& type, const ccos::HUInt64& value);
    void signalMainViewTBTValueChanged(const MainViewTBTType& type, const ccos::HUInt64& value);
    void signalMainViewNaviValueChanged(const MainViewNaviDistType& type, const ccos::HUInt64& stat, const ccos::HDouble& value,
                                        const ccos::HUInt64& unit);
    void signalMainViewTBTTPMsgChanged(const MainViewTBTTpType& type, const QString& TpMsg);
    void signalMainViewADASStatChanged(const MainViewADASType& type, const ccos::HUInt64& value);
    void signalMainViewADASValueChanged(const MainViewADASType& type, const ccos::HDouble& value);
    void signalGaugeValueChanged(const GaugeType& type, const ccos::HDouble& value);
    void signalGaugeStatChanged(const GaugeType& type, const ccos::HUInt64& value);
    void signalIntroOutroDataChanged(const IntroOutroType& type, const ccos::HUInt64& value);
    void signalIlluminationLevelChanged(const EventIlluminationType& type, const ccos::HUInt64& value);
    void signalServiceRequiredWarningStatChanged(const ServiceRequiredWarningType& type, const ccos::HInt64& value);
    void signalFuelGaugeValueChanged(const ccos::HDouble& value);
    void signalADASParkingBVMNewStatChanged(const ADASParkingBVMNewType& type, const ccos::HUInt64& value);
    void signalContentsISLAFinalAddtnSignChanged(const ccos::HUInt64& stat, const ccos::HUInt64& islaSignValue,
                                                 const ccos::HUInt64& disSignValue);
    void signalADASParkingNEWChanged(const ADASParkingNEWType& type, const ccos::HUInt64& stat);
    void signalMainViewADASDrivingNewValueChanged(const MainViewADASDrivingNewType& type, const ccos::HUInt64& value);
    void signalContentsADASDrivingNewSummaryInfoChanged(const MainViewADASDrivingNewType& type, const ccos::HUInt64& value);
    void signalContentsADASDrivingNewISLAChanged(const MainViewADASDrivingNewType& type, const ccos::HUInt64& value);
    void signalContentsADASDrivingNewISLAFinalAddtnlSignChanged(const ccos::HUInt64& stat, const ccos::HUInt64& islaSignValue,
                                                                const ccos::HUInt64& disSignValue);

private:
    HVehicleSignalModel* mVehicleSignalModel = nullptr;
};

#endif  // APPSERVICE_H
