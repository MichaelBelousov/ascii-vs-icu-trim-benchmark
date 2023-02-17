
all: test.cpp criterion.hpp
	clang++ -std=c++17 ./test.cpp -O3 -g -o test `pkg-config --libs --cflags icu-uc icu-io` -fbracket-depth=258

