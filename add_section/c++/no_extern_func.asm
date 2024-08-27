bits 32
section .text
global _start
_start:
mov edx, len
mov ecx, txt
mov ebx, 1
mov eax, 4
int 0x80

mov eax, 1
mov ebx, 0
int 0x80
section .data
txt db `hello\n`
len equ $ - txt 
