#!/bin/bash

EXEC=./codexion
TIMEOUT=5

tests=(
"1 800 200 200 200 3 0 fifo"
"2 800 200 200 200 3 0 fifo"
"3 800 200 200 200 3 0 fifo"
"4 800 200 200 200 3 0 fifo"
"5 800 200 200 200 3 0 fifo"

"2 100 60 60 60 5 0 fifo"
"3 100 60 60 60 5 0 fifo"
"4 100 60 60 60 5 0 fifo"
"5 100 60 60 60 5 0 fifo"

"2 500 100 100 100 10 0 edf"
"4 500 100 100 100 10 0 edf"
"5 500 100 100 100 10 0 edf"

"2 60 30 30 30 2 0 fifo"
"3 60 30 30 30 2 0 fifo"
"4 60 30 30 30 2 0 fifo"

"10 1000 200 200 200 5 0 fifo"
"20 1000 200 200 200 3 0 fifo"
"50 1000 200 200 200 2 0 fifo"

"50 1000 50 50 50 5 0 edf"
"100 1000 50 50 50 2 0 fifo"
)

GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[1;33m"
RESET="\033[0m"

passed=0
failed=0

for args in "${tests[@]}"
do
    echo "================================================="
    echo "$EXEC $args"

    timeout ${TIMEOUT}s $EXEC $args > /dev/null 2>&1
    status=$?

    if [ $status -eq 0 ]; then
        echo -e "${GREEN}PASS${RESET}"
        ((passed++))
    elif [ $status -eq 124 ]; then
        echo -e "${YELLOW}TIMEOUT (possible deadlock)${RESET}"
        ((failed++))
    else
        echo -e "${RED}FAILED (exit code $status)${RESET}"
        ((failed++))
    fi
done

echo
echo "===================="
echo "Passed : $passed"
echo "Failed : $failed"