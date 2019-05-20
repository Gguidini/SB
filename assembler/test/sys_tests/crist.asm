debug : equ 0
;debug : equ 1

section text

;copy a, b + 2
copy b + 2, a


pz:macro
output zero
end

;div zero

input A
input B
load ZERO;batata
add A
add B

label1:
output a
label2:
label3:
label4: output b

pz
;pz zero

copy a, b
;copy a b

;copy a, b, c
;copy a, b c


if debug
output a

;a
store C
;
output C;copy A B C D E

stop

;d: space

section data

A : space
B : space 3
C : space 20
ZERO : const 0
one : const 0x1
minus_two : const -0x2