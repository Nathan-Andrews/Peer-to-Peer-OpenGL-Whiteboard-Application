# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Platform-specific configurations
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    # Linux-specific flags
    LIBS = -lGL -lGLU -lGLEW -lglfw -lpthread
    BOOST_LIBS = -lboost_system
    INCLUDES = -I/usr/include -I/usr/local/include
    LIBDIRS = -L/usr/lib -L/usr/local/lib
else ifeq ($(UNAME_S), Darwin)
    # macOS-specific flags
    LIBS = -lglfw -lGLEW -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    BOOST_LIBS = -lboost_system
    INCLUDES = -I/opt/homebrew/include -I/usr/local/include
    LIBDIRS = -L/opt/homebrew/lib -L/usr/local/lib
else
    # Windows-specific flags (MinGW)
    LIBS = -lglfw3 -lglew32 -lopengl32 -lgdi32 -luser32 -lkernel32
    BOOST_LIBS = -lboost_system-mt
    INCLUDES = -IC:/boost/include -IC:/glfw/include -IC:/glm/include -IC:/glew/include
    LIBDIRS = -LC:/boost/lib -LC:/glfw/lib -LC:/glew/lib
endif

# Directories
SRCDIR = Peer-to-Peer-OpenGL-Whiteboard-Application
BUILDDIR = build
TARGET = app

# Source files and dependencies
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# Build rules
.PHONY: all clean distclean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBDIRS) -o $@ $^ $(LIBS) $(BOOST_LIBS)

# Generate dependencies while compiling
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean built files
clean:
	$(RM) $(BUILDDIR)/*.o $(BUILDDIR)/*.d $(TARGET)

# Clean everything including build directory
distclean: clean
	$(RM) -r $(BUILDDIR)

# Print variables for debugging
debug:
	@echo "Sources: $(SRCS)"
	@echo "Objects: $(OBJS)"
	@echo "Dependencies: $(DEPS)"