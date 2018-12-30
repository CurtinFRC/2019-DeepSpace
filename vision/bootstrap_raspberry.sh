#!/bin/sh
sudo apt-get install -y avahi-daemon libnss-mdns
echo curtinvision | sudo tee /etc/hostname

sudo usermod -l curtinfrc linaro
sudo usermod --password curtinfrc curtinfrc

sudo reboot