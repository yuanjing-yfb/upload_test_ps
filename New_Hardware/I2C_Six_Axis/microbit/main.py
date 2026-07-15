# Imports go at the top
from microbit import *
from lsm6ds3 import LSM6DS3

display.off()
sensor = LSM6DS3()

while True:
    ax, ay, az, gx, gy, gz = sensor.get_readings()
    print("Accelerometer: ({}, {}, {})\n".format(ax, ay, az))
    print("Gyroscope: ({}, {}, {})\n".format(gx, gy, gz))
    sleep(1000)
