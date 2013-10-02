all: server

server: server.cpp
	$(CXX) -std=gnu++11 -O0 -lmysqlclient -lfcgi++ -g $+ -o $@

clean:
	rm server
