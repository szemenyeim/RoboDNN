CXX=g++
RCXX=env  /usr/bin/xcrun clang++
CXXFLAGS=-I . -Wall -Wextra -Ofast -MD -ffast-math -fblocks -march=i686 -target i686-linux-elf 
CXXFLAGS += --sysroot /usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot -isystem /usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.8.5/include-fixed -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.8.5/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/include/boost-1_55 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/pthread/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/rt/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/jpeg/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/avahi/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/dbus/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/openni2/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/glib2/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/png/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/v4l/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tiff/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tbb/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/zlib/include		 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/opt/local/include			 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include/c++			 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include/c++/v1				 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/i686-aldebaran-linux-gnu/sys-include		 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/i686-aldebaran-linux-gnu/sys-include		 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/i686-aldebaran-linux-gnu/includ -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/include				 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/i686-aldebaran-linux-gnu/usr/include	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/i686-aldebaran-linux-gnu/usr/local/include	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/i686-aldebaran-linux-gnu/usr/include/bits	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/include/alsoap -DIS_LOCAL_OFF

BUILD = bin

_OBJS = Activations.o BLAS.o ConvLayers.o PoolLayers.o RoboDNN.o Utils.o UtilityLayers.o
OBJS = $(addprefix $(BUILD)/,$(_OBJS))

_ROBJS = Activations.o BLAS.o ConvLayers.o PoolLayers.o RoboDNN.o Utils.o UtilityLayers.o test.o
ROBJS = $(addprefix $(BUILD)/,$(_ROBJS))

LTEST = layerTests.cpp
TEST = test.cpp

DEPS = Activations.h BLAS.h Layer.h ConvLayers.h PoolLayers.h RoboDNN.h UtiliyLayers.h Utils.h

LDFLAGS = -lm $(BUILD)/RoboDNN.so

$(BUILD)/%.o: %.cpp
	@mkdir -p bin
	$(RCXX) -c $< $(CXXFLAGS) -o $@

lib: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(BUILD)/RoboDNN.so

tests: lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LTEST) -o $(BUILD)/layerTest
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TEST) -o $(BUILD)/test

robot: $(ROBJS)
	$(RCXX) $(CXXFLAGS) $^ -o $(BUILD)/test

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
