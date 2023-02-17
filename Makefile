
all: test.cpp criterion.hpp
	clang++ -std=c++17 ./test.cpp -g -O3 -o test `pkg-config --libs --cflags icu-uc icu-io` -fbracket-depth=258

