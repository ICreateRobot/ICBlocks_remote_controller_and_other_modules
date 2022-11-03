#include <FastLED.h>

#include <Arduino.h>
#include <SPI.h>
#include "BluetoothSerial.h"

#include "stdlib.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#include <Keypad.h>
#include <stdio.h>
#include <string>
#include <Ticker.h>
#include <EEPROM.h>
#include "esp_system.h"

//
const int wdtTimeout = 3000;  //time in ms to trigger the watchdog
hw_timer_t *timer = NULL;



#define ANALOG_PIN_0   36  //右Y摇杆 上小下大
#define ANALOG_PIN_1   39  //右X摇杆 左小右大
#define ANALOG_PIN_2   34  //左Y摇杆 上小下大 
#define ANALOG_PIN_3   35  //左X摇杆 左小右大
#define ANALOG_PIN_4   32  //开关机电压采集
#define R_U            23
#define R_D            22
#define R_M            18
#define L_U            14
#define L_D            12
#define L_M            25
#define powerout       27
//WS2812灯
#define NUM_LEDS 1             // LED灯珠数量
#define DATA_PIN 4              // Arduino输出控制信号引脚
#define LED_TYPE WS2812         // LED灯带型号
#define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序
uint8_t max_bright = 100;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
CRGB leds[NUM_LEDS];            // 建立灯带leds
//
boolean doSacn = true;
boolean doConnect = false;
boolean connected = false;
//

#define MED_Threshold 300//死区
struct stu {

  uint16_t MAX;//极大值
  uint16_t MIN;//极小值
  uint16_t MED;//中值
  uint16_t Data;//当前值
  uint8_t  Equivalence;//校准后的值
} RR_X, RR_Y, RL_X, RL_Y;

//
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '5', '9'},
  {'2', '6', '0'},
  {'3', '7', '#'},
  {'4', '8', '*'}
};
byte rowPins[ROWS] = {26, 13, 15, 2};
byte colPins[COLS] = {5, 19, 21};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
char key;
//蓝牙
BLEAdvertisedDevice* pServer;
BLERemoteCharacteristic* pRemoteCharacteristic;
#define SERVICE_UUID "0000ffe0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ffe1-0000-1000-8000-00805f9b34fb"

uint8_t BLE_OUT[6];
BLEClient* pClient  = BLEDevice::createClient(); // 创建客户端
void NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

uint8_t BLE_Pattern = 4;//蓝牙的当前模式 0状态未知  1 打开搜索 搜索指定MAC 2 发现指定MAC的设备 等待连接  3已连接   4需要重新绑定设备 5根本不需要连接
String BLE_Mac;
unsigned long BLE_Search_Begin_Time;
uint8_t Last_BLE_Pattern;//上次蓝牙的模式
uint8_t Push_EN = false;
uint8_t Push_Buff_Count;

// 搜索到设备时回调功能
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (BLE_Pattern == 1) //有缓存的MAC地址
      {
        if ((String)(advertisedDevice.getAddress().toString().c_str()) == BLE_Mac) //寻找到UUID符合要求的主机
        {
          advertisedDevice.getScan()->stop(); // 停止当前扫描
          pServer = new BLEAdvertisedDevice(advertisedDevice); // 暂存设备
          doConnect = true;
          Serial.println("发现指定MAC的设备");
          BLE_Pattern = 2;//2 发现指定MAC的设备 等待连接
        }
      }
      if (BLE_Pattern == 4) //2 需要重新绑定
      {
        if (advertisedDevice.haveServiceUUID() && (String)(advertisedDevice.getServiceUUID().toString().c_str()) == SERVICE_UUID) //寻找到UUID符合要求的主机
          // if (advertisedDevice.haveServiceUUID()) //寻找到UUID符合要求的主机
        {
          advertisedDevice.getScan()->stop(); // 停止当前扫描
          pServer = new BLEAdvertisedDevice(advertisedDevice); // 暂存设备
          doConnect = true;
          Serial.print("发现想要连接的设备 其UUID为");
          Serial.println((String)(advertisedDevice.getServiceUUID().toString().c_str()));
        }
      }
    }
};

