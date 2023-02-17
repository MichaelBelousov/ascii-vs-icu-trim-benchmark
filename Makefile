
all: test.cpp criterion.hpp
	clang++ -std=c++17 ./test.cpp -O3 -g -o test -licuuc -fbracket-depth=258

