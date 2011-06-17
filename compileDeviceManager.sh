#!/bin/bash

## USAGE : 
## ./compile.sh LINUX
##         OR
## ./compile.sh LINUX64
##         OR
## ./compile.sh MACOS

CURRENT_PATH=`pwd`

OS=$1

if [ "$OS" = "LINUX" ] || [ "$OS" = "LINUX64" ]
then
{
DEVICE_MANAGER_PATH='/home/luc/Documents/workspace/Device-Manager'
LIB_ISCORE_PATH='/home/luc/Documents/workspace/libIscore'
ACOUSMOSCRIBE_PATH='/home/luc/Documents/workspace/Acousmoscribe'
}
elif [ "$OS" = "MACOS" ]
then
{
DEVICE_MANAGER_PATH='/Users/luc/Documents/workspace/Device-Manager'
LIB_ISCORE_PATH='/Users/luc/Documents/workspace/libIscore'
ACOUSMOSCRIBE_PATH='/Users/luc/Documents/workspace/Acousmoscribe'
}
fi

QMAKE_LINUX='qmake -spec linux-g++'
QMAKE_LINUX_64='qmake -spec linux-g++-64'
QMAKE_MACOS='qmake -spec macx-g++'
MAKE='make'
MAKE_LINUX='make linux'
MAKE_MAC='make mac'
MAKE_CLEAN='make clean'

echo '-=[START]=-'

## Device-Manager Compilation

echo '-=[Device Manager : Library]=- Compiling ...'
cd $DEVICE_MANAGER_PATH
if [ "$OS" = "LINUX" -o "$OS" = "LINUX64" ]
then
{
echo '-=[Device Manager : LINUX]=-'
echo '-=[Device Manager : Library]=- Cleaning ...'
$MAKE_CLEAN
echo '-=[Device Manager : Library]=- Compiling  ...'
$MAKE_LINUX
echo '-=[Device Manager : Library]=- Installing ...'
sudo cp libDeviceManager.a /usr/local/lib/
sudo mkdir -p /usr/local/include/DeviceManager/
sudo cp DeviceManager.h /usr/local/include/DeviceManager/
echo '-=[Device Manager : Library]=- Done.'
cd Plugins/Linux_MacOS/
echo '-=[Device Manager : Plugins]=- Cleaning ...'
$MAKE_CLEAN
echo '-=[Device Manager : Plugins]=- Compiling  ...'
chmod u+x compileForLinux.sh
./compileForLinux.sh
$MAKE_LINUX
echo '-=[Device Manager : Plugins]=- Installing ...'
sudo mkdir -p /usr/local/lib/IScore/
sudo cp Plugins/*.so /usr/local/lib/IScore/
}
elif [ "$OS" = "MACOS" ]
then
{
echo '-=[Device Manager : MACOS]=-'
echo '-=[Device Manager : Library]=- Cleaning ...'
$MAKE_CLEAN
echo '-=[Device Manager : Library]=- Compiling  ...'
$MAKE_MAC
echo '-=[Device Manager : Library]=- Installing ...'
sudo cp libDeviceManager.a /usr/local/lib/
sudo mkdir -p /usr/local/include/DeviceManager/
sudo cp DeviceManager.h /usr/local/include/DeviceManager/
echo '-=[Device Manager : Library]=- Done.'
cd Plugins/Linux_MacOS/
echo '-=[Device Manager : Plugins]=- Cleaning ...'
$MAKE_CLEAN
echo '-=[Device Manager : Plugins]=- Compiling  ...'
chmod u+x compileForMac.sh
./compileForMac.sh
echo '-=[Device Manager : Plugins]=- Compiling  ...'
$MAKE_MAC
echo '-=[Device Manager : Plugins]=- Installing ...'
sudo mkdir -p /usr/local/lib/IScore/
sudo cp Plugins/*.dylib /usr/local/lib/IScore/
}
fi