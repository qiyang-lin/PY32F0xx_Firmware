================================================================================
                                样例使用说明
                             Sample Description
================================================================================
功能描述：
本样例主要读取DBGMCU->IDCODE寄存器和UID的值。
UID Word0表示LotNumber, Word1表示WaferNumber, Word2表示X和Y的坐标。

Function descriptions:
This sample mainly reads the values of DBGMCU->IDCODE register and UID.
UID Word0 means LotNumber, Word1 means WaferNumber, Word2 means X and Y coordinates.
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
1. 编译下载程序到MCU，并运行；
2. 仿真模式下在Watch窗口查看全局变量aShowDeviceID, aShowRevisionID,
   aShowCoordinate, aShowWaferNumber, aShowLotNumber的值。

Example execution steps:
1. Compile and download the program to the MCU and run it;
2. check the values of global variables aShowDeviceID, aShowRevisionID, 
aShowCoordinate, aShowWaferNumber, aShowLotNumber in the Watch window in 
simulation mode.
aShowCoordinate, aShowWaferNumber, aShowLotNumber in simulation mode.
================================================================================
注意事项：


================================================================================