// 客户端与服务器连接与断开回调功能
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
      leds[0] = CRGB ( 0, 0, 250);
      FastLED.show();
      Serial.println("连接设备成功");
      if ( BLE_Pattern != 2) //说明不是通过指定MAC搜索来的 需要存MAC
      {
        Serial.println("保存设备MAC");
        BLE_Mac = (String)(pServer->getAddress().toString().c_str());
        Serial.println(BLE_Mac);
        EEPROM.writeString(2, BLE_Mac);
        EEPROM.commit();
        Serial.println(EEPROM.readString(2));
      }
      BLE_Pattern = 3;//已连接
      connected = true;
    }
    void onDisconnect(BLEClient* pclient)
    {
      Push_EN = false;
      connected = false;
      leds[0] = CRGB ( 255, 0, 0);
      FastLED.show();
      Serial.println("失去与设备的连接3");
      ESP.restart();
      leds[0] = CRGB ( 255, 0, 0);
      FastLED.show();
      Serial.println("失去与设备的连接2");

      doSacn = true;
      Serial.println("失去与设备的连接1");
      if (BLE_Pattern == 3)
        // ESP.restart();
        if (BLE_Pattern != 5)
        {
          BLE_Pattern = 0;//退回到开始状态
        }

    }
};
// 收到服务推送的数据时的回调函数
void NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  char buf[length + 1];
  for (size_t i = 0; i < length; i++) {
    buf[i] = pData[i];
  }
  buf[length] = 0;
  Serial.printf("该消息长度为: %d; 内容为: %s\r\n", length, buf);
}

// 用来连接设备获取其中的服务与特征
bool ConnectToServer(void) {
  BLEClient* pClient  = BLEDevice::createClient(); // 创建客户端

  pClient->setClientCallbacks(new MyClientCallback()); // 添加客户端与服务器连接与断开回调功能
  if (!pClient->connect(pServer)) { // 尝试连接设备
    return false;
  }
  Serial.println("连接设备成功");

  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID); // 尝试获取设备中的服务
  if (pRemoteService == nullptr) {
    Serial.println("获取服务失败");
    pClient->disconnect();
    return false;
  }
  Serial.println("获取服务成功");

  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID); // 尝试获取服务中的特征
  if (pRemoteCharacteristic == nullptr) {
    Serial.println("获取特性失败");
    pClient->disconnect();
    return false;
  }
  Serial.println("获取特征成功");

  if (pRemoteCharacteristic->canRead()) { // 如果特征值可以读取则读取数据
    Serial.printf("该特征值可以读取并且当前值为: %s\r\n", pRemoteCharacteristic->readValue().c_str());
  }
  Push_EN = true;
  if (pRemoteCharacteristic->canNotify()) { // 如果特征值启用了推送则添加推送接收处理
    pRemoteCharacteristic->registerForNotify(NotifyCallback);
  }
}
//试图连接蓝牙
void Connect_ble()
{
  if (doSacn == true)
  {
    if ((BLE_Pattern != 5)) //确定不是不需要连接
      if (BLE_Pattern != 3) //搜索之后太久没连上需要重新搜索
      {
        if ((millis() - BLE_Search_Begin_Time) > 3000)
        {
          BLE_Pattern = 0;
        }
        Last_BLE_Pattern = BLE_Pattern;
      }

    if (BLE_Pattern == 0) //蓝牙状态未知
    {
      BLE_Mac = EEPROM.readString(2);
      Serial.println("开始搜索设备5");
      BLE_Pattern = 1; //1 打开搜索 搜索指定MAC
      BLEDevice::getScan()->clearResults();
      BLEDevice::getScan()->start(3, false);
      BLE_Search_Begin_Time = millis();
      Serial.println(BLE_Mac);
    }
  }
  // 如果按键按下
  //if (Key.T)
  if (key == '#')
  {
    if (connected != 0)//如果连着设备就先断掉
    {
      pClient->disconnect();
      delay(500);
    }
    Serial.println("开始搜索随机设备4");
    BLE_Pattern = 4;//选择模式重新绑定设备
    BLEDevice::getScan()->clearResults();
    BLEDevice::getScan()->start(3, false);
    BLE_Search_Begin_Time = millis();
  }
  //  if (Key.O)
  //  {
  //    BLE_Pattern = 5;
  //    if (connected != 0)
  //    {
  //      pClient->disconnect();
  //    }
  //  }

  // 如果找到设备就尝试连接设备
  if (doConnect) {
    ConnectToServer();
    doConnect = false;
  }

}
//
void IRAM_ATTR resetModule() {
  ets_printf("reboot\n");
  //leds[0] = CRGB(0,0,0);
  leds[0] = CRGB::Red;          // 设置灯带中第一个灯珠颜色为红色，leds[0]为第一个灯珠，leds[1]为第二个灯珠
  FastLED.show();               // 更新LED色彩
  esp_restart();
  ESP.restart();
  while (1)
  {
    digitalWrite(powerout, LOW);
    digitalWrite(powerout, LOW);
    ets_printf("reboot1\n");
    ESP.restart();
  }
}
//
int SELECT_LEVEL = 0;
int BLOCKS_DATA = 0;
//uint8_t BLOCKS_DATA[] = {0xfe, 0, 0, 0, 0, 0, 0, 0xfc};
int analog_value0 = 0;
int analog_value1 = 0;
int analog_value2 = 0;
int analog_value3 = 0;
int analog_value4 = 0;
uint8_t MOTOR_Lock = 0;
uint8_t KEY_DOWN = 0;

