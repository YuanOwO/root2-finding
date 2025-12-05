CXX = clang++
CXXFLAGS = -std=c++17 -Iinclude -O2 -O3 -Wall -Wextra -Wshadow -fsanitize=address

TARGET = newton_cli

SRCS = $(wildcard src/*.cpp)
SRCS += $(wildcard src/cpplib/*.cpp)
OBJS = $(SRCS:src/%.cpp=build/%.o)

all: $(TARGET)

# 編譯成執行檔
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	@echo "編譯成功: $(TARGET)"

# 編譯 .cpp 成 .o 檔案
build/%.o: src/%.cpp
	@mkdir -p build/$(dir $*)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -rf build $(TARGET)