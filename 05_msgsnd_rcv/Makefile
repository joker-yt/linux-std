SRC=01.cpp 02.cpp 03.cpp 04.cpp
EXE=$(subst .cpp,.bin,$(SRC))
CPPFLAGS=-std=c++11 -MMD -g

all: $(EXE)
-include $(DEPS)

.SUFFIXES:	.cpp .bin
.cpp.bin:
	g++ $< -o $@ $(CPPFLAGS)

clean:
	rm -fr *.bin *.gch *.d
