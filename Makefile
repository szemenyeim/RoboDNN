CXX=g++
CXXFLAGS=-I . -Wall -Wextra -Ofast -MD -ffast-math -march=native 
BUILD = bin
RBUILD = build.nao-atom

RCXX = env  /usr/bin/xcrun clang++
RCXXFLAGS = -I . -Wall -Wextra -Ofast -MD -ffast-math -march=i686 -fPIC -DSO -fno-exceptions -std=c++11
INCLUDES = -I/Users/martonszemenyei/src/MiPal/GUNao/posix/guvision -DV4L -I/Users/martonszemenyei/src/MiPal/GUNao/posix/guUDPreceiver -I/Users/martonszemenyei/src/MiPal/GUNao/posix/gusimplewhiteboard -I/Users/martonszemenyei/src/MiPal/GUNao/Common --sysroot /usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot				 -isystem /usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.8.5/include-fixed -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.8.5/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/include/boost-1_55 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/pthread/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/rt/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/jpeg/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/avahi/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/dbus/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/openni2/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/glib2/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/png/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/v4l/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tiff/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tbb/include -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/zlib/include	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/opt/local/include			 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include/c++			 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/include/c++/v1				 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/i686-aldebaran-linux-gnu/sys-include		 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/i686-aldebaran-linux-gnu/sys-include		 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/i686-aldebaran-linux-gnu/include		 				 				 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/include				 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/i686-aldebaran-linux-gnu/usr/include	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/i686-aldebaran-linux-gnu/usr/local/include	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/i686-aldebaran-linux-gnu/usr/include/bits	 -I/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/include/alsoap -DNAO_V4 -DNAOQI_VERSION_MAJOR=2 -I/Users/martonszemenyei/src/MiPal/GUNao/Common -I/Users/martonszemenyei/src/MiPal/GUNao/posix/gusimplewhiteboard/typeClassDefs -I/Users/martonszemenyei/src/MiPal/GUNao/posix -DGUV4LVISION_IS_REMOTE_ON -DGUV4LVISION_IS_LOCAL_OFF -DIS_REMOTE_ON -DIS_LOCAL_OFF -DUSE_OPENCV -DNAO -DUSE_LIBDISPATCH -march=i686 -fPIC

LD = env  /usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/bin/i686-aldebaran-linux-gnu-gcc -rdynamic
RLDFLAGS =  --sysroot /usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot				 -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.8.5 -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/lib/boost-1_55 -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/pthread/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/rt/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/jpeg/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/avahi/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/dbus/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/openni2/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/glib2/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/png/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/v4l/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tiff/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tbb/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/zlib/lib					 -Wl,-R/home/nao/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.8.5 -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/local/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/lib/boost-1_55 -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/libnaoqi/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/pthread/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/rt/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/jpeg/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/avahi/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/dbus/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/openni2/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/glib2/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/png/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/v4l/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tiff/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/tbb/lib -Wl,-rpath-link,/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/staging/atom/zlib/lib -lgcc -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/lib -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/i686-aldebaran-linux-gnu/sysroot/usr/lib	 -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/lib		 -L/usr/local/naoqi-sdk-2.1.4.13-mac64/crosstoolchain/cross/atom/lib/gcc/i686-aldebaran-linux-gnu/4.5.3						 -Wl,-R/opt/local/lib			 -Wl,-R/usr/local/lib			 -Wl,-R/usr/lib/qt4				 -lc -lc++ -lm

_OBJS = Activations.o BLAS.o ConvLayers.o PoolLayers.o RoboDNN.o Utils.o UtilityLayers.o
OBJS = $(addprefix $(BUILD)/,$(_OBJS))

LTEST = layerTests.cpp
TEST = test.cpp

_ROBJS = $(_OBJS)
_ROBJS += test.o
ROBJS = $(addprefix $(RBUILD)/,$(_ROBJS))


DEPS = Activations.h BLAS.h Layer.h ConvLayers.h PoolLayers.h RoboDNN.h UtiliyLayers.h Utils.h

LDFLAGS = -lm $(BUILD)/RoboDNN.so

$(BUILD)/%.o: %.cpp
	@mkdir -p bin
	$(CXX) -c $< $(CXXFLAGS) -o $@

$(RBUILD)/%.o: %.cpp
	@mkdir -p build.nao-atom
	$(RCXX) -c $< -fblocks $(RCXXFLAGS) -stdlib=libc++ -target i686-linux-elf $(INCLUDES) -o $@

lib: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(BUILD)/RoboDNN.so

tests: lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LTEST) -o $(BUILD)/layerTest
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TEST) -o $(BUILD)/test

robot: $(ROBJS)
	$(LD) $(RCXXFLAGS) $(INCLUDES) $(RLDFLAGS) $^ -o $(RBUILD)/test

clean:
	rm -rf $(BUILD)
	rm -rf $(RBUILD)

-include $(OBJS:.o=.d)
