CC=clang++
LD=clang++

CCFLAGS=$(CCSTDFLAGS) $(CCWFLAGS) $(CCOFLAGS) 
CCSTDFLAGS=-std=c++11
CCWFLAGS=-Wextra
CCOFLAGS=-O3

LDFLAGS=-pthread

CCPFLAGS=-O0 -g -pg
LDPFLAGS=-g -pg

SRC=cstf.cpp\
	moves.cpp\
	coords.cpp\
	bitboards.cpp\
	colors.cpp\
	dignities.cpp\
	pieces.cpp\
	enpassants.cpp\
	castles.cpp\
	boards.cpp\
	generator.cpp

MAINSRC=ecce.cpp $(SRC)
MAINOBJ=$(MAINSRC:.cpp=.o)
PROFMAINOBJ=$(MAINSRC:.cpp=.a)

TESTSRC=test.cpp $(SRC)
TESTOBJ=$(TESTSRC:.cpp=.o)
PROFTESTOBJ=$(TESTSRC:.cpp=.a)

MAINEXE=ecce
TESTEXE=test

PROFMAINEXE=ecce_profile
PROFTESTEXE=test_profile

all: build profile
reall: clean all 

build: buildmain buildtest
buildmain: $(MAINSRC) $(MAINEXE)
buildtest: $(TESTSRC) $(TESTEXE)

rebuild: clean build 

profile: profilemain profiletest
profilemain: $(PROFMAINSRC) $(PROFMAINEXE)
profiletest: $(PROFTESTSRC) $(PROFTESTEXE)

reprofile: clean profile 

run: runtest runmain
runmain: buildmain 
	./$(MAINEXE)
runtest: buildtest 
	./$(TESTEXE)

runprofile: runprofiletest runprofilemain
runprofilemain: profilemain 
	./$(PROFMAINEXE)
runprofiletest: profiletest 
	./$(PROFTESTEXE)

clean:
	@rm *.o 2> /dev/null && echo "Ecce was cleaned" || echo "Nothing to clean" > /dev/null
	@rm *.a 2> /dev/null && echo "Profile was cleaned" || echo "Nothing to clean" > /dev/null

.cpp.o:
	@echo "[CC] $(@:.o=)"
	@$(CC) $(CCFLAGS) -c $< -o $@

$(MAINEXE): $(MAINOBJ)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) $(MAINOBJ) -o $@

$(TESTEXE): $(TESTOBJ)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) $(TESTOBJ) -o $@

.cpp.a:
	@echo "[CCP] $(@:.a=)"
	@$(CC) $(CCFLAGS) $(CCPFLAGS) -c $< -o $@

$(PROFMAINEXE): $(PROFMAINOBJ)
	@echo "[LDP] $@"
	@$(LD) $(LDFLAGS) $(LDPFLAGS) $(PROFMAINOBJ) -o $@

$(PROFTESTEXE): $(PROFTESTOBJ)
	@echo "[LDP] $@"
	@$(LD) $(LDFLAGS) $(LDPFLAGS) $(PROFTESTOBJ) -o $@
