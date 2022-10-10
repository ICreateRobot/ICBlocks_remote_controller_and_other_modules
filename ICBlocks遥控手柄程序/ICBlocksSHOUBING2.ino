#include <BLEDevice.h>
#include "BluetoothSerial.h"
#include <Keypad.h>
#include <FastLED.h>

//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif
//
//BluetoothSerial SerialBT;

#define SERVICE_UUID "0000ffe0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ffe1-0000-1000-8000-00805f9b34fb"

#define ANALOG_PIN_0   36  //右Y摇杆 上小下大
#define ANALOG_PIN_1   39  //右X摇杆 左小右大
#define ANALOG_PIN_2   34  //左Y摇杆 上小下大 
#define ANALOG_PIN_3   35  //左X摇杆 左小右大
#define ANALOG_PIN_4   32  //开关机电压采集
#define R_U            14
#define R_D            12
#define R_M            18
#define L_U            22
#define L_D            23
#define L_M            25
//#define LED            4
//#define powerin        32
#define powerout       27
//WS2812灯
#define NUM_LEDS 2             // LED灯珠数量
#define DATA_PIN 4              // Arduino输出控制信号引脚
#define LED_TYPE WS2812         // LED灯带型号
#define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序
uint8_t max_bright = 100;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
CRGB leds[NUM_LEDS];            // 建立灯带leds
//
boolean doSacn = true;
boolean doConnect = false;
boolean connected = false;

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
BLEAdvertisedDevice* pServer;
BLERemoteCharacteristic* pRemoteCharacteristic;

// 搜索到设备时回调功能
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.haveServiceUUID() && (String)(advertisedDevice.getServiceUUID().toString().c_str()) == SERVICE_UUID) //寻找到UUID符合要求的主机
      {
        advertisedDevice.getScan()->stop(); // 停止当前扫描
        pServer = new BLEAdvertisedDevice(advertisedDevice); // 暂存设备
        doConnect = true;
        Serial.println("发现想要连接的设备");
      }
    }
};

// 客户端与服务器连接与断开回调功能
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {}
    void onDisconnect(BLEClient* pclient) {
      connected = false;
      ESP.restart();  //重启复位esp32
      leds[0] = CRGB ( 255, 0, 0);
      FastLED.show();
      Serial.println("失去与设备的连接");
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
  if (pRemoteCharacteristic->canNotify()) { // 如果特征值启用了推送则添加推送接收处理
    pRemoteCharacteristic->registerForNotify(NotifyCallback);
  }
}
int SELECT_LEVEL = 0;
int BLOCKS_DATA = 0;
int analog_value0 = 0;
int analog_value1 = 0;
int analog_value2 = 0;
int analog_value3 = 0;
int analog_value4 = 0;
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
 // pinMode(LED, OUTPUT);
  //pinMode(powerin, INPUT);
  pinMode(powerout, OUTPUT);
  //灯带
  LEDS.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化灯带
  FastLED.setBrightness(max_bright);
  leds[0] = CRGB ( 255, 0, 0);
  FastLED.show();               // 更新LED色彩
  delay(100);
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(80);
}
char key;
uint8_t stop_flag = 0;
uint8_t power_flag = 0;
uint8_t power_key_flag = 1;

