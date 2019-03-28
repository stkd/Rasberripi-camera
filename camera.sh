#!/bin/bash

DATE=$(date +"%Y-%m-%d_%H%M")
/usr/bin/raspistill -vf -hf -q 90 -o /tmp/$DATE.jpg 2>&1 > /tmp/camera.log 
