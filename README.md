# README #

Ecce - is a chess engine, created as work-on-errors of previously engine - ExCE. It will be more fast, strong and less experimental, uses best current chess acknowledgments.

## Philosophy ##

* Aggressive optimization.
* Ignoring flexible and clear code.
* Focus on single architecture and rules.

## Compiling ##

On linux systems install g++ and just run 
```
#!Makefile

make
```
 or 
```
#!Makefile

make build
```
 in src directory.

## Nonstandard UCI commands ##

* board - shows a psevdo-graphical board for current position.
* fen - shows a feh for current position.
* test [level] - runs a tests suite for check internal rules.
* perft <depth> - runs a performance test for current position at custom depth.
* speed - runs a tests suite for check engine speed.
