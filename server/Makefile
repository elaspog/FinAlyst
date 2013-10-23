all: server clienttest

%.o: %.cpp *.hpp
	$(CXX) -Wall -Wextra -std=gnu++11 -O0 -c -g $< -o $@

server: server.o rand.o logger.o session.o request.o template_common.o utils.o
	$(CXX) -lmysqlclient -lfcgi++ -lgcrypt $+ -o $@

clienttest: clienttest.cs
	dmcs -debug -r:System.Runtime.Serialization $<

clean:
	rm -f server *.o clienttest.exe *.mdb
