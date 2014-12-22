leddarUI
========

qt interface for leddar

## Build

    cd build (mkdir if it does not exist)
    cmake ..
    make
    ./leddartech_node (to execute)


## Configuration Parameters Description
- OVERSAMPLING_EXPONENT : Number of oversampling cycles, Higher values enhance accuracy/ precision/resolution and reduce measurement rate. Oversampling can only take values that are a power of two. This property can have a value between 0 and 3, giving oversampling of 1, 2, 4 and 8.
- ACCUMULATION_EXPONENT : Number of accumulations, higher values enhance range, reduce measurement rate and noise. The number of accumulations can only take values that are a power of two. This property can have a value between 0 and 10 giving a number of accumulations of 1, 2, 4, 8, 16, 32, 64, 128, 256, 512 or 1024.
- BASE_POINT_COUNT : Number of base sample points, determines maximum detection range. The number of base samples acquired by the sensor. The number of samples that the sensor acquires can be changed. However the number of samples set here is the "base" number before oversampling. So for example setting this property to 30 and oversampling to 8 will give 240 samples.
- AUTOMATIC_LED_INTENSITY : Control of automatic LED intensity. A value of 0 will set manual intensity mode. Any other value will set automatic mode where the LED intensity is computed by the sensor.
- LED_INTENSITY : The relative intensity of LEDs from 0 to 16. (2=10%, 3=20%, 5=35%, 7=50%, 9=65%, 11=80%, 12=90% 16=100%)
- CHANGE_DELAY : Number of measurements of filtering for automatic LED control. When true, this parameter controls the reactivity of the control. The higher this value, the slower the sensor will be to adjust the LED intensity to condition changes. This property is specified as a number of measurement, so the effective delay will depend on the current measurement rate.
- THRESHOLD_OFFSET : An offset to increase the detection threshold. By default this property is 0 which makes the sensor very sensitive. However depending on your application this may yield many "false" detections with low amplitude that are just noise. To eliminate them you can increase this value. The value to use is application dependant and must be found by experimentation.




## Other useful information
- Username : pi
- Password : ninja
- Go back to terminal from X Session (keyboard shortcut) : Ctrl + Alt + F1
- You can start X session from terminal : startx(PiTFT screen) or FRAMEBUFFER=/dev/fb0 startx (HDMI) (if it does not work, do the command "mv /usr/share/X11/xorg.conf.d/99-pift.conf ~" and do the inverse command if you want to lauch on PiTFT screen by default).
- Different screen resolution to start X session : startx -- -dpi 30
- Start wifi config from a terminal in X session : /usr/sbin/wpa_gui
- The program is launched just after the X session is started. The script that makes this behavior is /etc/X11/xsession.d/99-leddartech. 
- 10-leddartech.rules must be placed in /etc/udev/rules.d/ folder

