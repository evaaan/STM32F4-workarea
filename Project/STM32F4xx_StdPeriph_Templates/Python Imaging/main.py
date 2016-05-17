import Image
import serial
import sys
'''
0x3E COM14 - Zoom function on/off (Bit[1]) - ON
0x41 COM16 - Scaling down on/off (Bit[0]) - ON
0x72 POIDX - Can skip every 2/4/8 lines and effectively scale down resolution
0x74 XINDX - Horizontal Scaling Down Coefficients (?)

'''
BAUDRATE = 115200
PORT = 'COM3'
FILENAME = 'output.data'
IMG_SIZE = 38400

def read_frame():
	cnt = 0
	img_data = ''
	file = open(FILENAME, 'wb')
	ser = serial.Serial(PORT, BAUDRATE, timeout=0.5)
	ser.write(b'S');
	while (cnt < IMG_SIZE):
		for c in ser.read():
			img_data += c
			cnt+=1
	file.write(img_data)
	file.close()
	size = (160, 120)
	img = Image.fromstring('L', size, img_data, 'raw', 'L;16')
	img.save('usart.png','png')
	
def write_serial():
	ser = serial.Serial(PORT, BAUDRATE)
	ser.write(b'hello\n')
	ser.close()
	
	
def main():
	file = open('rgb565_dump.data', 'rb')
	data = file.read()
	file.close()
	
	size = (160,120)
	
	#mode, size, data, "raw", raw mode, stride, orientation
	img = Image.fromstring('L', size, data, 'raw', 'L;16')
	img.save('output.png','png')
	img.show()

if __name__ == "__main__":
	read_frame()