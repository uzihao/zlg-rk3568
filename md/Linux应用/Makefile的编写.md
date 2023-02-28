```bash
#
# Makefile for Linux application test example.
#
#---------------------------------------------------------------
# Linux application Makefile sample
# make          -- the optical output without debug info
# make clean    -- clean the object file(s)
#---------------------------------------------------------------
#	

#---------------------------------	/* execute file(s) */
TARGET	= demo
#---------------------------------	/* object file(s) */
OBJECTS = main.o search_droneid.cpp 
#---------------------------------	/* header file(s) */
FFTW3_INC ?= /home/uzihao/3rdparty/FFTW3/x86-install/include
#---------------------------------	/* library file(s) */
FFTW3_LIB ?= /home/uzihao/3rdparty/FFTW3/x86-install/lib

#CROSS = aarch64-linux-gnu-
CC = $(CROSS)g++
AS = $(CROSS)as
LD = $(CROSS)ld

CFLAGS ?= -I${shell pwd} -I$(FFTW3_INC) 
LDFLAGS ?= -lfftw3 -L$(FFTW3_LIB)

all:   $(TARGET)
	
$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	
# $(OBJECTS): %.o:%.c
$(OBJECTS): %.o:%.cpp $(TESTFILE_H)
	$(CC) -c $< $(CFLAGS)
	
clean:
	$(RM) $(TARGET) *.o

```

