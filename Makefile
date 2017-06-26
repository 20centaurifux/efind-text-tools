CC=gcc
CFLAGS=-Wall -std=c99 -fPIC -O0 -g -nostartfiles -shared
LIBS=

VERSION=0.1.0

all:
	$(CC) $(CFLAGS) $(INC) ./text-tools.c -o ./text-tools.so $(LDFLAGS) $(LIBS)

clean:
	rm -f ./text-tools.so

install:
	test -d "$(DESTDIR)/etc/efind/extensions" || mkdir -p "$(DESTDIR)/etc/efind/extensions"
	cp ./text-tools.so "$(DESTDIR)/etc/efind/extensions/"
	chmod 755 "$(DESTDIR)/etc/efind/extensions/text-tools.so"

uninstall:
	rm -f "$(DESTDIR)/etc/efind/extensions/text-tools.so"

tarball:
	cd .. && \
	rm -rf ./efind-text-tools-$(VERSION) && \
	cp -r ./efind-text-tools ./efind-text-tools-$(VERSION) && \
	find ./efind-text-tools-$(VERSION) -name ".git*" | xargs rm -r && \
	tar cfJ ./efind-text-tools-$(VERSION).tar.xz ./efind-text-tools-$(VERSION) --remove-files 
