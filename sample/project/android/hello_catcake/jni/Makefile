BASE_DIR = .

CXX         = g++
CXXFLAGS    = -O2 -fno-strict-aliasing -Wall -s
LDFLAGS     =
DEFINES     = -DNDEBUG -DCK_LINUX
INCLUDES    = -I$(BASE_DIR)/include
LIBS        = -L$(BASE_DIR)/lib.x86 -L/usr/X11R6/lib -lcatcake -lasound -lfreetype -lpng -lz -lGL -lXxf86vm -lpthread -lX11
SRCS        = $(shell find . -name "*.cpp")
OBJS        = $(addsuffix .o, $(basename $(SRCS)))
TARGET      = hello_catcake
#TARGET      = $(notdir $(CURDIR))


.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)


.SUFFIXES: .o .cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
