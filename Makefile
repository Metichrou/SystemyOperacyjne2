CXX = g++
CXXFLAGS = -std=c++20 -pthread -Wall -Wextra -O2

TARGET = dining
SRC = dining_philosophers.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) 5  # Change for different amount of philosophers

.PHONY: all clean run
