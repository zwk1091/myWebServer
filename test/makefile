helloworld:main.o hello.o
	g++ main.o hello.o -o helloworld

hello.o:hello.cpp hello.h
	g++ -c hello.cpp -o hello.o

main.o:main.cpp 
	g++ -c main.cpp -o main.o

clean:
	rm -rf *.o helloworld
