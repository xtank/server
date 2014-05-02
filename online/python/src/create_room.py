from socket import *
import sys
import struct

sys.path.append("/home/kentguan/xtank/online/python/proto")

import data_pb2
import login_pb2
import room_pb2

HOST='192.168.174.128'
PORT=9009

ADDR=(HOST, PORT);
tcpCli=socket(AF_INET, SOCK_STREAM);
tcpCli.connect(ADDR);

#login
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

#create room
for i in range(1,31):
    create_room = room_pb2.cs_create_room();
    create_room.map_id = 1;
    create_room.name = "girl";

    body_str = create_room.SerializeToString();
    length = 20 + len(body_str);
    seq = 1;
    cmd = 157;
    ret = 545155399;
    checksum = 0;

    header_str = struct.pack('iiiii',length,seq,cmd,ret,checksum);
    tcpCli.send(header_str+body_str);
    
    print "create room",i

raw_input('enter stop');

tcpCli.close()
