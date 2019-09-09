


a.exe:main.o FileOperator.o
	g++ -o a.exe main.o FileOperator.o

main.o:main.cpp file_operations.h
	g++ -c main.cpp -o main.o

FileOperator.o:file_operations.cpp file_operations.h
	g++ -g -fPIC -Wno-deprecated -c file_operations.cpp -o FileOperator.o
