from socket import *
import sys
import struct

sys.path.append("/home/kentguan/xtank/online/python/proto")

import data_pb2
import login_pb2

HOST='192.168.174.128'
PORT=9009

ADDR=(HOST, PORT);
tcpCli=socket(AF_INET, SOCK_STREAM);
tcpCli.connect(ADDR);

enter = login_pb2.cs_enter_server();
enter.userid = 545155399;

body_str = enter.SerializeToString();

print 'bodystr',len(body_str);

length = 20 + len(body_str);
seq = 1;
cmd = 101;
ret = 545155399;
checksum = 0;

header_str = struct.pack('iiiii',length,seq,cmd,ret,checksum);

print 'headstr',len(header_str),header_str;

tcpCli.send(header_str+body_str);

raw_input("input stop!");

tcpCli.close()
