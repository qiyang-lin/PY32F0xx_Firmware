================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
此样例演示了ADC模块的温度传感器功能，程序下载后，串口每隔200ms打印一次当前检测的
温度值和对应的采样值。

Function descriptions:
This sample demonstrates the temperature sensor function of the ADC module. 
After downloading the program, the current temperature value and corresponding 
sampled value will be printed via the serial port every 200ms.
================================================================================
测试环境：
测试用板：PY32F002A_STK
MDK版本： 5.28
IAR版本： 9.20
GCC版本： GNU Arm Embedded Toolchain 10.3-2021.10

Test environment:
Test board: PY32F002A_STK
MDK Version: 5.28
IAR Version: 9.20
GCC Version: GNU Arm Embedded Toolchain 10.3-2021.10
================================================================================
使用步骤：
1. 根据实际VCC电压修改参数VDDA_APPLI
2. 编译下载程序到MCU，并运行
3. 串口每隔200ms输出当前的温度和对应的采样值

Example execution steps:
1. Modify the parameter VDDA_APPLI according to the actual VCC voltage.
2. Compile and download the programme to the MCU and run it.
3. Output the current temperature and the corresponding sampling value through 
the serial port every 200ms.
================================================================================
注意事项：
通过USB转TTL模块连接PC与STK板,STK板与USB转TTL模块的连线方式如下；
@PrintfConfigStart
STK板        USB转TTL模块
PA02(TX)  -->  RX
PA03(RX)  -->  TX
GND       -->  GND
UART配置为波特率115200，数据位8，停止位1，校验位None
@PrintfConfigEnd

Notes:
Connect the PC to the STK board through the USB to TTL module, and the connection
method between the STK board and the USB to TTL module is as follows:
@PrintfConfigStart
STK board USB to TTL module
PA02(TX)  -->  RX
PA03(RX)  -->  TX
GND       -->  GND
UART is configured as BaudRate 115200, data bit 8, stop bit 1, and parity None.
@PrintfConfigEnd
================================================================================