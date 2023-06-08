#ifndef DATA_MODEL_MODEL_H
#define DATA_MODEL_MODEL_H

#include <QMap>
#include "DataModel.h"

class DataModelMain : public DataModel {
public:
    QString mObjectName = QString("DataModelMain");
    QMap<int, QString> mSheetName = QMap<int, QString>();
    QMap<int, QString> mContextName = QMap<int, QString>();
    int mCusorPosition = 0;
};


#endif  // DATA_MODEL_MODEL_H
