 #!/bin/sh
 # http://gpp256.sytes.net:18080/wiki/?Android%2FLinux%A4%AB%A4%E9%A4%CE%B0%DC%BF%A2%A5%CE%A5%A6%A5%CF%A5%A6
CONFIG="--disable-id3v2 --disable-lfs-alias --disable-feature-report --with-seektable=0 --disable-8bit --disable-messages --disable-feeder --disable-ntom --disable-downsample --disable-icy"

 mkdir -p bin.android/lib
 export NDK_TOP=/android-ndk
 export SYSROOT=${NDK_TOP}/platforms/android-9/arch-arm
 export CFLAGS="-march=armv5te -msoft-float -Os -s "
 export CC="${NDK_TOP}/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc -mandroid --sysroot=$SYSROOT"
 export LD_LIBRARY_PATH=bin.android/lib:${LD_LIBRARY_PATH}
 mkdir obj.android
 (cd obj.android; ../configure --host=arm-eabi --prefix=$(pwd)/bin.android/ ${CONFIG} )
