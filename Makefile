CC = cc
CFLAGS = -Ipcg/include
TARGET = passwdgen

.PHONY: all

all: libpcg_random.a
	$(CC) $(CFLAGS) -Lpcg/src -lpcg_random -o $(TARGET) passwdgen.c

libpcg_random.a:
	cd ./pcg/src && $(MAKE)

.PHONY: clean

clean:
	cd ./pcg && $(MAKE) clean
	rm -rf *.o $(TARGET)

