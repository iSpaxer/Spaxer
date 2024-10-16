#ifndef STATICDATA_H
#define STATICDATA_H

#include <QObject>

class StaticData : public QObject {
    Q_OBJECT
public:
    static const QString serviceUuid;
    static const QString characteristicUuid;

    explicit StaticData(QObject *parent = nullptr);

};







#endif // STATICDATA_H
