# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -I/usr/local/include   
LDFLAGS = -L/usr/local/lib -lgtest -lgtest_main -pthread   

# CXXFLAGS = -std=c++20 -Iinclude -I/usr/local/include   
# LDFLAGS = -L/usr/local/lib -lgtest -lgtest_main -pthread   

# CXXFLAGS += -fsanitize=address -g
# LDFLAGS += -fsanitize=address


# Output binary for main program
TARGET = main

# Source files for the main program
SRCS = easy_df_adam.cpp

# Object files (generated from source files)
OBJS = $(SRCS:.cpp=.o)

# Build the main target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Test files
TESTS = $(wildcard tests/test*.cpp)
TEST_TARGET = test_output

# Compile and run tests with Google Test
test_only: $(TEST_TARGET)
	./$(TEST_TARGET) #--gtest_fail_fast

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

test: clean test_only
pytest: clean test_only


.PHONY: clean run test
