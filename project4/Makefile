CC = g++
CXXFLAGS = -g -Wall -std=c++11

SRCS := project4.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET = project4
$(TARGET): $(OBJS)
			 $(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS) -lpthread

clean:
			rm $(TARGET) $(OBJS)
