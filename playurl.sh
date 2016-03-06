#!/bin/bash

gcc -O2 -o playurl playurl.c -lmpg123 -lao -lcurl

#./playurl http://url.to/file.mp3
