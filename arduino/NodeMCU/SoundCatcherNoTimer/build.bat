REM @echo off 
REM set proj_path=C:/Users/axlan/Documents/Sound Catcher/arduino/NodeMCU/SoundCatcherNoTimer/
REM @echo off

REM mkdir artifacts
REM mkdir artifacts/build
REM mkdir artifacts\cache

REM "C:/Program Files (x86)/Arduino/arduino-builder" -dump-prefs -logger=machine -hardware "C:/Program Files (x86)/Arduino/hardware" -hardware "C:/Users/axlan/AppData/Local/Arduino15/packages" -tools "C:/Program Files (x86)/Arduino/tools-builder" -tools "C:/Program Files (x86)/Arduino/hardware/tools/avr" -tools "C:/Users/axlan/AppData/Local/Arduino15/packages" -built-in-libraries "C:/Program Files (x86)/Arduino/libraries" -libraries "C:/Users/axlan/Documents/Arduino/libraries" -fqbn=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=115200,FlashSize=4M3M -ide-version=10802 -build-path "%proj_path%artifacts/build" -warnings=none -build-cache "%proj_path%artifacts/cache" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.esptool.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/esptool/0.4.9" -prefs=runtime.tools.xtensa-lx106-elf-gcc.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2" -prefs=runtime.tools.mkspiffs.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/mkspiffs/0.1.2" -verbose "%proj_path%/SoundCatcherNoTimer.ino"

REM "C:/Program Files (x86)/Arduino/arduino-builder" -compile -logger=machine -hardware "C:/Program Files (x86)/Arduino/hardware" -hardware "C:/Users/axlan/AppData/Local/Arduino15/packages" -tools "C:/Program Files (x86)/Arduino/tools-builder" -tools "C:/Program Files (x86)/Arduino/hardware/tools/avr" -tools "C:/Users/axlan/AppData/Local/Arduino15/packages" -built-in-libraries "C:/Program Files (x86)/Arduino/libraries" -libraries "C:/Users/axlan/Documents/Arduino/libraries" -fqbn=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=115200,FlashSize=4M3M -ide-version=10802 -build-path "%proj_path%artifacts/build" -warnings=none -build-cache "%proj_path%artifacts/cache" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.esptool.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/esptool/0.4.9" -prefs=runtime.tools.xtensa-lx106-elf-gcc.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2" -prefs=runtime.tools.mkspiffs.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/mkspiffs/0.1.2" -verbose "%proj_path%/SoundCatcherNoTimer.ino" 

REM copy artifacts\build\SoundCatcherNoTimer.ino.bin  artifacts\

REM RMDIR  /Q /S artifacts\build

REM REM 2> out/build.log

REM REM type out\\build.log | sed s/\\out\\sketch//

@echo off 
set proj_path=C:/Users/axlan/Documents/Sound Catcher/arduino/NodeMCU/SoundCatcherNoTimer/
set build_path=C:\Users\axlan\AppData\Local\Temp\arduino_build_733127
@echo off

mkdir out

"C:/Program Files (x86)/Arduino/arduino-builder" -dump-prefs -logger=machine -hardware "C:/Program Files (x86)/Arduino/hardware" -hardware "C:/Users/axlan/AppData/Local/Arduino15/packages" -tools "C:/Program Files (x86)/Arduino/tools-builder" -tools "C:/Program Files (x86)/Arduino/hardware/tools/avr" -tools "C:/Users/axlan/AppData/Local/Arduino15/packages" -built-in-libraries "C:/Program Files (x86)/Arduino/libraries" -libraries "C:/Users/axlan/Documents/Arduino/libraries" -fqbn=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=115200,FlashSize=4M3M -ide-version=10802 -build-path "%build_path%" -warnings=none -build-cache "C:/Users/axlan/AppData/Local/Temp/arduino_cache_719912" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.esptool.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/esptool/0.4.9" -prefs=runtime.tools.mkspiffs.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/mkspiffs/0.1.2" -prefs=runtime.tools.xtensa-lx106-elf-gcc.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2" -verbose "%proj_path%/SoundCatcherNoTimer.ino"
"C:/Program Files (x86)/Arduino/arduino-builder" -compile -logger=machine -hardware "C:/Program Files (x86)/Arduino/hardware" -hardware "C:/Users/axlan/AppData/Local/Arduino15/packages" -tools "C:/Program Files (x86)/Arduino/tools-builder" -tools "C:/Program Files (x86)/Arduino/hardware/tools/avr" -tools "C:/Users/axlan/AppData/Local/Arduino15/packages" -built-in-libraries "C:/Program Files (x86)/Arduino/libraries" -libraries "C:/Users/axlan/Documents/Arduino/libraries" -fqbn=esp8266:esp8266:nodemcuv2:CpuFrequency=80,UploadSpeed=115200,FlashSize=4M3M -ide-version=10802 -build-path "%build_path%" -warnings=none -build-cache "C:/Users/axlan/AppData/Local/Temp/arduino_cache_719912" -prefs=build.warn_data_percentage=75 -prefs=runtime.tools.esptool.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/esptool/0.4.9" -prefs=runtime.tools.mkspiffs.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/mkspiffs/0.1.2" -prefs=runtime.tools.xtensa-lx106-elf-gcc.path="C:/Users/axlan/AppData/Local/Arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2" -verbose "%proj_path%/SoundCatcherNoTimer.ino" 2> out/build.log

copy %build_path%\SoundCatcherNoTimer.ino.bin out\

type out\build.log | sed s/\\out\\sketch//