#! /bin/sh
./test-launch "(rpicamsrc bitrate=8500000 hflip=true vflip=true preview=false ! video/x-h264,width=640, height=480,framerate=45/1,profile=high ! h264parse ! rtph264pay name=pay0 pt=96 )"
