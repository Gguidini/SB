; Função para escrever strings na tela

section .data
crlf db 0xd, 0xa

section .bss
string resb 20

section .text
global _start
_start:

push string
push dword 20
call readStr
add esp, 8
push string
push eax
call putStr
add esp, 8
mov eax, 1
mov ebx, 0
int 0x80

; Função para ler string do teclado
; Recebe dois argumentos pela pilha:
;   1. Ponteiro para a string
;   2. Tamanho da string
readStr:
    enter 0,0
    ; Parâmetros
    %define SRC_ADDRS dword [ebp + 12]
    %define SIZE dword [ebp + 8]
    ; Salva registradores utilizados
    push ebx
    push ecx
    push edx
    ; Lê a string
    mov eax, 3
    mov ebx, 0
    mov ecx, SRC_ADDRS
    mov edx, SIZE
    int 0x80
    ; Conta bytes lidos
    sub eax, eax
    mov ebx, SRC_ADDRS
bytes_loop:
    cmp byte [ebx + eax], 0xa ; Comparando com enter
    je done_readStr
    cmp eax, SIZE  ; Vê se estourou o tamanho
    jge done_readStr
    inc eax ; Mais um byte
    jmp bytes_loop
done_readStr:
    ; Recupera valor dos registradores
    ; EAX = bytes lidos
    pop edx
    pop ecx
    pop ebx
    ; Retorna
    leave
    ret

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
    ; Imprime a string
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