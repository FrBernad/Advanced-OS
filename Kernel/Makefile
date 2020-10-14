include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_IDT=$(wildcard Interrupts/*.c)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)
SOURCES_RESOURCES=$(wildcard resources/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)
OBJECTS_RESOURCES=$(SOURCES_RESOURCES:.c=.o)

LOADERSRC=loader.asm

MM=-D FREE_LIST_MM

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=
#NO SSE EN MAKEFILE.INC DESACTIVA PUTNOS FLOTANTES
all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_IDT) $(OBJECTS_DRIVERS) $(OBJECTS_LIBRARIES) $(OBJECTS_RESOURCES)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include $(MM) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin libraries/*.o Interrupts/*.o drivers/*.o resources/*.o

.PHONY: all clean