uint8_t stop_flag = 0;
uint8_t power_flag = 0;
uint8_t power_key_flag = 1;

void Shutdown(void)
{
  //analog_value4 = analogRead(ANALOG_PIN_4);
  if (analogRead(ANALOG_PIN_4) > 4000)
  {
    power_key_flag = 1;
  }
  if ((power_flag == 0) && (power_key_flag == 1)) //开机
  {
    if (analogRead(ANALOG_PIN_4) <= 3800)
    {
      delay(10);
      if (analogRead(ANALOG_PIN_4) <= 3800)
      {
        delay(10);
        if (analogRead(ANALOG_PIN_4) <= 3800)
        {
          Serial.println("开机");

          digitalWrite(powerout, HIGH);
          delay(10);
          digitalWrite(powerout, HIGH);
          power_flag = 1;
          power_key_flag = 0;
          leds[0] = CRGB::Red;          // 设置灯带中第一个灯珠颜色为红色，leds[0]为第一个灯珠，leds[1]为第二个灯珠
          FastLED.show();               // 更新LED色彩
        }
      }
    }
  }
  else if ((power_flag == 1) && (power_key_flag == 1)) //关机
  { //Serial.println("关机1");
    if (analogRead(ANALOG_PIN_4) <= 2500)
    {
      Serial.println("关机2");
      delay(10);
      if (analogRead(ANALOG_PIN_4) <= 2500)
      { Serial.println("关机3");
        delay(10);
        if (analogRead(ANALOG_PIN_4) <= 2500)
        {
          leds[0] = CRGB ( 0, 0, 0);//leds[0] = CRGB::Red;          // 设置灯带中第一个灯珠颜色为红色，leds[0]为第一个灯珠，leds[1]为第二个灯珠
          FastLED.show();               // 更新LED色彩
          digitalWrite(powerout, LOW);
          delay(20);
          digitalWrite(powerout, LOW);
          power_flag = 0;
          power_key_flag = 0;
          Serial.println("关机");
          while (1);
        }
      }
    }
  }
}

