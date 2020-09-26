GLOBAL cpuVendor
GLOBAL cpuModel
GLOBAL cpuTemp
GLOBAL sys_getMem

GLOBAL updateSnapshot
GLOBAL getSnapshot

GLOBAL getBSDTimeInfo

GLOBAL callTimerTick

EXTERN scheduler

section .text

%macro pushaq 0
    push rax      ;save current rax
    push rbx      ;save current rbx
    push rcx      ;save current rcx
    push rdx      ;save current rdx
    push rbp      ;save current rbp
    push rdi      ;save current rdi
    push rsi      ;save current rsi
    push r8       ;save current r8
    push r9       ;save current r9
    push r10      ;save current r10
    push r11      ;save current r11
    push r12      ;save current r12
    push r13      ;save current r13
    push r14      ;save current r14
    push r15      ;save current r15
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
	
cpuTemp:
	push rbp
	mov rbp, rsp

	push rcx
	push rdi

	mov rax,0
	mov rcx,0
	mov ecx, 0x19C ;codigo que corresponde a THERMAL STATUS
	;rdmsr          ;me deja en eax la parte baja del msr solicitado, que en este caso es la unica que me interesa
	mov rax,0x88310000
	shr rax,16                          ;shifteo que usaria para quedarme con los bits que me importan
	and rax,0x7F ;0x00000000000007F     ;set in rax last 7 bits digital readout (22:16)
	mov rdi,rax							;backup digital readout
 
 	mov rcx,0
	mov rax,0
	mov ecx,0x1A2
	;rdmsr                             ;access DTS_THERMAL_PROFILE msr
	mov rax,0x690A00
	shr rax,16                         ;shifteo que usaria para quedarme con los bits que me importan
	and rax,0xFF ;0x00000000000000FF    ;set in rax last 8 bits of TCC ACTIVATION TEMP (23:16)

	sub rax,rdi

	pop rdi
	pop rcx
							
	mov rsp, rbp
	pop rbp
	ret             ;se devuelve en eax el valor de la temp en C

sys_getMem:
	push rbp
	mov rbp, rsp

	push rdx
	push r10

	mov rax,rsi

	mov rdx,0
.loopStart:
	cmp rdx,32
	jle .loopEnd
	dec rdx
	mov r10,[rdi]
	mov [rsi],r10
	inc rdi
	inc rsi
	jmp .loopStart
.loopEnd:
	pop r10
	pop rdx

	mov rsp, rbp
	pop rbp
	ret

getBSDTimeInfo:
    push rbp
    mov rbp,rsp

    mov rax, rdi
    out 70h, al
    mov rax,0
    in al, 71h

    leave
    ret

callTimerTick:
	int 20h
	ret