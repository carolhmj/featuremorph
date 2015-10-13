#!/bin/bash
ffmpeg -f image2 -framerate 24 -i image%03d.png -s 570x720 saida_nova.mp4

