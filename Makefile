heat_dissipation_asm.o: heat_dissipation.asm
	nasm -f elf64 -w+all -w+error -o heat_dissipation_asm.o heat_dissipation.asm

heat_dissipation.o: heat_dissipation.c
	gcc -c -Wall -Wextra -O2 -std=c11 -o heat_dissipation.o heat_dissipation.c

heat_dissipation: heat_dissipation.o heat_dissipation_asm.o
	gcc heat_dissipation_asm.o heat_dissipation.o -o heat_dissipation

clean:
	rm -f heat_dissipation.o heat_dissipation_asm.o heat_dissipation

.DEFAULT_GOAL := all

all: heat_dissipation