void KEY_Control(void)//按键控制
{
  KEY_DOWN = KEY_Scan(0);
  Serial.println(KEY_DOWN);
}
uint8_t KEY_Scan(uint8_t mode)
{
  static uint8_t key_up = 1;   //按键松开标志
  uint8_t KEY0, KEY1, KEY2, KEY3;
  if (mode == 1)key_up = 1; //支持连按
  KEY0 = digitalRead(R_U);
  KEY1 = digitalRead(R_D);
  KEY2 = digitalRead(R_M);
  KEY3 = digitalRead(L_M);

  if (key_up && (KEY0 == 1 || KEY1 == 1 || KEY2 == 0 || KEY3 == 0))
  {
    delay(10);
    Serial.println(666);
    key_up = 0;
    if (KEY0 == 1)       return 1;
    else if (KEY1 == 1)  return 2;
    else if (KEY2 == 0)  return 3;
    else if (KEY3 == 0) return 4;
  }
  else if (KEY0 == 0 && KEY1 == 0 && KEY2 == 1 && KEY3 == 1)key_up = 1;
  return 0;   //无按键按下
}
uint8_t Light_flag = 0;
uint8_t MOTOR_Speed = 2; //速度档位默认中速
uint8_t Running_Speed = 2; //速度档位默认中速
uint8_t Expression_flag = 0;
uint8_t Voice_flag = 1;
uint8_t Expression_OPEN = 0;
uint8_t Voice_OPEN = 0;
void  Matrices_KEY_Control(void)  //矩阵按键控制
{
  if (key == '1')
  {
    BLOCKS_DATA = 14;//右转90度
  }
  else if (key == '2')
  {
    BLOCKS_DATA = 16;//前进一步
  }
  else if (key == '3')
  {
    BLOCKS_DATA = 13;//左转90度
  }
  else if (key == '4')
  {
    BLOCKS_DATA = 15;//后退一步
  }
  //开关灯
  else if (key == '6')  //开关灯按键5555
  {
    if (Light_flag == 0)
    {
      BLOCKS_DATA = 32;//开灯
      Light_flag = 1;
    }
    else
    {
      BLOCKS_DATA = 33;//关灯
      Light_flag = 0;
    }
  }
  //表情模块
  else if ((key == '5') && (Expression_OPEN == 1)) //切换表情
  {
    delay(200);
    if ((key == '5') && (Expression_OPEN == 1))
    {
      Expression_flag++;
      if (Expression_flag >= 6)
      {
        Expression_flag = 1;
      }
      if (Expression_flag == 1)
      {
        BLOCKS_DATA = 21;//表情1
      }
      else if (Expression_flag == 2)
      {
        BLOCKS_DATA = 22;//表情2
      }
      else if (Expression_flag == 3)
      {
        BLOCKS_DATA = 23;//表情3
      }
      else if (Expression_flag == 4)
      {
        BLOCKS_DATA = 24;//表情4
      }
      else if (Expression_flag == 5)
      {
        BLOCKS_DATA = 25;//表情5
      }
      Expression_OPEN = 0;
    }
  }
  //声音模块
  else if ((key == '8') && (Voice_OPEN == 1)) //切换声音按键
  {
    delay(200);
    if (key == '8')
    {
      Voice_flag++;
      if (Voice_flag > 6)
      {
        Voice_flag = 1;
      }
      if ( Voice_flag == 1 )
      {
        BLOCKS_DATA = 26;//警车
      }
      else if ( Voice_flag == 2 )
      {
        BLOCKS_DATA = 27;//消防车
      }
      else if ( Voice_flag == 3 )
      {
        BLOCKS_DATA = 28;//门铃声
      }
      else if ( Voice_flag == 4 )
      {
        BLOCKS_DATA = 29;//汽车鸣笛
      }
      else if ( Voice_flag == 5 )
      {
        BLOCKS_DATA = 30;//电话铃声
      }
      else if ( Voice_flag == 6 )
      {
        BLOCKS_DATA = 31;//播放录音
      }
      Voice_OPEN = 0;
    }
  }
  else if (key == '7')  //播放当前声音按键
  {
    delay(200);
    if (key == '7')
    {
      if ( Voice_flag == 1 )
      {
        BLOCKS_DATA = 26;//警车
      }
      else if ( Voice_flag == 2 )
      {
        BLOCKS_DATA = 27;//消防车
      }
      else if ( Voice_flag == 3 )
      {
        BLOCKS_DATA = 28;//门铃声
      }
      else if ( Voice_flag == 4 )
      {
        BLOCKS_DATA = 29;//汽车鸣笛
      }
      else if ( Voice_flag == 5 )
      {
        BLOCKS_DATA = 30;//电话铃声
      }
      else if ( Voice_flag == 6 )
      {
        BLOCKS_DATA = 31;//播放录音
      }
    }
  }
  //速度档位设定
  else if (key == '9')//减速
  {
    Running_Speed--;
    MOTOR_Speed--;
  }
  else if (key == '0')//加速
  {
    Running_Speed++;
    MOTOR_Speed++;
  }
  if (MOTOR_Speed <= 1)
  {
    Running_Speed = 1;
    MOTOR_Speed = 1;
  }
  if (MOTOR_Speed >= 3)
  {
    Running_Speed = 3;
    MOTOR_Speed = 3;
  }
}
void Calculate(struct stu *rocker) //处理校准参数
{
  if (rocker->Data > rocker->MAX)
    rocker->Data = rocker->MAX;
  if (rocker->Data < rocker->MIN)
    rocker->Data = rocker->MIN;
  uint8_t RE_Calculate = 100;
  if (rocker->Data > (rocker->MED + MED_Threshold)) //大于中值上阈值
  {
    RE_Calculate = (uint8_t)map(rocker->Data, rocker->MED + MED_Threshold, rocker->MAX, 101, 200);
  }
  if (rocker->Data < (rocker->MED - MED_Threshold)) //小于中下阈值
  {
    RE_Calculate = (uint8_t)map(rocker->Data, rocker->MIN, rocker->MED - MED_Threshold, 0, 99);
  }
  rocker->Equivalence = RE_Calculate;
}
void Read_Rocker()//读取摇杆
{
  ADC_Collection();
  RL_X.Data = analog_value0;
  RL_Y.Data = analog_value1;
  RR_X.Data = analog_value2;
  RR_Y.Data = analog_value3;

  Calculate(&RL_X);
  Calculate(&RL_Y);
  Calculate(&RR_X);
  Calculate(&RR_Y);
  // key = keypad.getKey();
  //if ((Key.Power_L + Key.Power_R) == 2)

  if ((digitalRead(R_M) == 0) && (digitalRead(L_M) == 0))
  { 
    uint16_t Trigger_time = 600;
    while ((digitalRead(R_M) == 0) && (digitalRead(L_M) == 0))
    {
      Trigger_time --;
      delay(1);
      Serial.println(Trigger_time);
      if (Trigger_time == 0)
      {
        ADC_Collection();
        //切换到校准模式初始化参数 取得中值
        RR_X.MAX = 0;
        RR_X.MIN = 0xffff;
        RR_X.MED = analog_value0;
        RR_Y.MAX = 0;
        RR_Y.MIN = 0xffff;
        RR_Y.MED = analog_value1;
        RL_X.MAX = 0;
        RL_X.MIN = 0xffff;
        RL_X.MED = analog_value2;
        RL_Y.MAX = 0;
        RL_Y.MIN = 0xffff;
        RL_Y.MED = analog_value3;
        leds[0] = CRGB(200, 0, 200);
        FastLED.show();               // 更新LED色彩
        break;
      }
      Serial.println(50);
    }
    while (!Trigger_time)
    {
      ADC_Collection();
      RL_X.Data = analog_value0;
      RL_Y.Data = analog_value1;
      RR_X.Data = analog_value2;
      RR_Y.Data = analog_value3;

      //极大值采集
      if (RL_X.Data > RL_X.MAX)
      {
        RL_X.MAX = RL_X.Data;
      }
      if (RL_Y.Data > RL_Y.MAX)
      {
        RL_Y.MAX = RL_Y.Data;
      }
      if (RR_X.Data > RR_X.MAX)
      {
        RR_X.MAX = RR_X.Data;
      }
      if (RR_Y.Data > RR_Y.MAX)
      {
        RR_Y.MAX = RR_Y.Data;
      }

      //极小值采集
      if (RL_X.Data < RL_X.MIN)
      {
        RL_X.MIN = RL_X.Data;
      }
      if (RL_Y.Data < RL_Y.MIN)
      {
        RL_Y.MIN = RL_Y.Data;
      }
      if (RR_X.Data < RR_X.MIN)
      {
        RR_X.MIN = RR_X.Data;
      }
      if (RR_Y.Data < RR_Y.MIN)
      {
        RR_Y.MIN = RR_Y.Data;
      }
      key = keypad.getKey();
      if (key == '*')
      { Serial.print("最大");
        EEPROM.writeShort(50, (int16_t)RL_X.MAX - MED_Threshold);
        EEPROM.writeShort(55, (int16_t)RL_X.MED);
        EEPROM.writeShort(60, (int16_t)RL_X.MIN + MED_Threshold);

        EEPROM.writeShort(65, (int16_t)RL_Y.MAX - MED_Threshold);
        EEPROM.writeShort(70, (int16_t)RL_Y.MED);
        EEPROM.writeShort(75, (int16_t)RL_Y.MIN + MED_Threshold);

        EEPROM.writeShort(80, (int16_t)RR_X.MAX - MED_Threshold);
        EEPROM.writeShort(85, (int16_t)RR_X.MED);
        EEPROM.writeShort(90, (int16_t)RR_X.MIN + MED_Threshold);

        EEPROM.writeShort(95, (int16_t)RR_Y.MAX - MED_Threshold);
        EEPROM.writeShort(100, (int16_t)RR_Y.MED);
        EEPROM.writeShort(105, (int16_t)RR_Y.MIN + MED_Threshold);

        EEPROM.end();
        leds[0] = CRGB(200, 0, 0);
        FastLED.show();               // 更新LED色彩
        break;
      }
    }
  }
}
void Read_Rocker_Parameter()//读摇杆的参数
{
  RL_X.MAX = (uint16_t)EEPROM.readShort(50);
  RL_X.MED = (uint16_t)EEPROM.readShort(55);
  RL_X.MIN = (uint16_t)EEPROM.readShort(60);

  RL_Y.MAX = (uint16_t)EEPROM.readShort(65);
  RL_Y.MED = (uint16_t)EEPROM.readShort(70);
  RL_Y.MIN = (uint16_t)EEPROM.readShort(75);

  RR_X.MAX = (uint16_t)EEPROM.readShort(80);
  RR_X.MED = (uint16_t)EEPROM.readShort(85);
  RR_X.MIN = (uint16_t)EEPROM.readShort(90);

  RR_Y.MAX = (uint16_t)EEPROM.readShort(95);
  RR_Y.MED = (uint16_t)EEPROM.readShort(100);
  RR_Y.MIN = (uint16_t)EEPROM.readShort(105);
}

