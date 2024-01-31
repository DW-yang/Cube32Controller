# Cube32Controller

## 简介
Cube32Controller是Cube小车项目的控制端程序
基于 合宙ESP32C3 开发板
使用 VSCode + PlatformIO 开发

## 主要特点
1. 使用ESP-NOW技术进行通信，可控范围大，控制延迟低

## 使用方法

#### 材料
1. 合宙ESP32C3 开发板 x1
2. JoyStick模块 x1
3. 18650锂电池充放电升压控制板 x1
4. 锂电池 x1

#### 接线
1. 锂电池连接充放电模块，将充放电模块的输出电压调至5V
2. 开发板 5V连接放电模块正极，GND连接负极，IO0 连接 JoyStick模块VRX，IO1 连接 JoyStick模块VRY，IO2 连接 JoyStick模块SW
3. JoyStick模块 +5V 连接开发板上的 3.3V电源引脚，GND 连接 开发板GND引脚

#### 烧录程序
1. Clone本仓库
2. 在源码中配置受控端MAC地址
3. 编译并烧录程序