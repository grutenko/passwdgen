CC = cc
CFLAGS = -O3 -std=c99
TARGET = passwdgen

PREFIX=/usr/local/bin

.PHONY: all release

all:
	$(CC) $(CFLAGS) -o $(TARGET) passwdgen.c

.PHONY:install

install:
	cp -f $(TARGET) $(PREFIX)/$(TARGET)
.PHONY: clean

clean:
	cd ./pcg && $(MAKE) clean
	rm -rf *.o $(TARGET)

