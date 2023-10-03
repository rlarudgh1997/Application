#ifndef HMISERVICE_H
#define HMISERVICE_H

#include <QObject>

#ifndef X86_64
#include <HSubscription.h>
#include <HVehicleSignalModel.h>
#include <listener/ClusterSignalListener.h>
#endif

#include "AppService.h"
#include "common/CommonEnum.h"

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

class CVService : public QObject {
    Q_OBJECT
public:
    explicit CVService(QObject* parent = Q_NULLPTR);
    virtual ~CVService();

    void addSubscription(const std::string& nodeAddress, const SignalHandlingFunc& handlingFunc);
    void addSubscriptions(const std::vector<std::string>& nodePaths, const SignalHandlingFunc& handlingFunc);

    void setAppService(AppService* appService);

private:
    void subscribeToCluster();
    void subscribeTelltale();
    void subscribeEvent();

    // Telltale
    void onTelltaleDualPowerStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleLiftgateStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleBEQStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleTachographStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleHeatedMirrorStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleWorkingLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleASRChanged(const HVehicleSignalList& signalList);
    void onTelltaleEBSAmberLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleEBSRedLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleLWSChanged(const HVehicleSignalList& signalList);
    void onTelltaleECASChanged(const HVehicleSignalList& signalList);
    void onTelltaleECASAxleMotionStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleECASAxleMotionStatOptionalChanged(const HVehicleSignalList& signalList);
    void onTelltaleBrakeAirLowLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleEHSAmberLampChanged(const HVehicleSignalList& signalList);
    void onTelltaleEAPULampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleAutoGreaseLowLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleCabTiltingStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleTMPTOStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleDifferencialLockStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleTMHighStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleWheelLockStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleRetarderLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleRetarderMalLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleHighVoltageBatteryChargeStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleTPMSCVStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleABSTrailerLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleNoABSTrailerLampChanged(const HVehicleSignalList& signalList);
    void onTelltaleABSCVStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleVDCChanged(const HVehicleSignalList& signalList);
    void onTelltaleVDCOFFStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleSteeringWarningLampStatChanged(const HVehicleSignalList& signalList);
    void onTelltaleParkingBrakeCVStatChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVServiceLampCVSrvLmpEvChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVServiceLampCVSrvLmpFcevChanged(const HVehicleSignalList& signalList);
    void onTelltalexEVPowerLimitCVChanged(const HVehicleSignalList& signalList);
    void onTelltaleGFDWarningChanged(const HVehicleSignalList& signalList);

private:
    AppService* mAppService = nullptr;
};

#endif  // HMISERVICE_H
