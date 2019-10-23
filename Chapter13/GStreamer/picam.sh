#! /bin/sh
gst-launch-1.0 rpicamsrc bitrate=1000000 ! "video/x-h264, width=800, height=600, framerate=25/1" ! rtph264pay config-interval=1 pt=96 ! gdppay ! udpsink host=172.30.1.16 port=5000

