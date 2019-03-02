CXX=g++
CXXFLAGS=-I . -Wall -Wextra -march=native -Ofast -MD -ffast-math
CXXFLAGST=-I . -Wall -Wextra -march=native -Ofast -MD -std=c++11 -ffast-math

BUILD = bin

_OBJS = Activations.o BLAS.o ConvLayers.o PoolLayers.o RoboDNN.o Utils.o UtilityLayers.o
OBJS = $(addprefix $(BUILD)/,$(_OBJS))

LTEST = layerTests.cpp
TEST = test.cpp

DEPS = Activations.h BLAS.h Layer.h ConvLayers.h PoolLayers.h RoboDNN.h UtiliyLayers.h Utils.h

LDFLAGS = -lm $(BUILD)/RoboDNN.so
LDFLAGST = -lm -lopencv_core -lopencv_imgcodecs -lboost_filesystem -lboost_system $(BUILD)/RoboDNN.so

$(BUILD)/%.o: %.cpp
	@mkdir -p bin
	$(CXX) -c $< $(CXXFLAGS) -o $@

lib: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $^ -o $(BUILD)/RoboDNN.so

tests: lib
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LTEST) -o $(BUILD)/layerTest
	$(CXX) $(CXXFLAGST) $(LDFLAGST) $(TEST) -o $(BUILD)/test

clean:
	rm -rf $(BUILD)

-include $(OBJS:.o=.d)
