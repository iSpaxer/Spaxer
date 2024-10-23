#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#pragma once

#include <QStringList>

// В объявлении используется только указатель на внутренний Objective-C класс
class BLEManager {
public:
    BLEManager();
    ~BLEManager();

    QStringList getConnectedDevices();

private:
    // Прячем Objective-C код за opaque pointer (чтобы не раскрывать Objective-C детали)
    class BLEManagerPrivate;
    BLEManagerPrivate* d;  // Указатель на частный (Objective-C) класс
};


#endif // BLEMANAGER_H
