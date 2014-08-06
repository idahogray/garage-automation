FROM idahogray/arch-arm
MAINTAINER Keith Gray "idahogray@gmail.com"
ENV REFRESHED_AT 2014-08-05
RUN pacman -Syu --noconfirm --ignore filesystem
RUN pacman -S --noconfirm python2 python2-pip base-devel git
RUN pip2 install pymodbus
RUN git clone https://github.com/idahogray/garage-automation.git /home/garage-automation
