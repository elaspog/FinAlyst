all: server assets/webserviceapi.html assets/changelog.html csharpexamples

csharpexamples: clienttest.exe stresstest.exe

%.o: %.cpp *.hpp
	$(CXX) -Wall -Wextra -std=gnu++11 -O0 -mtune=native -march=native -c -g $< -o $@

server: server.o session.o session_mem.o rand.o logger.o request.o template_common.o utils.o
	$(CXX) -lmysqlclient -lfcgi++ -lgcrypt $+ -o $@

%.exe: %.cs
	dmcs -debug -r:System.Runtime.Serialization $<

%.html: %.markdown
	echo '<html><head><meta charset="UTF-8"><link rel="stylesheet" type="text/css" href="main.css"></head><body>' > $@
	markdown2 -x wiki-tables $< >> $@
	echo '</html>' >> $@

clean:
	rm -f server *.o clienttest.exe *.mdb

.PHONY: clean csharpexamples
