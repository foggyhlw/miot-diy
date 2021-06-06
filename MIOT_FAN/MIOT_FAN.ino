/* *****************************************************************
 *
 * Download latest Blinker library here:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * 
 * Blinker is a cross-hardware, cross-platform solution for the IoT. 
 * It provides APP, device and server support, 
 * and uses public cloud services for data transmission and storage.
 * It can be used in smart home, data monitoring and other fields 
 * to help users build Internet of Things projects better and faster.
 * 
 * Make sure installed 2.7.4 or later ESP8266/Arduino package,
 * if use ESP8266 with Blinker.
 * https://github.com/esp8266/Arduino/releases
 * 
 * Make sure installed 1.0.4 or later ESP32/Arduino package,
 * if use ESP32 with Blinker.
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * Docs: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************
 * 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.5 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_FAN

#include <Blinker.h>
#include "Button2.h"

char auth[] = "device code";
char ssid[] = "ssid";
char pswd[] = "password";

const int FAN_gpio=5;
const int FAN_speed_low=14;
const int FAN_speed_medium=12;
const int FAN_speed_high=13;
const int FAN_direction_gpio=4;
const int FAN_manual_control=3;
Button2 button = Button2(FAN_manual_control);

bool oState = false;
bool hsState = false;
bool vsState = false;
uint8_t setLevel;

void button_click(Button2& btn){
  // Serial.print(current_speed);
  if( setLevel >= 3){
    set_FANSpeed(1);
  }
  else{
    set_FANSpeed(setLevel+1);
  }
}

void button_doubleClick(Button2& btn){
    oState = !oState;
    digitalWrite(FAN_gpio, oState);
}

void set_FANSpeed( uint8_t level ){
    digitalWrite(FAN_speed_low, LOW);
    digitalWrite(FAN_speed_medium, LOW);
    digitalWrite(FAN_speed_high, LOW);
    delay(40);
    if(level == 1 || level == 0){
      if(!digitalRead(FAN_speed_high) && !digitalRead(FAN_speed_medium))
      digitalWrite(FAN_speed_low, HIGH);
    }
    if(level == 2){
      if(!digitalRead(FAN_speed_low) && !digitalRead(FAN_speed_high))
      digitalWrite(FAN_speed_medium, HIGH);
    }
    if(level == 3){
      if(!digitalRead(FAN_speed_low) && !digitalRead(FAN_speed_medium))
      digitalWrite(FAN_speed_high, HIGH);
    }
    setLevel = level;
}

void miotLevel(uint8_t level)
{
    BLINKER_LOG("need set level: ", level);
    // 0:AUTO MODE, 1-3 LEVEL
    set_FANSpeed(level);

    BlinkerMIOT.level(level);
    BlinkerMIOT.print();
}

void miotHSwingState(const String & state)
{
    BLINKER_LOG("need set HSwing state: ", state);
    // horizontal-swing

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);

        BlinkerMIOT.hswing("on");
        BlinkerMIOT.print();

        hsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);

        BlinkerMIOT.hswing("off");
        BlinkerMIOT.print();

        hsState = false;
    }
}

void miotVSwingState(const String & state)
{
    BLINKER_LOG("need set VSwing state: ", state);
    // vertical-swing

    if (state == BLINKER_CMD_ON) {
        digitalWrite(LED_BUILTIN, HIGH);

        BlinkerMIOT.vswing("on");
        BlinkerMIOT.print();

        vsState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);

        BlinkerMIOT.vswing("off");
        BlinkerMIOT.print();

        vsState = false;
    }
}

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        // digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(FAN_gpio, HIGH);
        set_FANSpeed(1);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {
        // digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(FAN_gpio, LOW);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        oState = false;
    }
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.hswing(hsState ? "on" : "off");
            BlinkerMIOT.vswing(vsState ? "on" : "off");
            BlinkerMIOT.level(setLevel);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.hswing(hsState ? "on" : "off");
            BlinkerMIOT.vswing(vsState ? "on" : "off");
            BlinkerMIOT.level(setLevel);
            BlinkerMIOT.print();
            break;
    }
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    Serial.begin(115200);

    button.setClickHandler(button_click);
    button.setDoubleClickHandler(button_doubleClick);

    pinMode(FAN_gpio, OUTPUT);
    pinMode(FAN_direction_gpio, OUTPUT);
    pinMode(FAN_speed_high, OUTPUT);
    pinMode(FAN_speed_medium, OUTPUT);
    pinMode(FAN_speed_low, OUTPUT);

    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachHSwing(miotHSwingState);
    BlinkerMIOT.attachVSwing(miotVSwingState);
    BlinkerMIOT.attachLevel(miotLevel);
    BlinkerMIOT.attachQuery(miotQuery);
}

void loop()
{
    Blinker.run();
    button.loop();
}