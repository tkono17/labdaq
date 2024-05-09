#!/usr/bin/env bash
cd /home/tkono/work/2024.Cosmic/dev
dn0=$(pwd)

cd Drivers
dn1=$(pwd)

cd CAENComm-v1.7.0/lib
sudo sh install_x64
cd ${dn1}

cd CAENVMELib-v4.0.2/lib
sudo sh install_x64
cd ${dn1}

cd CAENDigitizer-v2.17.3/lib
sudo sh install_x64
cd ${dn1}

cd CAENUSBdrvB-1.5.5
make
sudo make install
cd ${dn1}

