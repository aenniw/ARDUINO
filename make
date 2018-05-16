#!/bin/bash

for PROJECT in ./*/platformio.ini; do
    cd ${PROJECT%*/*} && \
        platformio run && \
        cd ../
done