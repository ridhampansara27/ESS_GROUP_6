
Group 6 – ESS Project  
Project Title: IoT Prototype: Smart Sensing via LoRa  

--------------------------------------------------------------------------------
📌 Project Description  
--------------------------------------------------------------------------------
This project implements a microcontroller-based IoT prototype designed for 
smart environmental sensing and wireless data communication using LoRa. The system components include:

- BME680 Sensor: Measures temperature, humidity, pressure, and gas resistance.
- STM32 NUCLEO-G071RB: Acts as the main microcontroller.
- SX1262 LoRa HAT: Enables long-range wireless data transmission over LoRa.
- UART Interface: Communicates with a PC terminal (e.g., PuTTY) for command input and data output.

The firmware supports:
- CLI commands via PuTTY (e.g., TEMP, HUM, PRESS, GAS, ALL, LORA SEND, LORA STATUS, SUM, MUL, etc.)
- Automatic and manual sensor data readout
- LoRa transmission of environmental data

--------------------------------------------------------------------------------
🧠 Team Members  
--------------------------------------------------------------------------------
- Ridham Pansara (45148)  
- Mishal Meethian Naina (44943)  
- Ndayambaje Celestin (44714)  
- Nasir Sani Ahmed (44749)  
- Abdullah Ashtar (468943)

--------------------------------------------------------------------------------
🗂 Folder & File Structure  
--------------------------------------------------------------------------------

| Folder/File Name            | Description                                                                  |
|-----------------------------|------------------------------------------------------------------------------|
| COMPONENT_CONNECTION/       | Images of components and physical wiring connections                         |
| DIAGRAMS/                   | Contains hardware diagram, software architecture, and flow diagram images    |
| PUTTY_DEMO/                 | Screenshots and video of CLI demo using PuTTY                                |
| MEETING/                    | Photo proof of team meetings                                                 |
| ESS_SensorDataRead/         | STM32CubeIDE project for continuous sensor data output via UART              |
| ESS_Prototype9/             | STM32CubeIDE project with CLI-based data retrieval functionality             |
| ESS_Prototype10/            | Final STM32CubeIDE project with full implementation (I2C + SPI + CLI + LoRa) |
| MEETING_LOGS_GROUP6.xlsx    | Log of meetings with date and purpose                                     |
| REQ_IMPL_TRACE_GROUP_6.xlsx | Requirements list, implementation status, and traceability matrix (3 sheets) |
| IOC_PIN.png                 | Screenshot of STM32CubeMX IOC pin configuration                              |
| PPT_GROUP6.pptx             | Final presentation slides for project demo                                   |
| README.txt                  | This documentation file                                                      |

--------------------------------------------------------------------------------
⚙️ How to Use the System  
--------------------------------------------------------------------------------

1. Connect Hardware:
   - Refer to Hardware_Diagram.png in the DIAGRAMS folder for correct wiring of:
     - BME680 (I2C)
     - SX1262 LoRa HAT (SPI)
     - UART-to-USB connection to PC

2. Flash Firmware:
   - Open STM32CubeIDE
   - Import the desired folder/project:
     - ESS_SensorDataRead for continuous mode
     - ESS_Prototype9 for CLI mode
     - ESS_Prototype10 for final full implementation
   - Connect the STM32 board and flash the code.

3. Open PuTTY Terminal:
   - Set Baud Rate: 115200  
   - Set COM Port: Check Device Manager (e.g., COM5)  
   - Choose Serial mode

4. Modes of Operation:

   - Continuous Mode (Folder: ESS_SensorDataRead)
     - Sensor data (Temp, Humidity, Pressure, Gas) is printed automatically.

   - CLI Mode (Folder: ESS_Prototype9)
     - Manually type and send commands over PuTTY:
       - TEMP, HUM, PRESS, GAS, ALL

   - Full Implementation Mode (Folder: ESS_Prototype10)
     - CLI Commands Supported:
       TEMP       → Read Temperature
       HUM        → Read Humidity
       PRESS      → Read Pressure
       GAS        → Read Gas Resistance
       ALL        → Read All Sensor Values
       LORA INIT  → Initialize LoRa Module
       LORA RESET → Reset LoRa Module
       LORA STATUS→ Check LoRa Status
       LORA SEND <cmd> or <msg> → Send Sensor Data or Message over LoRa
       SUM <a> <b> → Returns a+b
       SUB <a> <b> → Returns a-b
       MUL <a> <b> → Returns a*b
       DIV <a> <b> → Returns a/b
       START       → Starts the system

5. LoRa Transmission:
   - Sensor values are also sent via LoRa.
   - Requires a compatible LoRa receiver on the other side.

--------------------------------------------------------------------------------
📌 Additional Notes  
--------------------------------------------------------------------------------

- Powering Devices:
  - STM32 board is powered via micro-USB.
  - LoRa HAT is powered via separate USB or 3.3V pin from NUCLEO.

- UART Settings in PuTTY:
  - Terminal > Keyboard: Set Return Key sends: LF
  - Terminal > Local Echo: Enable, if needed to see typed commands.

- Debugging Tip:
  - If typing commands doesn’t respond in PuTTY, check:
    - UART pins and baud rate
    - Line ending mode
    - Whether the right STM32 firmware is flashed

- Project Duration:
  - June 13, 2025 – July 21, 2025  
  - Total of 13 team meetings were held (see MEETING_LOGS_GROUP6.xlsx)
  - Frequent group chat discussions for active problem-solving

--------------------------------------------------------------------------------
