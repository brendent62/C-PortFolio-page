#!/bin/bash

set -e

set -x

echo "starting test"

gcc -o myprogram *server.c -Wall -Wextra

if ./myprogram --test; then
    echo "all test passed"
else
    echo "test failed"
    exit 1
fi