#define N 10
int motor_flag = 0;
uint8_t running_flag = 0;
uint8_t Servo_flag = 0;
uint8_t Angle = 100;
uint8_t analog_value00[N] = {};
void ADC_Collection(void)
{
  int sum0 = 0;
  int sum1 = 0;
  int sum2 = 0;
  int sum3 = 0;
  for (uint8_t count = 0; count < N; count++)
  {
    sum0 += analogRead(ANALOG_PIN_0);
    sum1 += analogRead(ANALOG_PIN_1);
    sum2 += analogRead(ANALOG_PIN_2);
    sum3 += analogRead(ANALOG_PIN_3);
  }
  analog_value0 = (sum0 / N);
  analog_value1 = (sum1 / N);
  analog_value2 = (sum2 / N);
  analog_value3 = (sum3 / N);
}
void ADC_Control(void)
{
  //ADC_Collection();
  if ((motor_flag == 1) && (MOTOR_Lock != 1))
  {
    BLOCKS_DATA = 35;
    motor_flag = 0;
  }
  if (RL_Y.Equivalence < 50)
  {
    if (KEY_DOWN == 3)
    {
      MOTOR_Lock = 1;
    }
    if (MOTOR_Speed == 1)
    {
      BLOCKS_DATA = 40;//电机正转低速
      motor_flag = 1;
    }
    else if (MOTOR_Speed == 2)
    {
      BLOCKS_DATA = 41;//电机正转中速
      motor_flag = 1;
    }
    else if (MOTOR_Speed == 3)
    {
      BLOCKS_DATA = 42;//电机正转高速
      motor_flag = 1;
    }
  }
  else if (RL_Y.Equivalence > 150)
  {
    if (KEY_DOWN == 3)
    {
      MOTOR_Lock = 1;
    }
    if (MOTOR_Speed == 1)
    {
      BLOCKS_DATA = 43;//电机正转低速
      motor_flag = 1;
    }
    else if (MOTOR_Speed == 2)
    {
      BLOCKS_DATA = 44;//电机正转中速
      motor_flag = 1;
    }
    else if (MOTOR_Speed == 3)
    {
      BLOCKS_DATA = 45;//电机正转高速
      motor_flag = 1;
    }
  }
  else if ((RL_Y.Equivalence > 50) && (RL_Y.Equivalence < 150))
  {
    if (KEY_DOWN == 3)
    {
      MOTOR_Lock = 0;
      BLOCKS_DATA = 35;//电机停止
    }
  }
  ////左摇杆 上下左右
  if (running_flag == 1)
  {
    BLOCKS_DATA = 20;   //
    running_flag = 0;
  }
  if (RR_X.Equivalence < 50)//摇上
  {
    if (Running_Speed == 1)
    {
      BLOCKS_DATA = 46;//正转低速
      running_flag = 1;
    }
    else if (Running_Speed == 2)
    {
      BLOCKS_DATA = 47;//正转中速
      running_flag = 1;
    }
    else if (Running_Speed == 3)
    {
      BLOCKS_DATA = 48;//正转高速
      running_flag = 1;
    }
  }
  else if (RR_X.Equivalence > 150)//摇下
  {
    if (Running_Speed == 1)
    {
      BLOCKS_DATA = 49;//正转低速
      running_flag = 1;
    }
    else if (Running_Speed == 2)
    {
      BLOCKS_DATA = 50;//正转中速
      running_flag = 1;
    }
    else if (Running_Speed == 3)
    {
      BLOCKS_DATA = 51;//正转高速
      running_flag = 1;
    }
  }
  if (RR_Y.Equivalence < 50)//摇左
  {
    BLOCKS_DATA = 17;//左转直到
    running_flag = 1;
  }
  else if (RR_Y.Equivalence > 150)//摇右
  {
    BLOCKS_DATA = 18;//右转直到
    running_flag = 1;
  }
  //  }
}

