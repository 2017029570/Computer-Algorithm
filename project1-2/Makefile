CC = g++
CXXFLAGS = -g -Wall -std=c++11

SRCS := project1-2.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET = project1-2
$(TARGET): $(OBJS)
			 $(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
		rm $(TARGET) $(OBJS)
