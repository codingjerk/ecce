CC=g++
LD=g++

CCFLAGS=$(CCSTDFLAGS) $(CCWFLAGS) $(CCOFLAGS) 
CCSTDFLAGS=-std=c++11
CCWFLAGS=-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
CCOFLAGS=-Ofast $(CCARCHFLAGS)
CCARCHFLAGS=-msse -msse2 -msse3 -mfpmath=sse -mtune=native -march=native

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
	generator.cpp\
	magics.cpp\
	generatorTables.cpp\
	mover.cpp\
	checker.cpp\
	uci.cpp\
	tests.cpp\
	PST.cpp\
	score.cpp\
	zobrist.cpp\
	captures.cpp\
	history.cpp\
	hash.cpp\
	recaptures.cpp\
	silents.cpp\
	uselesses.cpp\

MAINSRC=ecce.cpp $(SRC)
MAINOBJ=$(MAINSRC:.cpp=.o)
PROFMAINOBJ=$(MAINSRC:.cpp=.a)

MAINEXE=ecce

PROFMAINEXE=ecce_profile

all: build profile
reall: clean all 

build: $(MAINSRC) $(MAINEXE)

rebuild: clean build 

profile: $(PROFMAINSRC) $(PROFMAINEXE)

reprofile: clean profile 

run: build 
	./$(MAINEXE)

runprofile: profile 
	./$(PROFMAINEXE)

clean:
	@rm *.o 2> /dev/null && echo "Ecce was cleaned" || echo "Nothing to clean" > /dev/null
	@rm *.a 2> /dev/null && echo "Profile was cleaned" || echo "Nothing to clean" > /dev/null

.cpp.o:
	@echo "[CC] $(@:.o=)"
	@$(CC) $(CCFLAGS) -c $< -o $@

$(MAINEXE): $(MAINOBJ)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) $(MAINOBJ) -o $@

.cpp.a:
	@echo "[CCP] $(@:.a=)"
	@$(CC) $(CCFLAGS) $(CCPFLAGS) -c $< -o $@

$(PROFMAINEXE): $(PROFMAINOBJ)
	@echo "[LDP] $@"
	@$(LD) $(LDFLAGS) $(LDPFLAGS) $(PROFMAINOBJ) -o $@
