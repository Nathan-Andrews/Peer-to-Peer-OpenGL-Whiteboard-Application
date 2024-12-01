# Minimum CMake version and project name
cmake_minimum_required(VERSION 3.14)
project(PeerToPeerOpenGLWhiteboard)

# Set C++ standard and compile flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Output the final executable in the project root directory as './app'
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Detect platform and configure platform-specific settings
if(UNIX AND NOT APPLE)
    set(LIBS GL GLU pthread)
    set(BOOST_LIBS boost_system)
    include_directories(/usr/include /usr/local/include)
    link_directories(/usr/lib /usr/local/lib)
elseif(APPLE)
    set(LIBS)
    set(BOOST_LIBS boost_system)
    include_directories(/opt/homebrew/include /usr/local/include)
    link_directories(/opt/homebrew/lib /usr/local/lib)
    set(CMAKE_EXE_LINKER_FLAGS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
elseif(WIN32)
    set(LIBS opengl32 gdi32 user32 kernel32)
    set(BOOST_LIBS boost_system-mt)
    include_directories(C:/boost/include C:/glfw/include)
    link_directories(C:/boost/lib C:/glfw/lib)
endif()

# Set source directories
set(SRCDIR Peer-to-Peer-OpenGL-Whiteboard-Application)
set(UIDIR ${SRCDIR}/UI)

# Add non-UI source files
file(GLOB NON_UI_SRCS "${SRCDIR}/*.cpp")
list(REMOVE_ITEM NON_UI_SRCS "${UIDIR}/clientinterface.cpp") # Exclude clientinterface.cpp from non-UI sources

# Find Qt6
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Find GLM
find_package(GLM REQUIRED)

# Add UI files
set(UI_FILES ${UIDIR}/clientinterface.ui)
set(MOC_FILES ${UIDIR}/clientinterface.h)
set(UI_SRCS ${UIDIR}/clientinterface.cpp)
set(UI_HEADERS ${UIDIR}/ui_clientinterface.h)

# Generate Qt-related files
qt6_wrap_ui(UI_HEADERS_GENERATED ${UI_FILES})
qt6_wrap_cpp(MOC_SOURCES ${MOC_FILES})

# Add executable target with all sources
add_executable(app
    ${NON_UI_SRCS}
    ${UI_SRCS}
    ${UI_HEADERS_GENERATED}
    ${MOC_SOURCES}
)

# Include directories for non-UI and UI code
target_include_directories(app PRIVATE 
    ${SRCDIR} 
    ${UIDIR}
    ${GLEW_INCLUDE_DIRS}  # Include GLEW
    ${GLFW_INCLUDE_DIRS}  # Include GLFW
    ${GLM_INCLUDE_DIRS}   # Include GLM
)

# Link libraries
target_link_libraries(app
    ${LIBS}
    ${BOOST_LIBS}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    GLEW::GLEW          # Link GLEW
    glfw3              # Link GLFW
    glm::glm           # Link GLM (usually header-only, no linking needed but added here for consistency)
)
