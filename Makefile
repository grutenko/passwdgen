CC = cc
CFLAGS = -Ipcg/include
TARGET = passwdgen

PREFIX=/usr/local/bin

TAG_COMMIT := $(shell git rev-list --abbrev-commit --tags --max-count=1)
TAG := $(shell git describe --abbrev=0 --tags ${TAG_COMMIT} 2>/dev/null || true)
VERSION_MAJOR := $(shell echo $(TAG) | sed -rn 's/v([0-9]+).([0-9]+)/\1/p')
VERSION_MINOR := $(shell echo $(TAG) | sed -rn 's/v([0-9]+).([0-9]+)/\2/p')
VERSION_PATCH := 0

.PHONY: all

all: libpcg_random.a version.h
	$(CC) $(CFLAGS) -Lpcg/src -lpcg_random -o $(TARGET) passwdgen.c

.PHONY:install

install:
	cp -f $(TARGET) $(PREFIX)/$(TARGET)

version.h:
	@echo $(TAG)
	sed -e 's/@@VERSION_MAJOR@@/$(VERSION_MAJOR)/;s/@@VERSION_MINOR@@/$(VERSION_MINOR)/;s/@@VERSION_PATCH@@/$(VERSION_PATCH)/' version.h.in > version.h

libpcg_random.a:
	cd ./pcg/src && $(MAKE)

.PHONY: clean

clean:
	cd ./pcg && $(MAKE) clean
	rm -rf *.o $(TARGET) version.h

