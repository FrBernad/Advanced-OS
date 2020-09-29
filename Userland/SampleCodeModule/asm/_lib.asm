GLOBAL syscall
GLOBAL sys_changeApp
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
syscall:
    push rbp
    mov rbp, rsp
	pushStateNoRax

    mov rax,rdi
    mov rdi, rsi
    mov rsi,rdx
    mov rdx,rcx
    mov r10,r8
    mov r8,r9
	mov r9,[rbp+16]
    int 80h

	popStateNoRax
	mov rsp, rbp
    pop rbp

    ret

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
