# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Platform-specific configurations
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    # Linux-specific flags
    LIBS = -lGL -lGLU -lglfw -lpthread
    BOOST_LIBS = -lboost_system
    INCLUDES = -I/usr/include -I/usr/local/include
    LIBDIRS = -L/usr/lib -L/usr/local/lib
else ifeq ($(UNAME_S), Darwin)
    # macOS-specific flags
    LIBS = -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    BOOST_LIBS = -lboost_system
    INCLUDES = -I/opt/homebrew/include -I/usr/local/include
    LIBDIRS = -L/opt/homebrew/lib -L/usr/local/lib
else
    # Windows-specific flags (MinGW)
    LIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32
    BOOST_LIBS = -lboost_system-mt
    INCLUDES = -IC:/boost/include -IC:/glfw/include
    LIBDIRS = -LC:/boost/lib -LC:/glfw/lib
endif

SRCDIR = Peer-to-Peer-OpenGL-Whiteboard-Application
BUILDDIR = build
TARGET = app

# Source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

# Build rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBDIRS) -o $@ $^ $(LIBS) $(BOOST_LIBS)

# Compile .cpp files into .o files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)