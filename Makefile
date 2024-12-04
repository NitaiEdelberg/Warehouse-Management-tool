all: clean compile link clesnObjAndRun

link:
	g++ -o bin/warehouse bin/Action.o bin/Customer.o bin/Order.o bin/Volunteer.o bin/WareHouse.o bin/Parse.o bin/main.o

compile:
	g++ -g -Wall -Weffc++ -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -c -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -c -o bin/Parse.o src/Parse.cpp
	g++ -g -Wall -Weffc++ -c -o bin/main.o src/main.cpp
clean:
	rm -f bin/*.o
clesnObjAndRun:
	rm -f *.o
	clear
	valgrind --leak-check=full --show-reachable=yes ./bin/warehouse "bin/rest/input_file.txt"