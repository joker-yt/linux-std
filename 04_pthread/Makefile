SRC=01.cpp
EXE=$(subst .cpp,.bin,$(SRC))
CPPFLAGS=-std=c++11 -g -lpthread

all: $(EXE)

.SUFFIXES:	.cpp .bin
.cpp.bin:
	g++ $< -o $@ $(CPPFLAGS)

clean:
	rm -fr *.bin *.gch
