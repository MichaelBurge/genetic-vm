primary_files = ast.cpp
options = -Wall -std=c++11 -g -pg
main.exe: main.cpp $(primary_files)
	g++ $(options) $^ -o $@
test_suite.exe: tests/main.cpp test.cpp $(primary_files)
	g++ $(options) $^ -o $@
test: test_suite.exe
	./test_suite.exe

