CXX = mex # C compiler
RM = rm -f  # rm command
TARGET = ../bin/MatchingPersuitFull3 # target

SRCS = $(wildcard *.cpp)

INCLUDE_PATH = ../utilitydll/ ../third_party/mex/
LIB_PATH = ../bin/
LIBS = utility

CXXFLAGS += $(foreach dir, $(INCLUDE_PATH), -I$(dir))
CXXFLAGS += CXXFLAGS="\$$CXXFLAGS -fopenmp"
CXXFLAGS += -cxx

LDFLAGS += -Wl,-rpath,./ 
LDFLAGS += $(foreach dir, $(LIB_PATH), -L$(dir))
LDFLAGS += $(foreach lib, $(LIBS), -l$(lib))
LDFLAGS += LDFLAGS="\$$LDFLAGS -fopenmp"
LDFLAGS += -cxx

OBJS = $(SRCS:.cpp=.o)
	
.PHONY: all
all: $(TARGET)
	
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)
 
 $(OBJS) : %.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
	 
.PHONY: clean
clean:
	${RM} ${TARGET} ${OBJS} $(SRCS:.cpp=.d)
