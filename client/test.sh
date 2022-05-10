#!/bin/bash


SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# path to executable file
CLIENT_APP="${SCRIPT_DIR}/build/client"

TEMP_SCRIPT="${SCRIPT_DIR}/temp.sh"

echo "\"${CLIENT_APP}\" " > "${TEMP_SCRIPT}"   

# rules for execute;
chmod 755 "${TEMP_SCRIPT}"

# open terminals
for ((i=0; i < 10; ++i )); do
    open -a Terminal "${TEMP_SCRIPT}"
done