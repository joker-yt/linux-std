SRC=01.cpp
EXE=$(subst .cpp,.bin,$(SRC))
CPPFLAGS=-std=c++11 -g -lpthread

UML=01-02_01.uml 01-03_01.uml
PNG=$(subst .uml,.png,$(UML))
PLANTUML=~/plantuml.jar
PLANTUMLFLAGS=-jar $(PLANTUML)
PNG_DIR=`pwd`/pics

all: $(EXE)

pics: $(PNG)

.SUFFIXES:	.cpp .bin .uml .png
.cpp.bin:
	g++ $< -o $@ $(CPPFLAGS)

.uml.png:
	mkdir -p $(PNG_DIR)
	java $(PLANTUMLFLAGS) -o $(PNG_DIR) $<

clean:
	rm -fr *.bin *.gch pics/*.png
