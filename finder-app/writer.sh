#!/bin/bash

if [ $# -ne 2 ]; then
  echo "Error: 2 (two) arguments required - <writefile> and <writestr>"
  exit 1
fi

writefile=$1
writestr=$2

# Create the directory path if it doesn't exist
mkdir -p "$(dirname "$writefile")"

# Try to create the file and write the content
if ! echo "$writestr" > "$writefile"; then
  echo "Error: Failed to create or write to file '$writefile'."
  exit 1
fi

echo "Content written to '$writefile' successfully."