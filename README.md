  linux-mpu9150
========

An implementation of 9-axis data fusion on Linux using the [InvenSense MPU-9150 IMU][1]

The linux-mpu9150 code uses the InvenSense Embedded Motion Driver v5.1.1 SDK
to obtain fused 6-axis quaternion data from the MPU-9150 DMP. The quaternion
data is then further corrected in the linux-mpu0150 code with magnetometer 
data collected from the IMU.

Because it is primarily intended for use on small-board ARM computers, the 
linux-mpu9150 code uses 32-bit floats rather then doubles for all floating 
point operations.

Testing has been done with [Raspberry Pi][2] and [Gumstix Overo and Duovero][3] systems.
The code should work fine with other small board systems like the [BeagleBones][4].

We've been using [Sparkfun MPU9150 Breakout Boards][5] for testing this code.

[1]: http://www.invensense.com/mems/gyro/nineaxis.html     "InvenSense"
[2]: http://www.raspberrypi.org/                           "Raspberry Pi"
[3]: https://www.gumstix.com/                              "Gumstix"
[4]: http://beagleboard.org/                               "Beagleboard"
[5]: https://www.sparkfun.com/products/11486               "Sparkfun"

  Calibration
========

The Invensense code provides self-calibration of the gyros whenever they are
idle more then 8 seconds.

There are functions in linux-mpu9150 to accept calibration data for the 
accelerometers and magnetometers, but currently no example implementation on 
how to generate that data. 

It's on the TODO list.


  Fetch
========

Use git to fetch the linux-mpu9150 project. You may have to install git on your
system first.

For RPi users running Raspbian, use this command

        sudo apt-get install git

Then to clone the repository assuming you have an Internet connection

        git clone git://github.com/Pansenti/linux-mpu9150.git


  Build
========

The linux-mpu9150 code is written in C. There is a make file called *Makefile-native*
for use when building directly on the system.

There is also a *Makefile-cross* makefile for use when using Yocto Project built
tools and cross-building linux-mpu9150 on a workstation.

A recommendation is to create a soft-link to the make file you want to use.

        root@duovero:~$ cd linux-mpu9150
        root@duovero:~/linux-mpu9150$ ln -s Makefile-native Makefile


After that you can just type make to build the code.

        root@duovero:~/linux-mpu9150$ make
        gcc -Wall -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -I eMPL -I glue -c eMPL/inv_mpu.c
        gcc -Wall -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -I eMPL -I glue -c eMPL/inv_mpu_dmp_motion_driver.c
        gcc -Wall -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -I eMPL -I glue -c glue/linux_glue.c
        gcc -Wall -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -I eMPL -I glue -c mpu9150/mpu9150.c
        gcc -Wall -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -c mpu9150/quaternion.c
        gcc -Wall -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -c mpu9150/vector3d.c
        gcc -Wall -I eMPL -I glue -I mpu9150 -DEMPL_TARGET_LINUX -DMPU9150 -DAK8975_SECONDARY -c main.c
        gcc -Wall inv_mpu.o inv_mpu_dmp_motion_driver.o linux_glue.o mpu9150.o quaternion.o vector3d.o main.o -lm -o imu 


The resulting executable is called *imu*.

  Enable i2c
========

Take a look at /etc/modules to see that the following two lines are present:

i2c-bcm2708
i2c-dev

If not, add them! 

To avoid having to use sudo to run the code, create a file:

/etc/udev/rules.d/90-i2c.rules

and add the line:

KERNEL=="i2c-[0-3]",MODE="0666"

then reboot to make sure everything is set correctly.

  Run
========

There are just a few parameters at present.

        pi@raspberrypi ~/linux-mpu9150 $ ./imu -h

        Usage: ./imu [options]
          -b <i2c-bus>          The I2C bus number where the IMU is. The default is 1 to use /dev/i2c-1.
          -s <sample-rate>      The IMU sample rate in Hz. Range 2-50, default 10.
          -y <yaw-mix-factor>   Effect of mag yaw on fused yaw data.
                                0 = gyro only
                                1 = mag only
                                > 1 scaled mag adjustment of gyro data
                                The default is 4.
          -h                    Show this help

        Example: ./imu -b3 -s20 -y10


The defaults will work for an RPi.

        pi@raspberrypi ~/linux-mpu9150 $ ./imu

        Initializing IMU .......... done


        Entering read loop (ctrl-c to exit)

         X: -2 Y: -62 Z: -4        ^C