//定时器中断

void setup() {
  Serial.begin(9600);
  //  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println();
  pinMode(ANALOG_PIN_0, INPUT);
  pinMode(ANALOG_PIN_1, INPUT);
  pinMode(ANALOG_PIN_2, INPUT);
  pinMode(ANALOG_PIN_3, INPUT);
  pinMode(ANALOG_PIN_4, INPUT);
  pinMode(R_U, INPUT);
  pinMode(R_D, INPUT);
  pinMode(R_M, INPUT_PULLUP);
  pinMode(L_U, INPUT);
  pinMode(L_D, INPUT);
  pinMode(L_M, INPUT_PULLUP);
  pinMode(powerout, OUTPUT);
  //灯带
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化灯带
  FastLED.setBrightness(max_bright);
  // leds[0] = CRGB(0,0,0);
  //leds[0] = CRGB::Yellow;          // 设置灯带中第一个灯珠颜色为红色，leds[0]为第一个灯珠，leds[1]为第二个灯珠
  // FastLED.show();               // 更新LED色彩
  delay(100);
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(80);
  
  EEPROM.begin(200);
  Read_Rocker_Parameter();//读摇杆的参数
  /*看门狗*/
  timer = timerBegin(0, 800, true);                  //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);  //attach callback
  timerAlarmWrite(timer, wdtTimeout * 1000, false); //set time in us
  timerAlarmEnable(timer);                          //enable interrupt

}
void loop() {
  Shutdown();
  timerWrite(timer, 0); //reset timer (feed watchdog)
  Serial.println("jdncskjzvfs");
  //  Serial.println(BLE_Mac);
  Read_Rocker();//读取摇杆
  //Serial.println(RL_X.Equivalence);
  //Serial.print(" ");
  Serial.println(RL_Y.Equivalence);
  // Serial.print(" ");
  //Serial.println(RR_X.Equivalence);
  //Serial.print(" ");
  Serial.println(RR_Y.Equivalence);
  // Serial.print(" ");
  key = keypad.getKey();
  if (key != NO_KEY) {
    // Serial.println(key);
  }
  else
  {
    key = 0;
    Expression_OPEN = 1;
    Voice_OPEN = 1;
  }
  //搜索连接蓝牙
  //connect_ble();
  Connect_ble();
  // ADC_Control();
  // KEY_Control();
  // Matrices_KEY_Control();
  // ADC_Control();
  if (connected)// 如果已经连接就可以向设备发送数据
  {
    KEY_Control();
    Matrices_KEY_Control();
    ADC_Control();
    if (BLOCKS_DATA != 0)// 如果可以向特征值写数据
    {
      Serial.println(BLOCKS_DATA);
      stop_flag = 1;
      pRemoteCharacteristic->writeValue(BLOCKS_DATA);
      delay(50);
      BLOCKS_DATA = 0;
    }

  }
}
