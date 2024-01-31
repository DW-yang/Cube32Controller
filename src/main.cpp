#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define INPUT_X 0 // JoyStick模块X轴
#define INPUT_Y 1 // JoyStick模块Y轴
#define SW 2  // JoyStick模块按下按钮
uint8_t cube8266[] = {0xE0, 0x98, 0x06, 0x1F, 0x34, 0xCD};  // 受控端MAC地址

// 消息结构体
typedef struct cube_message
{
  bool isCMD;
  uint8_t data[8];
} cube_message;

// 全局变量
cube_message msg;
esp_now_peer_info_t peerInfo;

// 函数定义
void onDataSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  // Serial.println("Send a package.");
}

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  // Serial.println("Receive a heartbeat package.");
}

void setup()
{
  // 初始化串口:
  Serial.begin(115200);
  delay(100);
  Serial.println("Cube32 Controller power on.");
  // GPIO配置
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INPUT_X, INPUT);
  pinMode(INPUT_Y, INPUT);
  pinMode(SW, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
  // espnow配置
  WiFi.mode(WIFI_MODE_STA);
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP_NOW");
    return;
  }
  esp_now_register_send_cb(onDataSend);
  esp_now_register_recv_cb(onDataRecv);
  memcpy(peerInfo.peer_addr, cube8266, sizeof(cube8266));
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Added peer.");
  Serial.println(WiFi.macAddress());
}

void loop()
{
  msg.isCMD = false;
  msg.data[0] = 0x00;
  msg.data[1] = 0x00;
  msg.data[2] = 0x00;
  msg.data[3] = 0x00;
  if (analogRead(INPUT_Y) <= 100){
    msg.data[0] = 255;
    msg.data[3] = 255;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Up");
  }
  else if (analogRead(INPUT_Y) >= 4000){
    msg.data[1] = 255;
    msg.data[2] = 255;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Down");
  }
  if (analogRead(INPUT_Y) <= 100 && analogRead(INPUT_X) <= 100){
    msg.data[0] = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Up and Left");
  }
  if (analogRead(INPUT_Y) <= 100 && analogRead(INPUT_X) >= 4000){
    msg.data[3] = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Up and Right");
  }
  if (analogRead(INPUT_Y) >= 4000 && analogRead(INPUT_X) <= 100){
    msg.data[1] = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Down and Left");
  }
  if (analogRead(INPUT_Y) >= 4000 && analogRead(INPUT_X) >= 4000){
    msg.data[2] = 0;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Down and Right");
  }
  if (analogRead(INPUT_Y) > 100 && analogRead(INPUT_Y) < 4000 && analogRead(INPUT_X) <= 100){
    msg.data[1] = 255;
    msg.data[3] = 255;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Left");
  }
  if (analogRead(INPUT_Y) > 100 && analogRead(INPUT_Y) < 4000 && analogRead(INPUT_X) >= 4000){
    msg.data[0] = 255;
    msg.data[2] = 255;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Right");
  }
  if (digitalRead(SW) == LOW){
    msg.isCMD = true;
    msg.data[0] = 0x00;
    msg.data[1] = 0x00;
    msg.data[2] = 0x00;
    msg.data[3] = 0x00;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Switch pressed.");
    delay(100);
  }
  esp_now_send(cube8266, (uint8_t *) &msg, sizeof(msg));
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}