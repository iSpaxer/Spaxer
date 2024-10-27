# Spaxer
Приложения для копирования текста~/изображений/файлов~ на одном устройстве и вставке на другом.

## Технологии
- Qt 6.7
- C++ 11
- Objective-C
- Objective-C++
- Bluetooth BLE
- qmake
- QWidgets
- XCode

## OS
Поддерживаются 3 операционные системы.  

Операционная система / git ветка 
- iOS/ios
- MacOS/ios
- Linux/linux

## Скриншоты
![telegram-cloud-photo-size-2-5255840467998860172-y](https://github.com/user-attachments/assets/97a47526-a125-4855-a0f7-1158737f54fa)
![telegram-cloud-photo-size-2-5255840467998860173-y](https://github.com/user-attachments/assets/6dce0652-f3d9-46ba-8164-033b0276392b)
![image](https://github.com/user-attachments/assets/1ffdac03-1bde-4a16-a873-12f7f5f04d17)
![telegram-cloud-photo-size-2-5255840467998860179-x](https://github.com/user-attachments/assets/83506b5e-7d81-4157-87a9-22f60e46895b)


## Todo
- Сделать рефактор кода:
    + Объединить ветки ios/linux
    + Разбить класс SearchWidget
    + Разложить на более удобные папки (например класс BleServer и класс BleClient должны быть в одной директории)

- На Linux починить сохранение в файл и подключение к устройству    
- На iOS убрать savearray, чтобы не было черных элементов в UI.
- Ошибка The value's length is invalid в CoreBluetooth (код ошибки 13) указывает на то, что длина данных, отправляемых через BLE, превышает максимальный размер, поддерживаемый характеристикой.
Для BLE передача данных ограничена, и максимальная длина зависит от параметров характеристики и MTU (Maximum Transmission Unit), который обычно ограничен до 20 байт по умолчанию, но может быть увеличен, если клиент и сервер поддерживают более крупные пакеты.
- Передача изображений
- Передача файлов
- Реализовтать настройки
- Сбилдить под OS Adnroid
- Сбилдить под OS Windows
