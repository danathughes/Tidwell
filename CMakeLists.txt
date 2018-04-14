# The name of the project
PROJECT (RunChip8)

# Minimum version of cmake required
CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

# Detect GTK+ headers / library files
FIND_PACKAGE (PkgConfig REQUIRED)
PKG_CHECK_MODULES (GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell where to look for libraries and headers
INCLUDE_DIRECTORIES (${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES (${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK_CFLAGS_OTHER})

# Use C++11 
set (CMAKE_CXX_STANDARD 11)

# Version Number
SET (Chip8_VERSION_MAJOR 1)
SET (Chip8_VERSION_MINOR 0)


# Bring the headers and source files into the project
INCLUDE_DIRECTORIES (include)
FILE (GLOB SOURCES "src/*.cpp")

# Add the executable
ADD_EXECUTABLE (RunChip8 ${SOURCES})

# Link the output files, and GTK+ libraries
TARGET_LINK_LIBRARIES (RunChip8 ${EXTRA_LIBS})
TARGET_LINK_LIBRARIES (RunChip8 ${GTK3_LIBRARIES})

# add the install targets
install (TARGETS RunChip8 DESTINATION bin)
