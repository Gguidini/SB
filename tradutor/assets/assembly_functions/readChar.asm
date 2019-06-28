; Função para ler char

section .data
crlf db 0xd, 0xa

section .bss
char resb 1

section .text
global _start
_start:

push dword char
call readChar
add esp, 4
push dword char
call putChar
add esp, 4
mov eax, 1
mov ebx, 0
int 0x80

; Lê um char do teclado
; Recebe endereço para salvar na pilha
readChar:
    enter 0,0
    ; Referencia para endereço do char
    %define CHAR_SRC dword [ebp+8]
    ; Salva registradores que serão usados
    push ebx
    push ecx
    push edx
    ; Lê o char
    mov eax, 3
    mov ebx, 0
    mov ecx, CHAR_SRC
    mov edx, 1
    int 0x80
    ; Recupera valores dos registradores
    pop edx
    pop ecx
    pop ebx
    mov eax, 1  ; Bytes lidos
    ; Retorna
    leave
    ret

; Função para escrever um char na tela
; Recebe o endereço do Char na pilha
; Retorna número de caracteres escritos
putChar:
    enter 0,0
    ; Referencia para endereço do char
    %define CHAR_SRC dword [ebp+8]
    ; Salva registradores que serão usados
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