# ©2013 YUICHIRO NAKADA

PACKAGE=catcake-base
OBJS=$(PACKAGE).o

CATCAKE=/root/prog/catcake

#CROSS=mipsel-uclibc-
CC=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar
RANLIB=$(CROSS)ranlib

#CCFLAGS+=-Os -Wall -I. -Wno-pointer-sign -fomit-frame-pointer
CCFLAGS+=-Os -Wall -I. -I$(CATCAKE)/include -fomit-frame-pointer -Wno-missing-braces
LDFLAGS+=-s -L$(CATCAKE)/lib -L/usr/X11R6/lib
LIBS=-lcatcake -lasound -lmad -lfreetype -lpng -ljpeg -lz -lGL -lXxf86vm -lpthread -lX11


# for Windows (Digital Mars C++)
WCC=		wine ./dm/bin/dmc
WCFLAGS=	-I./dm/stlport/stlport -I. -D__LCC__
## http://curl.haxx.se/download.html
# wine dm/bin/implib.exe /s curl.lib libcurl.dll
# wine dm/bin/implib.exe /s miniupnpc.lib miniupnpc.dll
#WLDFLAGS=	SoftPixelEngine.lib
WOBJS=$(PACKAGE).cpp


all: $(PACKAGE)

$(PACKAGE): $(OBJS)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(OBJS) -o $(PACKAGE) $(LIBS)

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

win: $(WOBJS)
	$(WCC) $(WCFLAGS) $(WOBJS) $(WLDFLAGS)

android:
	sh $(CATCAKE)/project/android/android-app/build.sh $(PACKAGE) $(PACKAGE) "$(PACKAGE).cpp" assets $(CATCAKE)

clean:
	rm -f *.o *.obj *.map $(PACKAGE)
