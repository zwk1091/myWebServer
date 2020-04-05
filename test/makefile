helloworld:main.o hello.o
	g++ -g main.o hello.o -o  helloworld

hello.o:hello.cpp hello.h
	g++ -g -c hello.cpp -o hello.o

main.o:main.cpp 
	g++ -g -c main.cpp -o main.o

clean:
	rm -rf *.o helloworld
