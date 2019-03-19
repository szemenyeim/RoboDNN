CXX=g++
CXXFLAGS=-I . -Wall -Wextra -Ofast -MD -ffast-math -march=native 
BUILD = bin

_OBJS = Activations.o BLAS.o ConvLayers.o PoolLayers.o RoboDNN.o Utils.o UtilityLayers.o
OBJS = $(addprefix $(BUILD)/,$(_OBJS))

LTEST = layerTests.cpp
TEST = test.cpp

DEPS = Activations.h BLAS.h Layer.h ConvLayers.h PoolLayers.h RoboDNN.h UtiliyLayers.h Utils.h

LDFLAGS = -lm $(BUILD)/RoboDNN.so

$(BUILD)/%.o: %.cpp
	@mkdir -p bin
	$(CXX) -c $< $(CXXFLAGS) -o $@

lib: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(BUILD)/RoboDNN.so

tests: lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LTEST) -o $(BUILD)/layerTest
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(TEST) -o $(BUILD)/test

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
