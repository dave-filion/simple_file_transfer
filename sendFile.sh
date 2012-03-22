#!/bin/bash

# Make big file
bin/makeBigFile

# Move file into test folder
mv before.txt test

# Send to server
bin/sender test/before.txt 127.0.0.1

