# DIWA-1-Can-Satellite
## Amateur Atmospheric Data Acquisition Can Satellite

Contributed in the creation of DIWA-1, a student-built Can Satellite that collects atmospheric data such as pressure, altitude, and temperature through the main MPL3115A2 sensor. Programmed the ESP32-DevKit V1 microcontroller which recieves the data from the sensor and sends it to a ground reciever connected to a laptop displaying the data. Also helped in engineering and wiring the electronics inside the CanSat's compact structure. Do note that some of the code was adapted from official library documentation and examples.

<img width="641" height="857" alt="image" src="https://github.com/user-attachments/assets/809a441a-968f-4475-a6b2-5f53d95257ed" />

## Hardware List
**Breadboard** \
**ESP32 DEVKIT V1 38-Pin** \
**Adafruit MPL3115A2** \
**9V Battery** \
**MB102 Power Module** \
**3D Printed Can Structure** | PLA | 76mm x 136mm \
**Parachute** | Ripstop Nylon Fabric and Polyester Rope | 0.677m Diameter | 0.36^2 Surface Area 


## Static Test Table 
<img width="855" height="712" alt="image" src="https://github.com/user-attachments/assets/c602aa54-cebc-4f92-91d8-a666fccb5341" />

The averaged sensor measurements demonstrate strong compliance with verification criteria. Temperature at 31.214°C falls well within the ±1–2°C tolerance of an actual value around 31°C, with a minimal deviation of just 0.214°C. The pressure averages 1010.6625 hPa which coordinates inside the local range of 1005–1012 hPa. Additionally, the altitude at 10.413 m aligns precisely with the ±3–5 m threshold describing the elevation of the area.

## Code Sources and Credits: 
- The master and slave microcontroller code is based on the ESP-NOW Broadcast Master and ESP-NOW Broadcast Slave examples by Lucas Saavedra Vaz (2024), included with the Arduino-ESP32 core by Espressif (github.com/espressif/arduino-esp32). 
- Sensor readings from the MPL3115A2 (pressure, altitude, and temperature) use the Adafruit MPL3115A2 Library by Adafruit (github.com/adafruit/Adafruit_MPL3115A2_Library) 
- I adapted these for DIWA-1 by combining sensor readings with ESP-NOW transmission, changing the SI units by conversion, formatting the data, and adding the data logging. The wiring, integration, testing, and data analysis were also done by me.

