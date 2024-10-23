#include "BLEManager.h"
#import <CoreBluetooth/CoreBluetooth.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>
// #include <IOBluetooth/IOBluetooth.h>
#import <Foundation/Foundation.h>

// Приватный Objective-C++ класс для работы с CoreBluetooth
@interface BLEManagerObjC : NSObject <CBCentralManagerDelegate> {
    CBCentralManager *centralManager;
    NSMutableArray *connectedDevices;
}

- (void)retrieveConnectedDevices;
- (NSArray *)getConnectedDevices;

@end

@implementation BLEManagerObjC

- (instancetype)init {
    self = [super init];
    if (self) {
        centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
        connectedDevices = [[NSMutableArray alloc] init];
    }
    return self;
}

- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    if (central.state == CBManagerStatePoweredOn) {
        [self retrieveConnectedDevices];
    } else {
        [connectedDevices removeAllObjects];
    }
}

- (void)retrieveConnectedDevices {
    NSArray *services = @[]; // Пустой массив для всех подключенных устройств
    NSArray *devices = [centralManager retrieveConnectedPeripheralsWithServices:services];
    [connectedDevices removeAllObjects];

    for (CBPeripheral *device in devices) {
        [connectedDevices addObject:device.name ?: @"Unknown device"];
    }
}

- (NSArray *)getConnectedDevices {
    return connectedDevices;
}

@end


class BLEManager::BLEManagerPrivate {

public:
    BLEManagerPrivate() {
        objCManager = [[BLEManagerObjC alloc] init];
    }

    ~BLEManagerPrivate() {}

    BLEManagerObjC* objCManager;
};

// Конструктор/деструктор BLEManager
BLEManager::BLEManager() : d(new BLEManagerPrivate()) {}

BLEManager::~BLEManager() {
    delete d;
}

QStringList BLEManager::getConnectedDevices() {
    NSArray *devices = [d->objCManager getConnectedDevices];
    QStringList deviceList;

    for (NSString *deviceName in devices) {
        deviceList.append(QString::fromNSString(deviceName));
    }


    return deviceList;
}
