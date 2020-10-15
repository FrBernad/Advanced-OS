GLOBAL sys_getMem
GLOBAL sys_RTCTime
GLOBAL sys_temp
GLOBAL sys_write
GLOBAL sys_getchar
GLOBAL sys_clear
GLOBAL sys_loadApp
GLOBAL sys_inforeg
GLOBAL sys_ps
GLOBAL sys_secsElapsed
GLOBAL sys_kill
GLOBAL sys_nice
GLOBAL sys_block
GLOBAL sys_unblock
GLOBAL sys_getPID
GLOBAL sys_malloc
GLOBAL sys_free
GLOBAL sys_yield
GLOBAL sys_sem_open
GLOBAL sys_sem_close
GLOBAL sys_sem_post
GLOBAL sys_sem_wait
GLOBAL sys_dumpSemaphores
GLOBAL sys_popen
GLOBAL sys_closePipe
GLOBAL sys_writePipe
GLOBAL sys_readPipe
GLOBAL sys_dumpPipes
GLOBAL sys_wait
GLOBAL sys_ticksElapsed
GLOBAL sys_dumpMM

GLOBAL cpuVendor
GLOBAL cpuModel

section .text

%macro pushStateNoRax 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popStateNoRax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro syscall 1
    push rbp
    mov rbp, rsp
	pushStateNoRax

    mov rax,%1
    mov r10,rcx
    int 80h

	popStateNoRax
	mov rsp, rbp
    pop rbp

    ret
%endmacro

sys_getMem:
	syscall 0

sys_RTCTime:
	syscall 1

sys_temp:
	syscall 2

sys_write:
	syscall 3

sys_getchar:
	syscall 4

sys_clear:
	syscall 5

sys_loadApp:
	syscall 6

sys_inforeg:
	syscall 7

sys_ps:
	syscall 8

sys_secsElapsed:
	syscall 9

sys_kill:
	syscall 10

sys_nice:
	syscall 11

sys_block:
	syscall 12

sys_unblock:
	syscall 13

sys_getPID:
	syscall 14

sys_malloc:
	syscall 15

sys_free:
	syscall 16

sys_yield: 
	syscall 17
	
sys_sem_open:
	syscall 18

sys_sem_close:
	syscall 19

sys_sem_post:
	syscall 20

sys_sem_wait:
	syscall 21

sys_dumpSemaphores:
	syscall 22

sys_popen:
	syscall 23

sys_closePipe:
	syscall 24

sys_writePipe:
	syscall 25

sys_readPipe:
	syscall 26

sys_dumpPipes:
	syscall 27

sys_wait:
	syscall 28

sys_ticksElapsed:
	syscall 29

sys_dumpMM:
	syscall 30

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx
	push rcx
	push rdx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+12], 0

	mov rax,rdi

	pop rdx
	pop rcx
	pop rbx

	mov rsp, rbp
	pop rbp
	ret

cpuModel:
	push rbp
	mov rbp, rsp

	mov rax,0
	mov rax,1
	cpuid

	shr rax,4
	and rax,0xF ;0x000000000000000F
	
	mov rsp, rbp
	pop rbp
	ret
