; Função para escrever strings na tela

section .data
hello db 'Hello, World!'
hello_size EQU $-hello
greeting db 'I come in peace :D'
greeting_size EQU $-greeting
crlf db 0xd, 0xa

section .text
global _start
_start:

push hello
push hello_size
call putStr
add esp, 8
push greeting
push greeting_size
call putStr
add esp, 8
mov eax, 1
mov ebx, 0
int 0x80

; Função para imprimir string na tela
; Recebe dois argumentos pela pilha:
;   1. Ponteiro para a string
;   2. Tamanho da string
putStr:
    enter 0,0
    ; Parâmetros
    %define SRC_ADDRS dword [ebp + 12]
    %define SIZE dword [ebp + 8]
    ; Salva registradores utilizados
    push ebx
    push ecx
    push edx
    ; Lê a string
    mov eax, 4
    mov ebx, 1
    mov ecx, SRC_ADDRS
    mov edx, SIZE
    int 0x80
    ; Imprime crlf
    mov eax, 4
    mov ebx, 1
    mov ecx, crlf
    mov edx, 2
    int 0x80
    ; Retorna valores dos registradores
    pop edx
    pop ecx
    pop ebx
    mov eax, SIZE
    ; Retorna
    leave
    ret