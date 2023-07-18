# balance-car
## 错误报刊🚫

### 1. PCB设计

#### 滑动开关
开关失效。原理图绘画的时候，把滑动开关其中两个脚都接到了VBAT上，铺铜的时候两脚都接入VBAT，导致开关即使左拨还是右拨中间引脚都会上电。

解决方案：给电源盒焊个开关，将电源盒引出的线接到开关上，再接入PCB中。

![switch](https://github.com/canwoh/balance-car/blob/master/Docs/images/p3.png?raw=true)

#### OLED模块：
GND与VCC刚好相反，导入PCB封装与实际不符。

![PCB中OLED](https://github.com/canwoh/balance-car/blob/master/Docs/images/p1.png?raw=true)

#### 电机模块：
注意同一轮子放在左右两侧，DRV8833 相同OUX输出出现两轮转向不一致的情况。

解决方案：右轮BO1与BO2在焊接时，反焊，左轮AO1->AO1 AO2->AO2  右轮BO1->BO2 BO2->BO1

![Motor](https://github.com/canwoh/balance-car/blob/master/Docs/images/p2.png?raw=true)

### 2.CubeMX初始化代码
#### Motors初始化
给电机少配置了两个TIM-PWM输出。PCB设计时给电机配置了两个GPIO，与两个TIM-PWM，实际需要4个PWM输出。

解决方案：后续软件上配置两个PWM Channel，利用中断控制GPIO电平，普通GPIO模拟PWM输出。

### 3.软件编写
#### 软件模拟IIC
* 时序控制错误，移植代码后没有调节时序延迟。查看SSD1306硬件手册时序在400KB/s模式下保底需要2.5us延迟。

* 软件模拟IIC，注意FreeRTOS任务切换时引起的时序问题。

#### FreeRTOS Task Stack Overflow问题
* 任务栈分配不足时，引发一些奇怪的现象。

#### 


## 项目改进🧐
* 项目采用MCU为STM32F103C8T6，Flash空间太小了，只有64KB。采用HAL+FreeRTOS+C/C++混合编写的组合还是太吃力了。编写代码大小受到较大限制。可尝试更换性能更好的MCU。

* 电源模块可以单独拎出来设计，贴片更加方便，压缩水平空间，降低体积。

* 有线器件，可以选择XH接插件方式进行连接。

* 额外设计一个USB串口兼下载模块，版上集成CMSIS-DAP模块(参考项目：vllink_lite)

* 项目中为了学习DMA，蓝牙模块采用UART+DMA模式，推荐采用中断UART接收形式。DMA接收定长输入的效果会更好一些，由于我所使用的上位机发送的信息是不定长且较短，不太好接收。当然DMA接收不定长输入也行，配合中断即可。

## 硬件参考

* 显示屏:OLED 0.96
* 惯性测量单元:MPU6050
* 电机驱动芯片:DRV8833
* 超声波:HC-SR04
* 蓝牙:HC-05
* 电机：淘宝随便买的
