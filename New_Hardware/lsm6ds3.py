from LSM6DS3 import LSM6DS3
import time

if __name__ == "__main__":
    print("Starting up code...")

    lsm6ds3 = LSM6DS3()

    try:
        while True:
            # 读取加速度计的三个轴值
            acc_x = lsm6ds3.read_acceleration_x()
            acc_y = lsm6ds3.read_acceleration_y()
            acc_z = lsm6ds3.read_acceleration_z()
            gyro_x = lsm6ds3.read_gyroscope_x()
            gyro_y = lsm6ds3.read_gyroscope_y()
            gyro_z = lsm6ds3.read_gyroscope_z()
            # 打印加速度计的值
            print(f"AX: {acc_x}, AY: {acc_y}, AZ: {acc_z}")
            # 打印陀螺仪的值
            print(f"GX: {gyro_x}, GY: {gyro_y}, GZ: {gyro_z}")
            # 暂停一秒
            roll, pitch = lsm6ds3.get_angle()
            print(f"Roll angle: {roll:.2f} degrees, Pitch angle: {pitch:.2f} degrees")
            time.sleep(1)

    except IOError as e:
        print("Unable to read from accelerometer, check the setup and try again. Error is: ")
        print(e)