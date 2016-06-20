all: simpleCostMap

simpleCostMap: simpleCostMap.cpp
	g++ -g $^ -o $@ -Wall -std=c++0x `pkg-config opencv --cflags --libs`
