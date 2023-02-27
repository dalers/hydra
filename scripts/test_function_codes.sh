#!/bin/bash
#
# Hydra moisture sensor test script
# - Test function codes (Address 1)
#
# modpoll must exist with path ../../tools/modpoll (outside MPLAB X project)
# Download modpoll from https://www.modbusdriver.com/modpoll.html
#
# #################################################################################

echo ""
echo ""
echo "HYDRA MOISTURE SENSOR - TEST FUNCTION CODES (ADDRESS 1)"
echo ""

echo ">> 1) Read Input Registers"
echo "      Reg 1 - Soil moisture. Unsigned"
echo "      Reg 2 - Temperature (signed deg C x 10)"
echo ""

~/bin/modpoll/modpoll -m rtu -1 -a 1 -r 1 -c 2 -t 3 -b 19200 -d 8 -p none /dev/ttyUSB0
echo ""

# -m rtu  : mode = rtu
# -1      : poll only once
# -a 1    : device id = 1
# -r 1    : start reference  = 1
# -c 5    : address count = 2 (registers 0 to 1)
# -t 3    : type = 3 "16-bit input register data type (default)"
# -b 9600 : baudrate = 19200
# -d 8    : data size = 8-bit (RTU)
# -p none : partity = none
# ttyUSB0 : port = /devtty/USB0 (Linux)

#
# #################################################################################
#

echo ">> 2) Read Holding Registers"
echo "      Reg 1 - Modbus address"
echo "      Reg 2 - Baud rate"
echo "      Reg 3 - Parity"
echo "      Reg 4 - Measurement interval (msec)"
echo "      Reg 5 - Go to sleep time (sec)"
echo ""

~/bin/modpoll/modpoll -m rtu -1 -a 1 -r 1 -c 5 -t 4 -b 19200 -d 8 -p none /dev/ttyUSB0
echo ""

# -m rtu  : mode = rtu
# -1      : poll only once
# -a 1    : device id = 1
# -r 1    : start reference  = 1
# -c 5    : address count = 5 (registers 0 to 4)
# -t 4    : type = 4 "16-bit output (holding) register data type (default)"
# -b 9600 : baudrate = 19200
# -d 8    : data size = 8-bit (RTU)
# -p none : partity = none
# ttyUSB0 : port = /devtty/USB0 (Linux)

#
# #################################################################################
#

echo ">> 3) Change Modbus Address"
echo "       TODO implement test"
echo ""

echo ">> 4) Change Baud rate"
echo "       TODO implement test"
echo ""

echo ">> 5) Change Parity"
echo "       TODO implement test"
echo ""

echo ">> 6) Change Measuremnt interval"
echo "       TODO implement test"
echo ""

echo ">> 7) Go to sleep"
echo "       TODO implement test"
echo ""

echo ""
echo "TEST COMPLETE"
echo ""
