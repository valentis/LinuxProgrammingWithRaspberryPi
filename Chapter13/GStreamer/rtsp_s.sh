#! /bin/sh
raspivid -t 0 -h 720 -w 1280 -fps 25 -b 2000000 -vf -hf -n -o - | gst-launch -v fdsrc ! h264parse ! gdppay ! tcpserversink port=5000 | ./test-launch "( tcpclientsrc host=localhost port=5000 ! gdpdepay ! h264parse ! rtph264pay name=pay0 pt=96 )"
