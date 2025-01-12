import serial
import sys
import time

s = serial.Serial(sys.argv[1], int(sys.argv[2]))

with open(sys.argv[3], "rb") as f: cont = f.read()

time.sleep(1)
s.read(s.inWaiting())

page_size = 256

read_back = b""

try:
    for i in range(0, len(cont), page_size):
        s.write(cont[i:i+page_size])
        rb = s.read(page_size)
        read_back += rb
        if rb != cont[i:i+page_size]:
            print("\nError writing page 0x{}-0x{}".format(hex(i)[2:].rjust(8, "0"), hex(i+page_size-1)[2:].rjust(8, "0")))
        print("\r0x{}/0x{} bytes written".format(hex(i+page_size)[2:].rjust(8, "0"), hex(len(cont))[2:].rjust(8, "0")), end="")
finally:
    with open("/tmp/readback.bin", "wb") as f: f.write(read_back)
    s.close()
