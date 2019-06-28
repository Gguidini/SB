; Função para escrever caracteres na tela

section .data
crlf db 0xd, 0xa
a db 'a'
zero db '0'
z db 'z'
hifen db '-'

section .text
global _start
_start:
    push dword a
    call putChar
    add esp, 4
    push dword zero
    call putChar
    add esp, 4
    push dword z
    call putChar
    add esp, 4
    push dword hifen
    call putChar
    add esp, 4
    mov eax, 1
    mov ebx, 0
    int 0x80

; Função para escrever um char na tela
; Recebe o endereço do Char na pilha
; Retorna número de caracteres escritos
putChar:
    enter 0,0
    ; Referencia para endereço do char
    %define CHAR_SRC dword [ebp+8]
    ; Salve registradores que serão usados
    push ebx
    push ecx
    push edx
    ; imprime o char
    mov eax, 4
    mov ebx, 1
    mov ecx, CHAR_SRC
    mov edx, 1
    int 0x80
    ; imprime quebra de linha
    mov eax, 4
    mov ebx, 1
    mov ecx, crlf
    mov edx, 2
    int 0x80
    ; retorna valores dos registradores
    pop edx
    pop ecx
    pop ebx
    mov eax, 1  ; Bytes escritos
    ; Retorna
    leave
    ret