SRC=01.cpp 02.cpp 03.cpp
OBJS=$(subst .cpp,.o,$(SRC))
EXE=$(subst .cpp,.bin,$(SRC))
CPPFLAGS=-std=c++11 -MMD -g
LDFLAGS=-lrt
## If you need PNG files by plantuml
# UML=01-02_01.uml 01-03_01.uml
# PNG=$(subst .uml,.png,$(UML))
# PLANTUML=~/plantuml.jar
# PLANTUMLFLAGS=-jar $(PLANTUML)
# PNG_DIR=`pwd`/pics

CXX=g++

# Phony is the rule that doesn't have any "target".
.PHONY: all clean check-syntax
all: $(EXE)
-include $(DEPS)

# Suffixes is needed for the rule of exchanging extentions
.SUFFIXES:	.cpp .bin
.cpp.bin:
	g++ $< -o $@ $(CPPFLAGS) $(LDFLAGS)

# .uml.png:
# 	mkdir -p $(PNG_DIR)
# 	java $(PLANTUMLFLAGS) -o $(PNG_DIR) $<

clean:
	rm -fr *.o *.gch *.d *.bin *.png test_dir/*

check-syntax:
	$(CXX) -o null -fsyntax-only $(CHK_SOURCES) $(INCLUDES) $(CPPFLAGS)
