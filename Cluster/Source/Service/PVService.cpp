#include "PVService.h"

#include "Logger.h"

PVService::PVService(QObject* parent) : QObject(parent), mAppService(nullptr) {
}

PVService::~PVService() {
}

void PVService::setAppService(AppService* appservice) {
    if (appservice == nullptr) {
        HDebug() << "AppService is nullptr";
        return;
    }
    mAppService = appservice;

    subscribeToCluster();
}

void PVService::subscribeToCluster() {
    if (mAppService == nullptr) {
        HDebug() << "Fail subscribeToCluster, AppsService is nullptr";
        return;
    }
    subscribeTelltale();
    subscribeEvent();
}

void PVService::subscribeTelltale() {
    /* EV_Power_Down */
    auto telltaleEvPwrDnHandler = std::bind(&PVService::onTelltaleEvPwrDnStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.EV_Power_Down.Telltale.EV_Power_Down.Stat, telltaleEvPwrDnHandler);

    /* xEVServiceLamp */
    auto srvLmpEvHandler = std::bind(&PVService::onTelltalexEVsrvLmpEvStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.xEV_Service_Lamp.Telltale.SrvLmpEv.Stat, srvLmpEvHandler);

    auto srvLmpHevHandler = std::bind(&PVService::onTelltalexEVsrvLmpHevStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.xEV_Service_Lamp.Telltale.SrvLmpHev.Stat, srvLmpHevHandler);

    auto srvLmpFcevHandler = std::bind(&PVService::onTelltalexEVsrvLmpFcevStatChanged, this, std::placeholders::_1);
    mAppService->addSubscription(SFC.xEV_Service_Lamp.Telltale.SrvLmpFcev.Stat, srvLmpFcevHandler);
}

void PVService::subscribeEvent() {
}

void PVService::onTelltaleEvPwrDnStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.EV_Power_Down.Telltale.EV_Power_Down.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleEvPwrDnStat,
                static_cast<ccos::HUInt64>(SFC.EV_Power_Down.Telltale.EV_Power_Down.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void PVService::onTelltalexEVsrvLmpEvStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_Service_Lamp.Telltale.SrvLmpEv.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleSrvLmpEvStat,
                static_cast<ccos::HUInt64>(SFC.xEV_Service_Lamp.Telltale.SrvLmpEv.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void PVService::onTelltalexEVsrvLmpHevStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_Service_Lamp.Telltale.SrvLmpHev.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleSrvLmpHevStat,
                static_cast<ccos::HUInt64>(SFC.xEV_Service_Lamp.Telltale.SrvLmpHev.Stat.value(vehicleSignal)));
            return;
        }
    }
}

void PVService::onTelltalexEVsrvLmpFcevStatChanged(const HVehicleSignalList& signalList) {
    for (const auto& vehicleSignal : signalList) {
        if (vehicleSignal.getNodePath() == SFC.xEV_Service_Lamp.Telltale.SrvLmpFcev.Stat) {
            emit mAppService->signalTelltaleValueChanged(
                TelltaleType::TelltaleSrvLmpFcevStat,
                static_cast<ccos::HUInt64>(SFC.xEV_Service_Lamp.Telltale.SrvLmpFcev.Stat.value(vehicleSignal)));
            return;
        }
    }
}
