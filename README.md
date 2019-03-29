# Signaling-Blynk
Signaling Arduino+esp8266+SIM800L+Blynk

![IMG_20190327_152148](https://user-images.githubusercontent.com/45194485/55217894-aa80cc00-5211-11e9-9fe6-9ef0c5320408.jpg)

## На печатной плате могут разместиться:

* Arduino Nano V3.0
* NodeMcu v3 для выхода в интернет.
* SIM800L для мобильной связи.
* Радиомодуль NRF24L01 для приёма сигналов от беспроводных датчиков.
* Датчик температуры DS18B20.
* Термистор.
* Микроволновый датчик движения RCWL-0516.
* Зуммер.
* Микрофон.
* MOSFET транзисторы AO3400A для включения платы NodeMcu и внешних маломощных устройств.
* Разъёмы для подключения других датчиков.

Cигнализация модульная, можно выбрать ту конфигурацию, которая нужна в данном случае. Она может работать с SIM800L или NodeMcu на выбор, либо с обоими модулями одновременно. В последнем случае имеет два независимых канала связи, что более надёжно.

Прошивка для NodeMcu написана в среде Arduino IDE. Чтобы собрать проект, необходимо установить библиотеку для ESP8266 от сюда https://github.com/esp8266/Arduino#available-versions

## Создание приложения Blynk для телефона

Для начала вам нужно скачать на свой телефон приложение Blynk для Arduino, ESP8266,RPi

IOS:

https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8

Android:

https://play.google.com/store/apps/details?id=cc.blynk

Затем получить auth token.

Чтобы связать NodeMcu и приложение нам понадобится auth token, который нам вышлют на электронную почту.

Создайте новый аккаунт в приложении blynk.

![2](https://user-images.githubusercontent.com/45194485/55215558-44914600-520b-11e9-83ef-d56790a1d6a9.jpg)

Создайте новый проект. Выберите плату и тип подключения.

![3](https://user-images.githubusercontent.com/45194485/55215570-4a872700-520b-11e9-80d9-da4774018951.jpg)

После создания, auth token придет вам на email.

![4](https://user-images.githubusercontent.com/45194485/55215700-bc5f7080-520b-11e9-9ec5-0329a4cd24f8.jpg)

Добавляем себе виджет Terminal

![5](https://user-images.githubusercontent.com/45194485/55215784-00527580-520c-11e9-97f4-2049da615118.jpg)

Устанавливаем виртуальный пин V0. Переключатели как на фото.

![6](https://user-images.githubusercontent.com/45194485/55215829-1f510780-520c-11e9-97d4-02cd090b9f09.jpg)

Добавляем виджет Table.

![7](https://user-images.githubusercontent.com/45194485/55215895-48719800-520c-11e9-8fbc-186983fdd3b9.jpg)

Устанавливаем виртуальный пин V1, переключатели в положение YES.

![8](https://user-images.githubusercontent.com/45194485/55215983-8f5f8d80-520c-11e9-914e-e74b93e8b17d.jpg)

Размещаем удобно эти виджеты на экране, должно получиться примерно так:

![9](https://user-images.githubusercontent.com/45194485/55216022-af8f4c80-520c-11e9-938b-08773e5f7ba1.jpg)

В терминал будут выводиться сообщения сигнализации. Так же из него можно отправлять в сигнализацию команды управления.

О событиях будут сообщать следующие виджеты: Eventor, Notification, Email и Twitter.
Добавляем их:

![10](https://user-images.githubusercontent.com/45194485/55216112-f4b37e80-520c-11e9-946e-8cfcd0a7e015.jpg)

![11](https://user-images.githubusercontent.com/45194485/55216122-f715d880-520c-11e9-8aac-715401f1c9c2.jpg)

Добавляем новый Event. Устанавливаем виртуальный пин V2, прописываем условие срабатывания V2 is equal 1.

![12](https://user-images.githubusercontent.com/45194485/55216239-42c88200-520d-11e9-9f8a-38703c935b06.jpg)

![13](https://user-images.githubusercontent.com/45194485/55216258-4bb95380-520d-11e9-95b7-fa88081e3f91.jpg)

Вписываем свой email, на который будут приходить сообщения от сигнализации.

![14](https://user-images.githubusercontent.com/45194485/55216266-51169e00-520d-11e9-9893-f00853ec8d8b.jpg)

В конце получаем следующее:

![15](https://user-images.githubusercontent.com/45194485/55217312-47db0080-5210-11e9-86b8-e4477b2298de.png)

#	НАСТРОЙКА ДАТЧИКОВ

Инициализируем каждый датчик

Если у вас другой датчик (не из списка), опишите его аналогично, и добавьте в SENSORS_INIT

Номер параметра и значение (см. sensor.cpp)
1. пин ардуино
2. тип датчика. Типы датчиков перечислены в "sensor.h"
3. уникальное имя датчика. Будет выводиться в отчётах.
4. уровен на цифровом пине датчика в спокойном режиме (LOW или  HIGHT)
5. время на подготовку датчика при старте в секундах,
   		когда к нему нельзя обращаться, чтобы не получить ложные данные.
   		Если не указать, то он будет равен 10 сек.
6. порог срабатывания аналогового датчика, по умолчанию равен 200

Для беспроводных датчиков задаётся всего 3 параметра
Номер параметра и значение (см. sensor.cpp)
1. тип датчика IR_SENSOR или RF24_SENSOR
2. уникальное имя датчика. Будет выводиться в отчётах.
3. кодовое слово, передаваемое беспроводным датчиком при срабатывании

## Примеры описания датчиков:
  
Геркон. Для него зарезервирован 4 пин ардуины DOOR_PIN
  
Sensor(DOOR_PIN, DIGITAL_SENSOR,"DOOR", HIGH,0)
	
Датчик температуры DS18B20. Распаян на плате на пине DOOR_PIN
	
Sensor(DOOR_PIN, DS18B20,	"18B20", LOW, 10, 45)
	
Датчик температуры термистор. Заведён на плате на пин А7

Sensor(A7, TERMISTOR, "TERM", LOW, 10, 45)

### Все датчики, где на выходе либо низкий, либо высокий логический уровень
	
Датчик движения RCWL-0516
	
Sensor(PIN, DIGITAL_SENSOR,"RADAR",LOW)
	
Датчик огня с проверкой на ложное срабатывание 10 сек.
	
Sensor(PIN, CHECK_DIGITAL_SENSOR,	"FIRE", HIGH)	

### Беспроводные датчики:
	
Датчик с передающим ик-диодом
	
Sensor(IR_SENSOR,	"IR_0", IR_CODE)

Датчик с передающим Wi-Fi модулем RF24L01
	
Sensor(RF24_SENSOR,	"nRF_0", RF0_CODE)

