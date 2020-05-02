#!/bin/bash

for PROJECT in ./*/platformio.ini; do
    platformio run --project-dir ${PROJECT%/platformio.ini} || \
        { echo "Project: ${PROJECT} compilation failed"; exit 1; }
done