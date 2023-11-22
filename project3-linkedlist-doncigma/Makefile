CXX = g++
CXXFLAGS = -Wall -std=c++11 -Iinclude -g
DIFFFLAGS = --strip-trailing-cr -s

OBJDIR = obj

SRCS = main.cpp helpers.cpp linkedlisttest.cpp
OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

TARGET = repl
LLTEST = lltest.txt

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include generated dependency files
-include $(DEPS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

lltest: $(TARGET)
	./$(TARGET) -e "t $(LLTEST)" 2> /dev/null

test: lltest

lltestdebug: $(TARGET)
	./$(TARGET) -e "t $(LLTEST)"

testdebug: lltestdebug

.PHONY: all clean lltest test lltestdebug testdebug
