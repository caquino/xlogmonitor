MAKEDEPEND = gcc -MM
CC = gcc

CFLAGS = -Wall -g `gtk-config --cflags` -D__XLPM__ -mpentium -O6
LFLAGS = `gtk-config --libs`

OBJECTS = pixmap.o about.o readrc.o main.o
SOURCES = pixmap.c about.c readrc.c main.c
HEADERS = pixmap.h about.h readrc.h main.h

all:		hackerzone-fi

hackerzone-fi:	$(OBJECTS)
		$(CC) -o xlogmonitor $(OBJECTS) $(LFLAGS)

depend:		$(SOURCES) $(HEADERS)
		$(MAKEDEPEND) $(CFLAGS) $(SOURCES) > rules.dep

clean:		
		rm -f *~ *.o *.bak xlogmonitor
		rm -f rules.dep
		touch rules.dep

include rules.dep
