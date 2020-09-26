GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
EXTERN scheduler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
EXTERN irqDispatcher

GLOBAL _syscallHandler
EXTERN sysCallDispatcher

GLOBAL _exception0Handler
GLOBAL _exception6Handler
EXTERN exceptionDispatcher

SECTION .text

%macro pushState 0
	push rax
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

%macro popState 0
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
	pop rax
%endmacro

%macro pushaq 0
    push rax      
    push rbx      
    push rcx      
    push rdx      
    push rbp      
    push rdi      
    push rsi      
    push r8       
    push r9       
    push r10      
    push r12     
    push r11     
    push r13     
    push r14     
    push r15     
    push fs
    push gs
%endmacro

%macro popaq 0
    pop gs
    pop fs
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
    pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp
	call exceptionDispatcher

    popState
	iretq
%endmacro

_syscallHandler:
	push rdi
	mov [sysRegisters+8*0],rax
	mov [sysRegisters+8*1],rdi
	mov [sysRegisters+8*2],rsi
	mov [sysRegisters+8*3],rdx
	mov [sysRegisters+8*4],r10
	mov [sysRegisters+8*5],r8
	mov [sysRegisters+8*6],r9

	mov rdi,sysRegisters
	call sysCallDispatcher

	pop rdi

	iretq

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

;8254 Timer (Timer Tick)
_irq00Handler:
	pushaq
 
	mov rdi, 0 ; pasaje de parametro
	mov rsi, rsp ; pasaje de parametro
	call irqDispatcher

	mov rdi,rsp
	call scheduler
	mov rsp,rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al
	
	popaq
	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1

	sysRegisters:
		rsrax resq 1
		rsrdi resq 1
		rsrsi resq 1
		rsrdx resq 1
		rsr10 resq 1
		rsr8 resq 1
		rsr9 resq 1

	