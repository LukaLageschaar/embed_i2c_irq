cmake -G "CodeBlocks - Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm-none-eabi-rpibplus.cmake ../ 
gmake 
cp ./kernel.img /Volumes/SD_card/
