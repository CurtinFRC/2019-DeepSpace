#!/bin/sh

# Enable password-less sudo for everyone
echo "%sudo ALL=(ALL) NOPASSWD:ALL" | sudo tee -a /etc/sudoers

# Install required packages
sudo apt-get update
sudo apt-get install -y avahi-daemon libnss-mdns v4l-utils imagemagick

# Add vision user and password
sudo adduser vision --disabled-password --gecos ""
sudo adduser vision sudo
sudo adduser vision video
sudo adduser vision audio
echo "vision:curtinfrc" | sudo chpasswd

# Set hostname, user and password
echo curtinvision | sudo tee /etc/hostname
echo "127.0.0.1 curtinvision" | sudo tee /etc/hosts

# Set Team #
echo Team Number?
read teamnum < /dev/tty
mkdir -p /home/vision
echo $teamnum > /home/vision/team

# Reboot to commit changes
sudo reboot