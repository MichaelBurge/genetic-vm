primary_files = ast.o vm.o
CPP_OPTIONS = -Wall -std=c++11 -g -pg
test_libs = -lboost_unit_test_framework

%.o: %.cpp
	g++ $(CPP_OPTIONS) -o $@ -c $<

main.exe: main.cpp $(primary_files)
	g++ $(CPP_OPTIONS) $^ -o $@
test_suite.exe: tests/main.cpp $(primary_files)
	g++ $(CPP_OPTIONS) $^ -o $@ $(test_libs)
test: test_suite.exe
	./test_suite.exe

