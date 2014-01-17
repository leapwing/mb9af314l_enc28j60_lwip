### <a name="MB9AF314L"></a>MCU Description ##
**MB9AF341L**  

32-bit ARM Cortex-M3 Core (r2p1)  
 - Up to 40MHz Frequency Operation  
 - Nested Vectored Interrupt Controller (NVIC)  
 - 24-bit System timer (SysTick)

On-chip Memories  
 - 256 Kbyte Flash  
 -  16 Kbyte SRAM for high-performance CPU  
 -  16 Kbyte SRAM for CPU/DMA Controller

USB Interface (Device / Host)  
 - USB2.0 Full/Low Speed supported  
 - Max. 6 EndPoint supported

Multi-function Serial Interface (Max. 8channels)  
 - UART, CSIO, LIN, I2C

DMA Controller (8channels)

A/D Converter (Max. 16channels)  
 - 12-bit A/D Converter  
 - Built-in 3unit  (MB9AF312L, F314L built-in 2unit)

Base Timer (Max. 8channels)  
 - Operation mode: 16-bit PWM, 16-bit PPG, 16/32-bit reload, 16/32-bit PWC

General Purpose I/O Port  
 - 51 fast I/O Ports

Multi-function Timer (Max. 2unit)

Quadrature Position/Revolution Counter (QPRC) (Max. 2unit)

Dual Timer (Two 32/16bit Down Counter)

Watch Counter

External Interrupt Controller Unit  
 - Up to 16 external vectors  
 - Include one non-maskable interrupt(NMI)

Watchdog Timer (2channels)

CRC (Cyclic Redundancy Check) Accelerator

Clock and Reset  
 - 5 clock sources (2 ext. osc, 2 CR osc, and PLL)  
 - Reset sources: INITX Pins, POR, SW, Watchdog, LVD, CSV

Clock Super Visor (CSV)  
 - Ext. OSC clock failure (clock stop) detect  
 - Ext. OSC frequency anomaly detect

Low Voltage Detector (LVD)  
 - LVD1: error reporting via interrupt  
 - LVD2: auto-reset operation

Low Power Mode  
 - 3 power saving modes (SLEEP, TIMER, STOP)

Debug  
 - Serial Wire JTAG Debug Port (SWJ-DP)

----------
### <a name="ENC28J60"></a>Stand-Alone Ethernet Controller with SPI Interface 
**ENC28J60**  

Ethernet Controller Features  

- IEEE 802.3™ Compatible Ethernet Controller
- Fully Compatible with 10/100/1000Base-T Networks
- Integrated MAC and 10Base-T PHY
- Supports One 10Base-T Port with Automatic Polarity Detection and Correction
- Supports Full and Half-Duplex modes
- Programmable Automatic Retransmit on Collision
- Programmable Padding and CRC Generation
- Programmable Automatic Rejection of Erroneous Packets
- SPI Interface with Clock Speeds up to 20 MHz

Buffer  

- 8-Kbyte Transmit/Receive Packet Dual Port SRAM
- Configurable Transmit/Receive Buffer Size
- Hardware Managed Circular Receive FIFO
- Byte-Wide Random and Sequential Access with Auto-Increment
- Internal DMA for Fast Data Movement
- Hardware Assisted Checksum Calculation for Various Network Protocols

Medium Access Controller (MAC) Features  

- Supports Unicast, Multicast and Broadcast Packets
- Programmable Receive Packet Filtering and Wake-up Host on Logical AND or OR of the Following:
	- Unicast destination address
	- Multicast address
	- Broadcast address
	- Magic Packet™
	- Group destination addresses as defined by
	- 64-bit Hash Table
	- Programmable Pattern Matching of up to 64 bytes at user-defined offset

Physical Layer (PHY) Features  

- Loopback mode
- Two Programmable LED Outputs for LINK, TX, RX, Collision and Full/Half-Duplex Status  

Operational  

- Six Interrupt Sources and One Interrupt Output Pin
- 25 MHz Clock Input Requirement
- Clock Out Pin with Programmable Prescaler
- Operating Voltage of 3.1V to 3.6V (3.3V typical)
- 5V Tolerant Inputs
- Temperature Range: -40°C to +85°C Industrial, 0°C to +70°C Commercial (SSOP only)
- 28-Pin SPDIP, SSOP, SOIC, QFN Packages  


----------
