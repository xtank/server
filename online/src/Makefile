CC = g++

WORKDIR = ../
BINDIR = $(WORKDIR)/bin
OBJDIR = $(WORKDIR)/obj

PROTODIR = $(WORKDIR)/proto
LIB_CLIENT_DIR = $(PROTODIR)/client
LIB_CLIENT_PROTO = $(LIB_CLIENT_DIR)/libclientproto.a

PROTOBUF_CFLAGS = $(shell pkg-config --cflags protobuf)
PROTOBUF_LFLAGS = $(shell pkg-config --libs protobuf)

CFLAGS  = -Wall -pipe -DUSE_TLOG -DDEBUG -g -D_GNU_SOURCE -shared -fPIC -D_REENTRANT \
		  -I$(WORKDIR)
LFLAGS 	= -L$(LIB_CLIENT_DIR) -lcrypto -lpthread -pthread -lm -ltaomee -ldl -lrt -lxml2 \
		  -lclientproto
INCLUDE =   -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -I../../ \
			-I/usr/include/libxml2

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
TARGETS = $(BINDIR)/libonline.so

all: $(TARGETS)
	
$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBDIR) $(LFLAGS) $(PROTOBUF_LFLAGS)
#	rm ../bin/$(TARGETS); mv $(TARGETS) ../bin/

$(OBJS): %.o: %.cpp
	$(CC) -c $(CFLAGS)  $(INCLUDE) $< -o $@

clean:
	rm -f *.o
	rm -f $(TARGETS)

