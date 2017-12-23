PREFIX?=/usr
CC=gcc
CFLAGS=-Wall -std=c99 -fPIC -O2 -nostartfiles -shared
LIBS=

MACHINE:=$(shell uname -m)

ifeq ($(MACHINE), x86_64)
	LIBDIR?=$(PREFIX)/lib64
else
	LIBDIR?=$(PREFIX)/lib
endif

VERSION=0.2.0

all:
	$(CC) $(CFLAGS) $(INC) ./text-tools.c -o ./text-tools.so $(LDFLAGS) $(LIBS)

clean:
	rm -f ./text-tools.so

install:
	test -d "$(DESTDIR)$(LIBDIR)/efind/extensions" || mkdir -p "$(DESTDIR)$(LIBDIR)/efind/extensions"
	cp ./text-tools.so "$(DESTDIR)$(LIBDIR)/efind/extensions"
	chmod 755 "$(DESTDIR)$(LIBDIR)/efind/extensions/text-tools.so"

uninstall:
	rm -f "$(DESTDIR)$(LIBDIR)/efind/extensions/text-tools.so"

tarball:
	cd .. && \
	rm -rf ./efind-text-tools-$(VERSION) && \
	cp -r ./efind-text-tools ./efind-text-tools-$(VERSION) && \
	find ./efind-text-tools-$(VERSION) -name ".git*" | xargs rm -r && \
	tar cfJ ./efind-text-tools-$(VERSION).tar.xz ./efind-text-tools-$(VERSION) --remove-files 
