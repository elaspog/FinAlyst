all: server clienttest.exe assets/webserviceapi.html

%.o: %.cpp *.hpp
	$(CXX) -Wall -Wextra -std=gnu++11 -O0 -mtune=native -march=native -c -g $< -o $@

server: server.o rand.o logger.o session.o request.o template_common.o utils.o
	$(CXX) -lmysqlclient -lfcgi++ -lgcrypt $+ -o $@

clienttest.exe: clienttest.cs
	dmcs -debug -r:System.Runtime.Serialization $<

assets/webserviceapi.html: webservice.markdown
	echo '<html><head><meta charset="UTF-8"><link rel="stylesheet" type="text/css" href="main.css"></head><body>' > $@
	markdown2 -x wiki-tables $< >> $@
	echo '</html>' >> $@

clean:
	rm -f server *.o clienttest.exe *.mdb
