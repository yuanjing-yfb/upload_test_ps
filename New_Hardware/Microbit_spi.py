from microbit import *

display.off()

# 初始化SPI
spi.init(baudrate=100000, bits=8, mode=0, sclk=pin13, mosi=pin15, miso=pin14)

# 为特定命令定义一个字典
COMMANDS = {
    0x01: "RockerY",
    0x02: "RockerX",
    0x03: "Key",
    0x04: "Voltage"
}

def read_register(command):
    # 检查命令是否在支持的范围内
    if command not in COMMANDS:
        return None
    
    # 创建一个输出缓冲区，并填充特定的命令值
    buffer_out = bytearray([command])
    buffer_in = bytearray(len(buffer_out))  # 输入缓冲区长度匹配
    
    # 执行SPI通信
    pin16.write_digital(0)  # CS引脚拉低 (通信开始)
    spi.write_readinto(buffer_out, buffer_in)
    pin16.write_digital(1)  # CS引脚拉高 (通信结束)
    
    # 返回接收到的数值
    return buffer_in[0]

while True:
    # 举例：尝试读取寄存器 0x01 的值
    resultX = read_register(0x02)
    sleep(100)
    resultY = read_register(0x01)
    sleep(100)
    print("RockerX:", resultX)
    print("RockerY:", resultY)
    sleep(100)