#include "staticdata.h"

const QString StaticData::serviceUuid = "{00001802-0000-1000-8000-00805F9B34FB}";
const QString StaticData::characteristicUuid = "{00005678-0000-1000-8000-00805F9B34FB}";

StaticData::StaticData(QObject *parent)
    : QObject{parent}
{}
