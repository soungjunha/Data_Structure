CXX := g++
CXXFLAGS := -std=c++11 -g -Wall -Wextra -Wpedantic

SRCS := main.cpp Manager.cpp MusicQueue.cpp PlayList.cpp TitleBST.cpp ArtistBST.cpp
OBJS := $(SRCS:.cpp=.o)
TARGET := run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
