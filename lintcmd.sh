#!/bin/sh

PATTERN=`echo $* | tr " " "|"`

lint -g \
	-I/usr/local/include/portaudio2 \
	-I/usr/local/include/ffmpeg1 \
	-ipHceh \
	$* 2>&1 |
	egrep "($PATTERN)"
