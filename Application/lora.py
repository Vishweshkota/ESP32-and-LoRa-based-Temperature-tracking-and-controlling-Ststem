import serial
import time
import sys

serialPort = serial.Serial(port="COM9", baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
serialString = ""  # Used to hold data coming over UART

"""
This function reads data comming on UART port
@params: NONE
@return: list
"""
def readData() -> list:
    serialString = serialPort.readline()
    lst = serialString.decode().strip().split(",")
    return lst

def sendData(command):
    buf = ""
    if(serialPort.writable()):
        buf = (f"AT+SEND=10,{len(command)},{command}\r\n")
        print(buf)
        serialPort.write(buf.encode())
        time.sleep(1)
"""
This function will set up the LORA configurations and sets it ready to listen incoming data
@ params: NONE
@return: NONE
"""
def setUpLoRA():
    if(serialPort):
        print("Connecting to com port......", serialPort.port)
        len = serialPort.write(("AT+RESET\r\n").encode())
        time.sleep(1)
        print(readData())
        if(len > 0):
            len = serialPort.write(("AT+MODE=0\r\n").encode())
            time.sleep(1)
            print(readData())
            if(len > 0):
                len = serialPort.write("AT+ADDRESS=11\r\n".encode())
                time.sleep(1)
                print(readData())
                if(len > 0):
                    len = serialPort.write("AT+NETWORKID=6\r\n".encode())
                    time.sleep(1)
                    print(readData())
                    if(len > 0):
                        len = serialPort.write("AT+ADDRESS=11\r\n".encode())
                        time.sleep(1)
                        print(readData())
                        if (len > 0):
                            len = serialPort.write("AT+BAND=915000000\r\n".encode())
                            time.sleep(1)
                            print(readData())


"""
This is the main calling method
"""
if __name__ == "__main__":
    
    prevTemp = 0.0
    """Calling Setup function to configure LoRa module"""
    setUpLoRA()
    while(1):
        # sendData(1)
        # time.sleep(1)
        # sendData(0)
        # time.sleep(1)
        currentTemp = readData()
        if(len(currentTemp) > 1):
            print(currentTemp[2])
            if(float(currentTemp[2]) > prevTemp):
                sendData("ON")
            else:
                sendData("OFF")
            prevTemp = float(currentTemp[2])
        # else:
            # print(currentTemp)