all:
	g++ -std=c++11 svr.cpp -o svr -lboost_system -lpthread -I/usr/local/include -L/usr/local/lib
	g++ -std=c++11 cli.cpp -o cli -lboost_system -lpthread -I/usr/local/include -L/usr/local/lib
	g++ -std=c++11 dbg.cpp -o dbg -lboost_system -lpthread -lboost_program_options -I/usr/local/include -L/usr/local/lib
