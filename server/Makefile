CXXFLAGS=-MD -Wall -Wextra -std=gnu++11 -O0 -mtune=native -march=native
LIBS=-lmysqlclient -lfcgi++ -lgcrypt -lmemcached

apps=server
units=server session session_mem rand logger request template_common utils \
      bulogic

all: $(apps) $(docs) csharpexamples

docs: assets/webserviceapi.html assets/changelog.html \
    assets/categoryapi.html assets/itemapi.html assets/planitemapi.html \
    assets/statisticsapi.html

csharpexamples: clienttest.exe stresstest.exe

$(apps): %: %.o $(units:=.o)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

-include $(units:=.d) $(apps:=.d)

%.exe: %.cs
	dmcs -debug -r:System.Runtime.Serialization $<

%.html: %.markdown
	echo '<html><head><meta charset="UTF-8"><link rel="stylesheet" type="text/css" href="main.css"></head><body>' > $@
	markdown2 -x wiki-tables $< >> $@
	echo '</html>' >> $@

clean:
	rm -f server *.o *.d clienttest.exe stresstest.exe *.mdb

.PHONY: all clean csharpexamples docs
