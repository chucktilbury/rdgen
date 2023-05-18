
CC		=	gcc
TARGET	=	rdgen
OBJS	=	memory.o \
			strs.o \
			rdgen.o \
			dump.o

DEFS	=	-DUSE_GC
DEBUG	=	-g
LIBS	=	-lm -lgc
LDIRS	=	-L.
IDIRS	=	-I.
COPTS	=	$(DEBUG) $(DEFS) $(IDIRS) -Wall -Wextra
LOPTS	=	$(DEBUG) $(LDIRS)

%.o:%.c
	$(CC) $(COPTS) -c -o $@ $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LOPTS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	-rm -f $(TARGET) $(OBJS)
