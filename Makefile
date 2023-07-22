CC = cc
CFLAGS = -Ipcg/include
TARGET = passwdgen

PREFIX=/usr/local/bin

.PHONY: all

all: libpcg_random.a version.h
	$(CC) $(CFLAGS) -Lpcg/src -lpcg_random -o $(TARGET) passwdgen.c

.PHONY:install

install:
	cp $(TARGET) $(PREFIX)/$(TARGET)

version.h:
	sed -e 's/@@VERSION_MAJOR@@/1/;s/@@VERSION_MINOR@@/1/;s/@@VERSION_PATCH@@/1/' version.h.in > version.h

libpcg_random.a:
	cd ./pcg/src && $(MAKE)

.PHONY: clean

clean:
	cd ./pcg && $(MAKE) clean
	rm -rf *.o $(TARGET) version.h

