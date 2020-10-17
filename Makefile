MM=FREE_LIST_MM

all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all MM=-D$(MM)

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

SOURCES_CPP=$(shell find . -type f -name '*.c')
OUT_CPP=$(SOURCES_CPP:.c=.cpp)

cpp: $(OUT_CPP)

pvs: clean
	pvs-studio-analyzer trace -- make  > /dev/null
	pvs-studio-analyzer analyze > /dev/null 2> /dev/null
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o report.tasks PVS-Studio.log  > /dev/null

%.cpp: %.c
	cppcheck --quiet --enable=all --force --inconclusive $< 2>> output.cppOut

cleanTest:
	rm output.cppOut report.tasks PVS-Studio.log

.PHONY: bootloader image collections kernel userland all clean cleanTest cpp
