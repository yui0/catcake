#!/bin/sh
ant clean

pushd jni
/android-ndk-r8b/ndk-build V=1 -B
popd

#date=`date '+%Y.%m.%d'`
#sed -e "s/%%DATE%%/$date/" assets/help.html.orig > assets/help.html

ant debug
adb install bin/Kikyu-debug.apk
