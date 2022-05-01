This is a fork of the original arduino-cmake project, but targetted at STM32duino and ESP32duino (WIP for ESP32)


**STM32 Installation :** 

- Install a toolchain.  I'd recommand using the arduino-stm32 toolchain available here https://github.com/stm32duino/arm-none-eabi-gcc . The code is about 50% smaller.

- Install the Arduino STM32 files from here (https://github.com/rogerclarkmelbourne/Arduino_STM32). Either put the files in ARDUINO_SDK/harwdare/arduino or anywhere else you want.
Note down the full path to the STM32 F1 folder
- Be sure you have a STLINKv2 clone and that you know how to reflash the bootloader. (i.e. cd /opt/arduino-1.8.5/hardware/STM32/arduino/tools/linux64/stlink && ./st-flash  write ~/Arduino_stm32/STM32duino-bootloader/binaries/generic_boot20_pc13.bin 0x8000000)

    - Copy the example/stm32 content somewhere so that you have a structure like 
|    Cmakelists.txt 
|    Hello.ino 
|    cmake/   (symlink to the cmake subfolder of arduino-cmake-stm32, or just copy it) 

    - Change the cmakelists.txt and use paths you noted before (toolchain &  arduino STM32 file in the  example)
   e.g. 
|    # TOOLCHAIN |
|    SET(PLATFORM_TOOLCHAIN_PATH "/home/fx/Arduino_stm32/arm-none-eabi-gcc/download/gcc-arm-none-eabi-8.2.1-1.7/bin") |
|    # STM32Duino, point to the STM32F1 subfolder |
|    SET(PLATFORM_PATH "/home/fx/Arduino/hardware/stm32/STM32F1") 
    
- cmake .. && make

You now have the elf & bin files that you can upload manually

  /opt/arduino-1.8.5/hardware/Arduino_STM32-master/tools/linux/maple_upload ttyACM0 2 1EAF:0003 the_file.bin

1EAF:0003 is the usb id, it can change a bit, mine is 1eaf:0004, lsusb will tell you what you have

Important: 
  - Tested only on linux, might not work well on OsX and Windows
