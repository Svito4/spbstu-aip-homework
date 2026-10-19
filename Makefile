CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror
BUILD_DIR = build
TARGET = $(BUILD_DIR)/matrix
SRC = main.cpp

.PHONY: all build run clean format format-check tidy test

all: build

build: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

format:
	clang-format -i $(SRC)

format-check:
	clang-format --dry-run --Werror $(SRC)

tidy:
	clang-tidy $(SRC) -- -std=c++14

test: build
	@echo "Running tests..."
	@printf "3 2\n1 2\n3 4\n5 6\n2 3\n7 8 9\n0 1 2\n" | ./$(TARGET) > $(BUILD_DIR)/test_out.txt
	@printf "1 2 7 8 9\n3 4 0 1 2\n5 6\n" | diff -u - $(BUILD_DIR)/test_out.txt
	@echo "All tests passed successfully!"
