#include "Service.h"

#include <regex>
#include <QDebug>


Service::Service(QObject* parent) : QObject(parent), mVehicleSignalModel(nullptr) {
    qDebug() << "Service - Start";
    init();
}

Service::~Service() {
}

void Service::init() {
    qDebug() << "Service - Init";
    // mVehicleSignalModel = new ccos::vehicle::vsm::HVehicleSignalModel;
}
