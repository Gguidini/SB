; Função para escrever numeros hexadecimais na tela
; Aceita números positivos

section .data
zero dd 0
one dd 1
ten dd 0xA
positive dd 0xBABA0000
max dd 0xFFFFFFFF
crlf db 0xd, 0xa
__hex_table db "0123456789ABCDEF"

section .bss
string resb 12

section .text
global _start
_start:
    push dword zero
    call putHex
    add esp, 4
    push dword one
    call putHex
    add esp, 4
    push dword ten
    call putHex
    add esp, 4
    push dword positive
    call putHex
    add esp, 4
    push dword max
    call putHex
    add esp, 4
    mov eax, 1
    mov ebx, 0
    int 0x80

; Função para escrever um hexadecimal na tela
; Aceita números positivos (base 16)
; Endereço do número passado na pilha
; Retorna número de bytes escritos em EAX 
putHex:
    enter 0,0
    ; Referencia para endereço do número
    %define NUMBER_SRC dword [ebp+8]
    ; salva registradores
    push esi
    push ecx
    push edx
    push ebx
    mov esi, NUMBER_SRC
    mov edx, [esi]
    sub ecx, ecx
    mov ebx, 16
    ; verifica se o número é zero
    cmp edx, 0
    jne not_zero
    ; Caso seja 0
    push byte 0x30
    inc ecx
    jmp print
not_zero:
    cmp edx, 0
    je print
    sub eax, eax    ; Limpa EAX
    mov al, dl      ; Prox char hexa
    and al, 0x0F    ; Pega so o ultimo nible
    mov ebx, __hex_table    ; Carrega tabela de tradução
    xlatb
    push ax       ; add byte na string
    inc ecx       ; another byte used
    shr edx, 4    ; Proximo char
    jmp not_zero
print:
    sub ebx, ebx
print_loop:
    cmp ebx, ecx
    je ok_to_print
    pop dx
    mov byte [string + ebx], dl
    inc ebx
    jmp print_loop
ok_to_print:
    push ecx    ; Salva bytes usados
    ; imprime o numero
    mov eax, 4
    mov ebx, 1
    mov edx, ecx
    mov ecx, string
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



