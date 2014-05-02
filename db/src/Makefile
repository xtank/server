CC = g++
WORKDIR = ../
SRCDIR = $(WORKDIR)/src
LIBDIR = $(WORKDIR)/lib
OBJDIR = $(WORKDIR)/obj
DEPDIR = $(WORKDIR)/obj
INCLUDEDIR = $(WORKDIR)/src
BINDIR = $(WORKDIR)/bin
TARGET = $(BINDIR)/libdb.so
PROTODIR = $(WORKDIR)/proto

LIB_DB_DIR = $(PROTODIR)
LIB_DB_PROTO = $(LIB_DB_DIR)/libdbproto.a
LIB_DB_MAKEFILE_DIR = $(LIB_DB_DIR)/define

CFLAGS = -Wall -DUSE_TLOG -Werror -fPIC -ggdb \
	-Iinclude -I/usr/lib/glib-2.0/include/ -I/usr/include/glib-2.0 \
	-I$(WORKDIR)/../ `pkg-config --cflags protobuf`-I/usr/include/mysql \
	-I/usr/include/dbser \
	-I$(PROTODIR)
LFLAGS = -Wall -Werror -ggdb \
	-L$(LIBDIR) -L$(PROTODIR) \
	-lcrypto `pkg-config --libs protobuf ` \
	-ldbproto -ltaomee++ -lmysqlclient -ldbser -ltaomee
SRCS = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDEDIR)/*.h)
OBJS := $(patsubst %.cpp,%.o,$(subst $(SRCDIR),$(OBJDIR),$(SRCS)))
DEPS := $(patsubst %.cpp,%.d,$(subst $(SRCDIR),$(DEPDIR),$(SRCS)))

all: db_proto $(TARGET)

$(TARGET): $(LIB_DB_PROTO) $(OBJS) 
	$(CC) -shared -o $(TARGET) $(OBJS) $(LFLAGS)

db_proto: 
	make -C $(LIB_DB_MAKEFILE_DIR)

#由于obj生成在$(OBJDIR)中，在依赖关系中加入目录
$(DEPDIR)/%.d: $(SRCDIR)/%.cpp  
	@echo "generate $@"
	@printf "$(OBJDIR)/" > $@.tmp
	@$(CC) -MM $< $(CFLAGS) >> $@.tmp
	@mv $@.tmp $@

sinclude $(DEPS)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	-rm $(OBJS) $(TARGET) $(DEPS)

clean-all:
	make -C $(LIB_DB_MAKEFILE_DIR) clean
