#!/bin/sh
ffmpeg -y -framerate 60 -i FRAMES/FRAME-%d.JPG -c:v libx264 FILM.MP4
