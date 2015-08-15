# Execute this script as root

export KALI_ROOT=/home/pi/_DEV/kaliscope/
. $KALI_ROOT/init.sh

python $KALI_ROOT/setupGpio.py

find $KALI_ROOT -name kalisync -exec {} --motorPin=17 --flashPin=27 --watch=18 --gpioDelay=40 --useTinyDisplay=true \;

