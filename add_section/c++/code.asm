bits 32
section .text
global _start
_start:
call label
label:
pop eax
mov esi, eax
sub esi, label
add eax, txt
mov ecx, eax
sub ecx, 5
mov eax, ecx
mov edx, len
mov ebx, 1
mov eax, 4
int 0x80

mov eax, esi
add eax, offset
mov eax, [eax]
add eax, esi
jmp eax
end:

mov eax, 1
mov ebx, 0
int 0x80
section .data
txt db `hello\n`
len equ $ - txt 
offset dd 0
