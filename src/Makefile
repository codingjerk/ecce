CC=clang++
LD=clang++

CCFLAGS=$(CCSTDFLAGS) $(CCWFLAGS) $(CCOFLAGS)
CCSTDFLAGS=-std=c++11
CCWFLAGS=
CCOFLAGS=-O3

LDFLAGS=-pthread

SRC=

OBJ=$(SRC:.cpp=.o)

MAINSRC=ecce.cpp $(SRC)
MAINOBJ=$(MAINSRC:.cpp=.o)

TESTSRC=test.cpp $(SRC)
TESTOBJ=$(TESTSRC:.cpp=.o)

MAINEXE=ecce
TESTEXE=test

build: buildmain buildtest
buildmain: $(MAINSRC) $(MAINEXE)
buildtest: $(TESTSRC) $(TESTEXE)

rebuild: clean build 

run: runtest runmain
runmain: buildmain 
	./$(MAINEXE)
runtest: buildtest 
	./$(TESTEXE)

clean:
	@rm *.o 2> /dev/null && echo "Ecce was cleaned" || echo "Nothing to clean"

$(MAINEXE): $(MAINOBJ)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) $(MAINOBJ) -o $@

$(TESTEXE): $(TESTOBJ)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) $(TESTOBJ) -o $@

.cpp.o:
	@echo "[CC] $<"
	@$(CC) $(CCFLAGS) -c $< -o $@
