#!/bin/sh

# Install required packages
sudo apt-get update
sudo apt-get install -y avahi-daemon libnss-mdns v4l-utils

# Set hostname, user and password
echo curtinvision | sudo tee /etc/hostname
sudo usermod -l curtinfrc linaro
sudo usermod --password curtinfrc curtinfrc

# Reboot to commit changes
sudo reboot