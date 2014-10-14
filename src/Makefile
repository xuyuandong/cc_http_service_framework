APP = $(PWD)/..

BIN = $(APP)/bin

AR = $(APP)/lib/*.a

LIB_PATH = -L /usr/local/lib/

LIBS = -lpthread -lrt -ldl -lcurl

INCLUDE = -I $(APP)/common/ \
		  -I $(APP)/include/ \
		  -I /usr/local/include/

LDFLAGS = -shared -g -Wall -fPIC

CXXFLAGS = -std=c++11 -fPIC -Wall -O3 -fopenmp
#CXXFLAGS = -std=c++11 -fPIC -g -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-label -Wno-parentheses -gdwarf-2 -gstrict-dwarf

CC = /usr/local/gcc-4.8.1/bin/g++

STGT = $(BIN)/service

SOBJ = service.o \
	   http_proxy.o \
	   request_handler.o \
	   mongoose.o 

CTGT = $(BIN)/client

COBJ = client.o

.PHONY : all clean lint
all: $(STGT) $(CTGT) 

lint:
	python cpplint.py src/*.cc include/*.h
	
.cc.o:
	$(CC) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

$(STGT): $(SOBJ)
	$(CC) $(CXXFLAGS) $(SOBJ) -o $(STGT) $(INCLUDE) $(AR) $(LIB_PATH) $(LIBS)

$(CTGT): $(COBJ)
	$(CC) $(CXXFLAGS) $(COBJ) -o $(CTGT) $(INCLUDE) $(AR) $(LIB_PATH) $(LIBS)

clean:
	rm $(SOBJ) $(STGT) $(COBJ) $(CTGT)
