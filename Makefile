# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -I/usr/local/include  # Adjust path to gtest include
LDFLAGS = -L/usr/local/lib -lgtest -lgtest_main -pthread  # Adjust path to gtest lib

# Output binary for main program
TARGET = main

# Source files for the main program
SRCS = main.cpp

# Object files (generated from source files)
OBJS = $(SRCS:.cpp=.o)

# Build the main target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Test files
TESTS = $(wildcard tests/test*.cpp)
TEST_TARGET = test_output

# Compile and run tests with Google Test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TESTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TESTS) $(LDFLAGS)

# Compile each .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET)

.PHONY: clean run test
