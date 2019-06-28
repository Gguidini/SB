; Função para ler hexadecimal do teclado
; Aceita apenas numeros positivos

section .data
crlf db 0xd, 0xa
__hex_table db "0123456789ABCDEF"

section .bss
number resd 1; int 32 bits
__string resb 12

section .text
global _start
_start:

    push dword number
    call readHex
    add esp, 4
    push dword number
    call putInt
    add esp, 4
    mov eax, 1
    mov ebx, 0
    int 0x80

; readHex lê um inteiro no teclaro
; Recebe como argumento o endereço para salvá-lo
readHex:
    enter 0,0
    ; Variáveis
    %define RET_ADDRS dword [ebp+8]
    ; Guarda valores de registradores que serão usados
    push ebx
    push ecx
    push edx
    ; Lê do teclado para __string intermediaria
    mov eax, 3
    mov ebx, 0
    mov ecx, __string
    mov edx, 8
    int 0x80
    ; Inicializa variáveis para decodificar numeros
    sub eax, eax
    sub ecx, ecx
    sub edx, edx
decode_hex:
    mov dl, [__string + ecx] ; Load bite in dl
    cmp edx, 0x0A   ; Compare with LF
    je done
    cmp ecx, 8      ; Max characteres hexa
    je done
    cmp edx, 0x39   ; Compara com '9'
    jbe is_hex_digit
    cmp edx, 0x5A   ; Compara com 'Z'
    jbe is_hex_uppercase
    ; Lowercase
    sub dl, 0x61    ; Subtrai 'a'
    add dl, 10      ; Soma 10 (valor de 'a')
    jmp loop_prep
is_hex_uppercase:
    sub dl, 0x41    ; Subtrai 'A'
    add dl, 10      ; Soma 10 (valor de 'A')
    jmp loop_prep
is_hex_digit:
    sub dl, 0x30    ; Subtrai '0'
loop_prep:
    shl eax, 4      ; eax * 16
    add eax, edx    ; eax + novo digito
    inc ecx
    jmp decode_hex
done:
    ; Salva numero no endereço argumento
    mov edx, RET_ADDRS
    mov [edx], eax
    ; Salva quantos bytes foram lidos
    mov eax, ecx
    ; Recupera registradores
    pop ecx
    pop ebx
    pop edx
    ; Retorna
    leave
    ret

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
    mov byte [__string + ebx], dl
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

