leddarUI
========

qt interface for leddar

## Build

    cd build (mkdir if it does not exist)
    cmake ..
    make
    ./leddartech_node (to execute)


## Configuration 



## Other useful information
- Username : pi
- Password : ninja
- Go back to terminal from X Session (keyboard shortcut) : Ctrl + Alt + F1
- You can start X session from terminal : startx(PiTFT screen) or FRAMEBUFFER=/dev/fb0 startx (HDMI) (if it does not work, do the command "mv /usr/share/X11/xorg.conf.d/99-pift.conf ~" and do the inverse command if you want to lauch on PiTFT screen by default).
- Different screen resolution to start X session : startx -- -dpi 30
- Start wifi config from a terminal in X session : /usr/sbin/wpa_gui