void Shutdown(void)
{
  //digitalWrite(powerout, HIGH);
  analog_value4 = analogRead(ANALOG_PIN_4);
  //Serial.println(analog_value4);
  if (analog_value4 > 4000)
  {
    power_key_flag = 1;
  }
  if ((power_flag == 0) && (power_key_flag == 1)) //开机
  {
    //if (digitalRead(powerin) == 0)
    if (analog_value4 <= 1000)
    {
      delay(200);
      if (analog_value4 <= 1000)
      {
        delay(200);
        if (analog_value4 <= 1000)
        {
          digitalWrite(powerout, HIGH);
          delay(100);
          digitalWrite(powerout, HIGH);
          power_flag = 1;
          power_key_flag = 0;
          Serial.println(55);
        }
      }
    }
  }
  else if ((power_flag == 1) && (power_key_flag == 1)) //关机
  { //Serial.println(61);
    //if (digitalRead(powerin) == 0)
    if (analog_value4 <= 1000)
    {
      //Serial.println(62);
      //if (digitalRead(powerin) == 0)
      if (analog_value4 <= 2000)
      {
        Serial.println(66);
        digitalWrite(powerout, LOW);
        delay(20);
        digitalWrite(powerout, LOW);
        power_flag = 0;
        power_key_flag = 0;
        while (1);

      }
    }
  }
}
uint8_t Expression_OPEN = 0;
uint8_t Voice_OPEN = 0;
void loop() {
  Shutdown();
  key = keypad.getKey();
  
   // Serial.println(key);
  if (key != NO_KEY) {
    Serial.println(key);
  }
  else
  {
    key = 0;
    Expression_OPEN = 1;
    Voice_OPEN = 1;
  }
  // 如果需要扫描则进行扫描
  if (connected == 0)
  {
    //搜索连接蓝牙
    if (key == '#') {
 //if (digitalRead(R_U) == 1) {
      Serial.println("开始搜索设备");
      BLEDevice::getScan()->clearResults();
      BLEDevice::getScan()->start(0); // 持续搜索设备
    }
  }
  // 如果找到设备就尝试连接设备
  if (doConnect) {
    if (ConnectToServer()) {
      connected = true;
      leds[0] = CRGB::Blue;          // 设置灯带中第一个灯珠颜色为红色，leds[0]为第一个灯珠，leds[1]为第二个灯珠
      FastLED.show();               // 更新LED色彩
    }
    else {
      doSacn = true;
    }
    doConnect = false;
  }
  // ADC_Control();
  if (connected) {       // 如果已经连接就可以向设备发送数据
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
    //delay(500);
  }
}
uint8_t MOTOR_Lock = 0;
uint8_t KEY_DOWN = 0;
void KEY_Control(void)//按键控制
{
  KEY_DOWN = KEY_Scan(0);
  Serial.println(KEY_DOWN);
  if (digitalRead(R_M) == 1)
  {
    //MOTOR_Lock=1;
  }
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
  //  Serial.println(KEY0);
  //  Serial.println(KEY1);
  //  Serial.println(KEY2);
  //  Serial.println(KEY3);
  //  Serial.println(key_up);
  //  delay(500);
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
  else if (key == '6')  //开关灯按键
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
  //
}
int running_flag = 0;
int motor_flag = 0;
void ADC_Control(void)
{
  analog_value0 = analogRead(ANALOG_PIN_0);
  analog_value1 = analogRead(ANALOG_PIN_1);
  analog_value2 = analogRead(ANALOG_PIN_2);
  analog_value3 = analogRead(ANALOG_PIN_3);
  //  Serial.println(analog_value0);
  //  Serial.println(analog_value1);
  //  Serial.println(analog_value2);
  //  Serial.println(analog_value3);
  //delay(1000);
  //摇杆运动标志

  ////右摇杆 上下左右
  if ((motor_flag == 1) && (MOTOR_Lock != 1))
  {
    BLOCKS_DATA = 35;
    motor_flag = 0;
  }
  //  if (((analog_value2 > 1000) && (analog_value2 < 3000)) || ((analog_value3 > 1000) && (analog_value3 < 3000)))
  //  {
  if (analog_value1 < 1000)
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
  else if (analog_value1 > 2800)
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
  else if ((analog_value1 > 1000) && (analog_value1 < 2800))
  {
    if (KEY_DOWN == 3)
    {
      MOTOR_Lock = 0;
      BLOCKS_DATA = 35;//电机停止
    }
  }
  //  }
  /*
    if (analog_value1 < 1000)
    {
    // BLOCKS_DATA = 19;
    motor_flag = 1;
    }
    else if (analog_value1 > 3000)
    {
    //BLOCKS_DATA = 15;
    motor_flag = 1;
    }
  */
  ////左摇杆 上下左右
  if (running_flag == 1)
  {
    BLOCKS_DATA = 20;   //
    running_flag = 0;
  }
  //  if ((analog_value0 > 1000) && (analog_value0 < 2800))
  //  {
  if (analog_value2 < 1000)//摇上
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
  else if (analog_value2 > 3000)//摇下
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
  if (analog_value3 < 1000)//摇左
  {
    BLOCKS_DATA = 17;//左转直到
    running_flag = 1;
  }
  else if (analog_value3 > 3000)//摇右
  {
    BLOCKS_DATA = 18;//右转直到
    running_flag = 1;
  }
  //  }
}
