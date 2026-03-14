CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.
TARGET = SmartHome
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
