#!/usr/bin/python

import RPi.GPIO as GPIO

channel=18
GPIO.setmode(GPIO.BCM)
GPIO.setup(channel, GPIO.IN, pull_up_down=GPIO.PUD_UP)
