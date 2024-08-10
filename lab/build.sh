#!/bin/bash

# Assembly name
ASSEMBLY="lab"

# Set the source directory
SRC_DIR="src"

# Set the output directory
BIN_DIR="bin"

# Create the output directory if it doesn't exist
mkdir -p $BIN_DIR

# Find all .c files in the source directory
SRC_FILES=$(find $SRC_DIR -name "*.c")

# Set the output or binary name
OUTPUT="$BIN_DIR/$ASSEMBLY"

# Set the compiler
CC="clang-18"

# Set any compiler flags
CFLAGS="-g"

# Set linker flags
LDFLAGS="-lxcb -lvulkan"

# Build the project
$CC $CFLAGS $SRC_FILES $LDFLAGS -o $OUTPUT

# Check if the build was successful
if [ $? -eq 0 ]; then
  echo "Build successful: $OUTPUT"
else
  echo "Build failed"
  exit 1
fi
