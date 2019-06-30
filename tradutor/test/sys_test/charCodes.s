section .text
global _start
_start:
push CHAR
call readChar
add esp, 4
mov eax, [CHAR]
push CHAR
call putInt
add esp, 4
push CHAR
call putHex
add esp, 4
mov eax, 1
mov ebx, 0
int 0x80
section .data
__string dd 0,0,0
crlf db 0xd, 0xa
__hex_table db "0123456789ABCDEF"
CHAR dd 0

section .text

readInt:
enter 5,0
%define RET_ADDRS dword [ebp+8]
%define NUMBER_STREAM dword [ebp-4]
%define SIGN byte [ebp-8]
mov NUMBER_STREAM, 0
mov SIGN, 0
push edx
push ebx
push ecx
mov eax, 3
mov ebx, 0
mov ecx, __string
mov edx, 12
int 0x80
cmp byte [__string], 0x2D
jne input_positive_integer
mov ecx, 1
mov SIGN, 1
jmp input_initialize_variables
input_positive_integer:
sub ecx, ecx
input_initialize_variables:
sub eax, eax
sub edx, edx
input_while:
mov dl, [__string + ecx]
cmp edx, 0x0A
je input_done
cmp edx, 0x0D
je input_done
mov ebx, eax
shl eax, 3
add eax, ebx
add eax, ebx
sub dl, 0x30
add eax, edx
inc ecx
jmp input_while
input_done:
cmp SIGN, 0
je input_save
neg eax
input_save:
mov edx, RET_ADDRS
mov [edx], eax
mov eax, ecx
pop ecx
pop ebx
pop edx
leave
ret

putInt:
enter 0,0
%define NUMBER_SRC dword [ebp+8]
push esi
push ecx
push edx
push ebx
mov esi, NUMBER_SRC
mov eax, [esi]
sub ecx, ecx
mov ebx, 10
mov byte [__string], 0
cmp eax, 0
jg output_is_positive
je output_is_zero
mov byte [__string], 0x2D
inc ecx
neg eax
jmp output_is_positive
output_is_zero:
push byte 0x30
inc ecx
jmp output_print
output_is_positive:
cmp eax, 0
je output_print
sub edx, edx
div ebx
add edx, 0x30
push dx
inc ecx
jmp output_is_positive
output_print:
sub ebx, ebx
cmp byte [__string], 0
je output_print_loop
inc ebx
output_print_loop:
cmp ebx, ecx
je output_ok_to_print
pop dx
mov byte [__string + ebx], dl
inc ebx
jmp output_print_loop
output_ok_to_print:
push ecx
mov eax, 4
mov ebx, 1
mov edx, ecx
mov ecx, __string
int 0x80
mov eax, 4
mov ebx, 1
mov ecx, crlf
mov edx, 2
int 0x80
pop eax
pop ebx
pop edx
pop ecx
pop esi
leave
ret

readHex:
enter 0,0
%define RET_ADDRS dword [ebp+8]
push ebx
push ecx
push edx
mov eax, 3
mov ebx, 0
mov ecx, __string
mov edx, 8
int 0x80
sub eax, eax
sub ecx, ecx
sub edx, edx
decode_hex:
mov dl, [__string + ecx]
cmp edx, 0x0A
je hinput_done
cmp ecx, 8
je hinput_done
cmp edx, 0x39
jbe is_hex_digit
cmp edx, 0x5A
jbe is_hex_uppercase
sub dl, 0x61
add dl, 10
jmp hinput_loop_prep
is_hex_uppercase:
sub dl, 0x41
add dl, 10
jmp hinput_loop_prep
is_hex_digit:
sub dl, 0x30
hinput_loop_prep:
shl eax, 4
add eax, edx
inc ecx
jmp decode_hex
hinput_done:
mov edx, RET_ADDRS
mov [edx], eax
mov eax, ecx
pop ecx
pop ebx
pop edx
leave
ret

putHex:
enter 0,0
%define NUMBER_SRC dword [ebp+8]
push esi
push ecx
push edx
push ebx
mov esi, NUMBER_SRC
mov edx, [esi]
sub ecx, ecx
mov ebx, 16
cmp edx, 0
jne houtput_not_zero
push byte 0x30
inc ecx
jmp houtput_print
houtput_not_zero:
cmp edx, 0
je houtput_print
sub eax, eax
mov al, dl
and al, 0x0F
mov ebx, __hex_table
xlatb
push ax
inc ecx
shr edx, 4
jmp houtput_not_zero
houtput_print:
sub ebx, ebx
houtput_print_loop:
cmp ebx, ecx
je houtput_ok_to_print
pop dx
mov byte [__string + ebx], dl
inc ebx
jmp houtput_print_loop
houtput_ok_to_print:
push ecx
mov eax, 4
mov ebx, 1
mov edx, ecx
mov ecx, __string
int 0x80
mov eax, 4
mov ebx, 1
mov ecx, crlf
mov edx, 2
int 0x80
pop eax
pop ebx
pop edx
pop ecx
pop esi
leave
ret

readChar:
enter 0,0
%define CHAR_SRC dword [ebp+8]
push ebx
push ecx
push edx
mov eax, 3
mov ebx, 0
mov ecx, CHAR_SRC
mov edx, 1
int 0x80
pop edx
pop ecx
pop ebx
mov eax, 1
leave
ret

putChar:
enter 0,0
%define CHAR_SRC dword [ebp+8]
push ebx
push ecx
push edx
mov eax, 4
mov ebx, 1
mov ecx, CHAR_SRC
mov edx, 1
int 0x80
mov eax, 4
mov ebx, 1
mov ecx, crlf
mov edx, 2
int 0x80
pop edx
pop ecx
pop ebx
mov eax, 1
leave
ret

readStr:
enter 0,0
%define SRC_ADDRS dword [ebp + 12]
%define SIZE dword [ebp + 8]
push ebx
push ecx
push edx
mov eax, 3
mov ebx, 0
mov ecx, SRC_ADDRS
mov edx, SIZE
int 0x80
sub eax, eax
mov ebx, SRC_ADDRS
sinput_bytes_loop:
cmp byte [ebx + eax], 0xa
je done_readStr
cmp eax, SIZE
jge done_readStr
inc eax
jmp sinput_bytes_loop
done_readStr:
pop edx
pop ecx
pop ebx
leave
ret

putStr:
enter 0,0
%define SRC_ADDRS dword [ebp + 12]
%define SIZE dword [ebp + 8]
push ebx
push ecx
push edx
mov eax, 4
mov ebx, 1
mov ecx, SRC_ADDRS
mov edx, SIZE
int 0x80
mov eax, 4
mov ebx, 1
mov ecx, crlf
mov edx, 2
int 0x80
pop edx
pop ecx
pop ebx
mov eax, SIZE
add eax, 2
leave
ret
