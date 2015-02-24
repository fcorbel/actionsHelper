#!/bin/bash

echo "Try to get focused window name"

name=$(cat /proc/$(xdotool getwindowpid $(xdotool getwindowfocus))/comm)
echo $name
name=$(ps -e | grep $(xdotool getwindowpid $(xdotool getwindowfocus)) | grep -v grep | awk '{print $4}')
echo $name
