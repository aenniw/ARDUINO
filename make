#!/bin/bash

EXIT_CODE=0
LOG_FILE=/tmp/platfomio.log

for PROJECT in ./*/platformio.ini; do
    cd ${PROJECT%*/*}
    echo -e "\n\t\e[32m\e[1m${PROJECT%*/*}\e[0m" | tr ./ ' '

    platformio run 2>&1 > ${LOG_FILE}
    EXIT_CODE=$?

    cd ../
    if [[ ! $EXIT_CODE -eq 0 ]]; then
        cat ${LOG_FILE}
        break
    else
        if grep -q SUMMARY ${LOG_FILE}; then
            cat ${LOG_FILE} | sed -n -e '/SUMMARY/,$p'
        else
            cat ${LOG_FILE} | sed -n -e '/Memory Usage/,$p'
        fi
    fi
done

exit ${EXIT_CODE}