#ifndef SERIVCE_H
#define SERIVCE_H
#include <QObject>


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


class Service : public QObject {
    Q_OBJECT

public:
    explicit Service(QObject* parent = Q_NULLPTR);
    virtual ~Service();

    void init();

    // HVehicleSignalModel* getVehicleSignalModel() const;

private:
    ccos::vehicle::vsm::HVehicleSignalModel* mVehicleSignalModel = nullptr;
};

#endif  // SERIVCE_H
