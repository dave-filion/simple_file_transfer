#!/bin/bash

# Make big file
./misc/bin/makeBigFile

# Move file into test folder
mv before.txt test

# Send to server
./client/bin/sender test/before.txt 127.0.0.1

# Run comparison
./misc/bin/compare test/before.txt test/after.txt
