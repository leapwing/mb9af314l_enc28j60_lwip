# MB9AF314L\_ENC28J60\_LwIP Firmware #

This example project is a port of the LwIP (Lightweight IP) TCP/IP suite
including a small webserver to the FUJITSU MB9AF314L & MICROCHIP ENC28J60.

### Hardware Info ###

Target MCU: [Fujitsu **MB9AF314L**][MB9AF314L]

Ethernet controller: [Microchip **ENC28J60**][ENC28J60]

Library version: [**lwip 1.4.1**][LwIP]

More other info: [**Link**](/doc/hardware.md)

### Interface ###

SPI (MFS:CH7) `4MHz baudrate`   
UART (MFS:CH4) `115K baudrate`   
LED x3 (GPIO)    
Joystick x1 (GPIO)  
potentiomenter x1 (AN01)	  
ENC28J60 RESET & SPI_SS & INT(GPIO)

### Link & Documents ###
- [Main Site](/)
- [Developer's blog][BLOG]
- [More Documents(Chinese)](/doc/zh_TW/) 
	- [About 關於](/doc/zh_TW/ABOUT.md)
	- [Speed Measure 速度量測](/doc/zh_TW/MEASURE.md)
	- [Other Info 其他訊息](/doc/zh_TW/OTHER.md)
  
### Screenshot ###
![demo_1](https://lh6.googleusercontent.com/-bVWokD-j00k/UtjVfMwL02I/AAAAAAAAAU4/dA4XB3QyDwM/w607-h659-no/mb9af314l_enc28j60_lwip_demo1.jpg)  
▲ Demo1 Screen    
![demo_2](https://lh3.googleusercontent.com/-RjI8artzSNU/UtjVfe_OFaI/AAAAAAAAAUo/D_Glndz3YuM/w723-h659-no/mb9af314l_enc28j60_lwip_demo2.jpg)  
▲ Demo2 Screen  

[MB9AF314L]: http://www.spansion.com/Products/microcontrollers/32-bit-ARM-Core/fm3/Pages/overview_32fm3.aspx "FM3 Family"
[ENC28J60]: http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en022889 "ENC28J60"
[LwIP]: http://savannah.nongnu.org/projects/lwip "A Lightweight TCP/IP stack"
[BLOG]: /