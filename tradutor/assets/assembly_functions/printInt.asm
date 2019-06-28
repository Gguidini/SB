; Função para escrever inteiros na tela
; Aceita números positivos e negativos
; TODO: Verificar se enter conta nos bites escritos

section .data
zero dd 0
one dd 1
ten dd 10
negative dd -1234
positive dd 5566
max dd 2147483647
min dd -2147483648
crlf db 0xd, 0xa

section .bss
__string resb 11
section .text
global _start
_start:
    push dword zero
    call putInt
    add esp, 4
    push dword one
    call putInt
    add esp, 4
    push dword negative
    call putInt
    add esp, 4
    push dword positive
    call putInt
    add esp, 4
    push dword max
    call putInt
    add esp, 4
    push dword min
    call putInt
    add esp, 4
    mov eax, 1
    mov ebx, 0
    int 0x80
; Função para escrever um inteiro na tela
; Aceita números positivos e negativos (base 10)
; Endereço do número passado na pilha
; Retorna número de bytes lidos em EAX 
putInt:
    enter 0,0
    ; Referencia para endereço do número
    %define NUMBER_SRC dword [ebp+8]
    ; salva registradores
    push esi
    push ecx
    push edx
    push ebx
    mov esi, NUMBER_SRC
    mov eax, [esi]
    sub ecx, ecx
    mov ebx, 10
    ; garante que o primeiro byte de __string eh 0
    mov byte [__string], 0
    ; verifica se o número é negativo
    cmp eax, 0
    jg is_positive
    je is_zero
    ; como é negativo, coloca '-' no início da __string
    mov byte [__string], 0x2D
    inc ecx
    neg eax
    jmp is_positive
is_zero:
    push byte 0x30
    inc ecx
    jmp print
is_positive:
    cmp eax, 0
    je print
    sub edx, edx
    ; EAX = Numero
    ; EBX = 10
    ; EDX.EAX / EBX => EAX; EDX.EAX % EBX => EDX
    div ebx
    add edx, 0x30 ; (char) EDX
    push dx       ; because chars only have 1 byte
    inc ecx       ; another byte used
    jmp is_positive
print:
    sub ebx, ebx
    cmp byte [__string], 0
    je print_loop
    inc ebx
print_loop:
    cmp ebx, ecx
    je ok_to_print
    pop dx
    mov byte [__string + ebx
    ], dl
    inc ebx
    jmp print_loop
ok_to_print:
    push ecx    ; Salva bytes usados
    ; imprime o numero
    mov eax, 4
    mov ebx, 1
    mov edx, ecx
    mov ecx, __string
    int 0x80
    ; imprime quebra de linha
    mov eax, 4
    mov ebx, 1
    mov ecx, crlf
    mov edx, 2
    int 0x80
    pop eax     ; Bytes usados, para retorno da função
    ; Retorna valores dos registradores
    pop ebx
    pop edx
    pop ecx
    pop esi
    leave
    ret



