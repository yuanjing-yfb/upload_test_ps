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
        '''
        RockerX_Value = ReadChannel(RockerX_Channel)
        time.sleep(0.1)
        RockerX_Volts = ConvertVolts(RockerX_Value,2)
        RockerY_Value = ReadChannel(RockerY_Channel)
        time.sleep(0.1)
        RockerY_Volts = ConvertVolts(RockerY_Value,2)
        print("--------------------------------------------")
        print("RockerX: {} ({}V)".format(RockerX_Value,RockerX_Volts))
        print("RockerY: {} ({}V)".format(RockerY_Value,RockerY_Volts))
        time.sleep(0.1)
    '''
        x_value = ReadChannel(RockerX_Channel)
        time.sleep(0.1)
        y_value = ReadChannel(RockerY_Channel)
        time.sleep(0.1)
        if x_value > 180:
            print("左")
        if x_value < 80:
            print("右")
        if y_value > 180:
            print("上")
        if y_value < 80:
            print("下")
        time.sleep(0.15)
            
except KeyboardInterrupt:
    print("The program was manually terminated") 

finally:
    spi.close()  