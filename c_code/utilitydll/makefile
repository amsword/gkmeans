CXX = g++ # C compiler
CXXFLAGS = -fPIC # C flags
LDFLAGS = -shared
RM = rm -f  # rm command
TARGET_PATH = ../bin
TARGET_LIB = $(TARGET_PATH)/libutility.so # target lib

SRCS = $(wildcard *.cpp)
# SRCS = utility_logger.cpp utility_global.cpp utility_assert.cpp   BinaryCodePartition.cpp utility_math_basic.cpp VirtualData.cpp DistanceCalculator.cpp CKMeansDistance.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all
all: $(TARGET_LIB)
 
$(TARGET_LIB): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
 
$(SRCS:.cpp=.d):%.d:%.cpp
	$(CXX) -c -MM $< >$@

# $(SRCS:.cpp=.d):%.d:%.cpp
	# $(CC) $(CFLAGS) -MM $< -MF $@ -o $(patsubst %.o,%.d,$@)   
# $(OBJS) : %.o : %.cpp
	# $(CC) -c -fPIC $< -o $@
include $(SRCS:.cpp=.d)

 
.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.cpp=.d)
