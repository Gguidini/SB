; Funções para ler e escrever inteiros (base 10)
; Aceita valores positivos e negativos
; INT MAX = 2,147,483,647
; INT MIN = -2,147,483,648
; NÃO FAZ DETECÇÃO DE OVERFLOW !!!
; TODO: Verificar se enter conta nos bytes escritos
section .data
crlf db 0xd, 0xa

section .bss
number resd 1; int 32 bits
string resb 12

section .text
global _start
_start:

    push dword number
    call readInt
    add esp, 4
    push dword number
    call putInt
    add esp, 4
    mov eax, 1
    mov ebx, 0

    int 0x80


; readInt lê um inteiro de 32 bits do teclado.
; Aceita números positivos e negativos, base 10.
; Resultado salvo no endereço indicado na pilha
; Retorna número de bytes lidos em EAX
readInt:
    ; Cria frame com 2 variáveis locais
    enter 5,0
    ; Referencia para endereço para salvar o resultado
    %define RET_ADDRS dword [ebp+8]
    ; Variáveis locais
    %define NUMBER_STREAM dword [ebp-4]
    %define SIGN byte [ebp-8]
    ; Limpa "variaveis locais"
    mov NUMBER_STREAM, 0
    mov SIGN, 0
    ; Salva registradores que serão usados pela função
    ; Com exceção de EAX
    push edx
    push ebx
    push ecx
    ; Lê do teclado
    mov eax, 3
    mov ebx, 0
    mov ecx, string
    mov edx, 12
    int 0x80
    ; Verifica se primeiro char é '-' (0x2D)
    cmp byte [string], 0x2D
    jne positive_integer
    mov ecx, 1      ; Bytes lidos
    mov SIGN, 1
    jmp initialize_variables
positive_integer:
    sub ecx, ecx    ; Bytes lidos
initialize_variables:
    sub eax, eax    ; Numero acumulado
    sub edx, edx
while:      
    ; Decode number
    mov dl, [string + ecx] ; Load bite in dl
    cmp edx, 0x0A   ; Compare with LF
    je done
    cmp edx, 0x0D   ; Compare with CR
    je done
    mov ebx, eax    ; copia eax
    shl eax, 3      ; eax * 8
    add eax, ebx    ; eax * 2 (com prox linha)
    add eax, ebx    ; total = eax * 10
    sub dl, 0x30    ; Subtrai '0'
    add eax, edx    ; eax + novo byte
    inc ecx         ; Prox byte da string
    jmp while
done:       
    ; Verifica se precisa trocar o sinal
    cmp SIGN, 0
    je save
    neg eax
save:
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
    ; Cria frame com 2 variáveis locais
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
    ; garante que o primeiro byte de string eh 0
    mov byte [string], 0
    ; verifica se o número é negativo
    cmp eax, 0
    jg is_positive
    je is_zero
    ; como é negativo, coloca '-' no início da string
    mov byte [string], 0x2D
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
    cmp byte [string], 0
    je print_loop
    inc ebx
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
