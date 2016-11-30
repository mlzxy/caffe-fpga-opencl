Install these softwares on

- **Host Machine**: The machine with FPGA installed!
- **USB Programming Machine**: The machine used to configure FPGA through USB Cable at the beginning, won't need afterwards

- **Development Machine**: The machine you use to write code.


## Host Machine: Xilinx `SDaccel`


- [Official Downloading & Installing & Configuration guide](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2015_4/ug1020-sdaccel-installation-guide.pdf)
- Some configuration Tips
    - Use that lame Centos 6.8, don't take a chance with the superb Centos 7. Otherwise, you may redo everything again :cry:
    - Use [Net install](https://www.if-not-true-then-false.com/2011/centos-6-netinstall-network-installation/) could save you from lots of trouble
    - Connect the Xilinx USB Cable Pins to FPGA in the correct way (or try both way)!  Make sure the **Green Light** on the cable driver is ON!

    - Recommend **Windows Machine** to program the OpenCL Framework to FPGA through USB Cable! Because the Windows version driver of the Xilinx USB Cable is more stable.

    - Install the **right kernel source**!
        - Use `yum install kernel-devel` may install the wrong one! Centos 6.8 seems to be ambiguous with kernel source package versioning.
        - View the kernel version number by `cd /lib/modules & ls` and download the corresponding rpm online.
        - Otherwise, **you may fail in the driver installation section** in the above guide.

    - Select the right FPGA device, be careful!
    ```
    xilinx:adm-pcie-ku3:2ddr:3.1
    xilinx:adm-pcie-ku3:2ddr:3.0
    xilinx:adm-pcie-ku3:1ddr:3.0
    xilinx:zc706:uart:1.0
    xilinx:tul-pcie3-ku115:2ddr:3.0
    xilinx:tul-pcie3-ku115:2ddr:3.1
    xilinx:adm-pcie-ku3:2ddr-xpr:3.1
    xilinx:adm-pcie-7v3:1ddr-ppc64le:2.1
    xilinx:adm-pcie-ku3:2ddr-xpr:3.0
    xilinx:adm-pcie-7v3:1ddr:3.0
    xilinx:adm-pcie-8k5:2ddr:3.1
    ```


## Host Machine: Linux `Centos 6.8` OS

As stated above, for the host machine.



## Host Machine: Cuda, If you want to Run it on GPU


Install Nvidia GPU OpenCL Environment (guide online available)



## Host Machine: Life saving Tools

- [Teamviewer](https://www.teamviewer.com/): To remotely view/control your host machine
- [Dropbox](https://www.dropbox.com/install-linux): Sync files between your development machine and host machine.
- [Sublime](https://www.sublimetext.com/): A decent text editor for old linux.

## USB Programming Machine: A Windows `7+` Machine (Or Virtual Machine)

As stated above, for the usb programing machine(won't need afterwards). I install `Windows 7` on my Centos using [VirtualBox](https://www.virtualbox.org/wiki/Downloads), even better than another PC since it's always there.


## USB Programming Machine: Xilinx Hardware Server & Cable Driver

From https://www.xilinx.com/support/download.html, I suggest you download the Vivado Design Suite Web Installer and install the license free version, which will include the USB Cable Driver and Hardware Server.


## Development Machine

- CPU OpenCL: For easy debugging and coding.
- [Caffe & Pycaffe](https://github.com/BVLC/caffe): To train caffe model.
- A reliable C++ IDE, for example [CLion](https://www.jetbrains.com/clion/).
