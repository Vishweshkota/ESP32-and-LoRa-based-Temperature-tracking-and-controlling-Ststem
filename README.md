# ESP32-and-LoRa-based-Temperature-tracking-and-controlling-Ststem
This a temperature monitoring and controlling system designed with ESP32 as the device and LoRa as a communication infrastructure with an python application to take decisions.

It conists of two key elemets:
- Device (Hardware)
- Application
# Device (Hardware)
- ESP32
- BMP280
- LoRa 

# ESP32
It is used as the main processor for this project which controlls all the device side data of a cyber physical system. It runs a C application for reading data from BMP32 and transmitting over a LoRa communication.
# BMP32
It is used to sense the environmental temperature.
# LoRa
It  is used as a main communication protocol

A Python application running on a PC receive this data from ESP32 over LoRa and takes appropiate actions.
<video controls src="Demo.mp4" title="Demo"></video>