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
	@printf "2 3\n1 2 3\n4 5 6\n" | ./$(TARGET) > $(BUILD_DIR)/test_out.txt
	@printf "1 4\n2 5\n3 6\n" | diff -u - $(BUILD_DIR)/test_out.txt
	@echo "All tests passed successfully!"
