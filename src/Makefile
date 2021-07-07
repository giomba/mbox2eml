ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

mbox2eml: mbox2eml.cpp
	g++ $(CCFLAGS) -o mbox2eml -std=c++11 -Wall mbox2eml.cpp

install: mbox2eml
	install -d $(DESTDIR)$(PREFIX)/bin
	install -t $(DESTDIR)$(PREFIX)/bin mbox2eml

clean:
	rm mbox2eml
