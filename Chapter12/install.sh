#! /bin/sh
sudo apt-get update
sudo apt-get -y install libncurses5-dev bc bison flex libssl-dev
cd
git clone https://github.com/raspberrypi/linux.git
sudo mv linux /usr/src
cd /usr/src/linux
sudo make mrproper && sudo ARCH=arm make bcm2711_defconfig
sudo make bzImage && sudo make modules && sudo make dtbs && sudo make modules_install
sudo cp arch/arm/boot/dts/*.dtb /boot
sudo cp arch/arm/boot/dts/overlays/*.dtb* /boot/overlays
sudo cp arch/arm/boot/dts/overlays/README /boot/overlays
sudo cp /boot/kernel7l.img /boot/kernel7l_old.img
sudo cp ./arch/arm/boot/zImage /boot/kernel7l.img
