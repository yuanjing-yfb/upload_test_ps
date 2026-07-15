import spidev
import time

spi = spidev.SpiDev()
spi.open(0, 1)  

spi.max_speed_hz = 500000 
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
        RockerX_Value = ReadChannel(RockerX_Channel)
        time.sleep(0.1)
        RockerX_Volts = ConvertVolts(RockerX_Value,2)
        RockerY_Value = ReadChannel(RockerY_Channel)
        time.sleep(0.1)
        RockerY_Volts = ConvertVolts(RockerY_Value,2)
        #Key_Value = ReadChannel(Key_Channel)
        #time.sleep(0.1)
        #Key_Volts = ConvertVolts(Key_Value,2)
        #Voltage_Value = ReadChannel(Voltage_Channel)
        #time.sleep(0.1)
        #Voltage_Volts = ConvertVolts(Voltage_Value,2)
        print("--------------------------------------------")
        print("RockerX: {} ({}V)".format(RockerX_Value,RockerX_Volts))
        print("RockerY: {} ({}V)".format(RockerY_Value,RockerY_Volts))
        #print("Key: {} ({}V)".format(Key_Value,Key_Volts))
        #print("Voltage: {} ({}V)".format(Voltage_Value,Voltage_Volts))
        time.sleep(0.1)

except KeyboardInterrupt:
    print("The program was manually terminated") 

finally:
    spi.close()  