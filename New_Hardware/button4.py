import spidev
import time

spi = spidev.SpiDev()
spi.open(0, 1)  

spi.max_speed_hz = 1000000 
spi.mode = 0  

RockerX_Channel = 0x02
RockerY_Channel  = 0x01
Key_Channel = 0x03
Voltage_Channel  = 0x04

def ReadChannel(channel):
    response = spi.xfer2([channel])
    return response[0]

def ConvertVolts(data,places):
    volts = (data * 3.3) / float(255)
    volts = round(volts,places)
    return volts

try:
    while True:
        Key_Value = ReadChannel(Key_Channel)
        time.sleep(0.1)
        if 181 <= Key_Value <= 191:
            print("Key 1 is pressed")
        elif 197 <= Key_Value <= 207:
            print("Key 2 is pressed")
        elif 212 <= Key_Value <= 222:
            print("Key 3 is pressed")
        elif 223 <= Key_Value <= 233:
            print("Key 4 is pressed")
        time.sleep(0.1)

except KeyboardInterrupt:
    print("The program was manually terminated") 

finally:
    spi.close()