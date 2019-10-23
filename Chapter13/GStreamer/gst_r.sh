#! /bin/sh
gst-launch-1.0 -v rtspsrc location="rtsp://172.30.1.16:8554/test" latency=0 ! rtph264depay ! ffdec_h264 ! queue ! videoconvert ! autovideosink
