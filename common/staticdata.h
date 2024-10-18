#ifndef STATICDATA_H
#define STATICDATA_H

#include <QObject>

class StaticData : public QObject {
    Q_OBJECT
public:
    static const QString SERVICE_UUID;
    static const QString CHARACTERISTIC_UUID;

    explicit StaticData(QObject *parent = nullptr);

};







#endif // STATICDATA_H
