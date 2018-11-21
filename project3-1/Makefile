CC = g++
CXXFLAGS = -g -Wall -std=c++11

SRCS := project3-1.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET = project3-1
$(TARGET): $(OBJS)
			 $(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS)

clean:
		rm $(TARGET) $(OBJS)
