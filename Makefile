CXX=g++
CXXFLAGS=-I . -Wall -Wextra -Ofast -MD

BUILD = bin

_OBJS = Activations.o BLAS.o ConvLayers.o PoolLayers.o RoboDNN.o Utils.o UtilityLayers.o
OBJS = $(addprefix $(BUILD)/,$(_OBJS))

LTEST = layerTests.cpp
TEST = test.cpp

DEPS = Activations.h BLAS.h Layer.h ConvLayers.h PoolLayers.h RoboDNN.h UtiliyLayers.h Utils.h

LDFLAGS = $(BUILD)/RoboDNN.so

$(BUILD)/%.o: %.cpp
	@mkdir -p bin
	$(CXX) -c $< $(CXXFLAGS) -o $@

lib: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(BUILD)/RoboDNN.so

tests: lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LTEST) -o $(BUILD)/layerTest
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -lopencv_core -lopencv_imgcodecs $(TEST) -o $(BUILD)/test

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
