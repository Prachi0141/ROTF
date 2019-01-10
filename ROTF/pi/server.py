import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 3125
s.bind(('10.42.0.1', port))
s.listen(3)
print('socket is listening')

while True:
	c, addr = s.accept()
	print('Got connection from', addr)
	print(c.recv(1024))
	c.close()