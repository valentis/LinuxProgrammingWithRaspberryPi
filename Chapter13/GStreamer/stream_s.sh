#! /bin/sh
raspivid -t 0 -w 640 -h 480 -fps 30 -b 2000000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! gdppay ! tcpserversink host=172.30.1.16 port=5000

