#!/bin/bash
#
# Hydra moisture sensor test script
# - Change sensor address from 3 to 1
#
# modpoll must exist with path ../../tools/modpoll (outside MPLAB X project)
# Download modpoll from https://www.modbusdriver.com/modpoll.html
#
# #################################################################################

echo ""
echo ""
echo "HYDRA MOISTURE SENSOR - CHANGE SENSOR ADDRESS 3 TO 1"
echo ""

echo ">> 1) Confirm Sensor Address 3"
echo ""
echo ">>    Read Input Registers"
echo ""
~/bin/modpoll/modpoll -m rtu -a 3 -r 1 -c 3 -t 3 -b 19200 -d 8 -p none -1 /dev/ttyUSB0
echo ""
echo "  [1] Soil moisture [unsigned int]"
echo "  [2] Temperature [signed deg C x 10]"
echo "  [3] Software Version [majorVer 4b][featureVer 4bit][bugfixVer 8b]"
echo ""

echo ">>    Read Holding Registers"
echo ""
~/bin/modpoll/modpoll -m rtu -a 3 -r 1 -c 5 -t 4 -b 19200 -d 8 -p none -1 /dev/ttyUSB0
echo ""
echo "  [1] Modbus address"
echo "  [2] Baud rate"
echo "  [3] Parity"
echo "  [4] Measurement interval (msec)"
echo "  [5] Go to sleep time (secs)"
echo ""

echo ">> 2) Set Sensor Address to 1"
echo ""
~/bin/modpoll/modpoll -m rtu -a 3 -r 1 -c 1 -t 4 -b 19200 -d 8 -p none -1 /dev/ttyUSB0 1
echo ""

echo ">> 4) Wait for sensor to restart..."
sleep 5
echo ""

echo ">> 3) Confirm Sensor Address 1"
echo ""
echo ">>    Read Input Registers"
echo ""
~/bin/modpoll/modpoll -m rtu -a 1 -r 1 -c 3 -t 3 -b 19200 -d 8 -p none -1 /dev/ttyUSB0
echo ""
echo "  [1] Soil moisture [unsigned int]"
echo "  [2] Temperature [signed deg C x 10]"
echo "  [3] Software Version [majorVer 4b][featureVer 4bit][bugfixVer 8b]"
echo ""

echo ">>    Read Holding Registers"
echo ""
~/bin/modpoll/modpoll -m rtu -a 1 -r 1 -c 5 -t 4 -b 19200 -d 8 -p none -1 /dev/ttyUSB0
echo ""
echo "  [1] Modbus address"
echo "  [2] Baud rate"
echo "  [3] Parity"
echo "  [4] Measurement interval (msec)"
echo "  [5] Go to sleep time (secs)"
echo ""

echo "DONE"
echo ""
