BUILD_DIR = build/linux
SRC_DIR = src
INCLUDE_DIR = include

CXX = clang++
CXXFLAGS = -std=c++11 -I$(INCLUDE_DIR)
LDFLAGS = -lgumbo -lboost_regex -lboost_system -lboost_filesystem -lcurl

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
EXECUTABLE = $(BUILD_DIR)/lol.gg

ifeq ($(CXX), clang++)
	CXXFLAGS += -Weverything -Werror -Wno-c++98-compat -Wno-disabled-macro-expansion
else
	CXXFLAGS += -Wall -Wextra -pedantic -Werror -pedantic-errors
endif

all: release

debug: CXXFLAGS += -DDEBUG -g -DGLIBCXX_FORCE_NEW
debug: $(SOURCES) $(EXECUTABLE)

release: CXXFLAGS += -O3
release: $(SOURCES) $(EXECUTABLE)

lint: CXXFLAGS += -fsyntax-only
lint: $(SOURCES) $(OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) $(BUILD_DIR)/Config

.PHONY: all clean
