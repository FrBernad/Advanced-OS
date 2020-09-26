include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_IDT=$(wildcard IDT/*.c)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)

LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=
#NO SSE EN MAKEFILE.INC DESACTIVA PUTNOS FLOTANTES
all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_IDT) $(OBJECTS_DRIVERS) $(OBJECTS_LIBRARIES)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin libraries/*.o IDT/*.o drivers/*.o

.PHONY: all clean
