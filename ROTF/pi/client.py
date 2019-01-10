import socket
import time

while True:
	s = socket.socket()
	s.connect(('10.42.0.92',3125))
	s.sendall('Here I am!')
	s.close()
	time.sleep(1)
