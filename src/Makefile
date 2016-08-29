#


CFLAGS_DEBUG=-g -O0 -D DEBUG # Include debug symbols, disable optimizations, etc

DEBUG=-DDEBUG
DEBUG=

CFLAGS=-Wall -g -DVERSION_TXT -DVersion_info="$(versionInfo)"

LFLAGS = -g 
CCINC=
LIBS=


# Binary file name
TARGET=test-tap

OBJS=test-tap.o
OBJS1=
OBJS2=

# Compile rule
.c.o:
	$(CC) $(CFLAGS) $(CCINC) -c $< 

# Target
all: $(TARGET)

$(TARGET): $(OBJS) $(OBJS1)
	$(CC) $(LFLAGS)  $(OBJS) $(OBJS2) -o $(TARGET) $(LIBS)
#	cp $(TARGET) $(TARGET).unstripped
#	$(STRIP) $(TARGET) 


clean:
	-rm $(TARGET) $(TARGET).unstripped $(OBJS) $(OBJS2)


