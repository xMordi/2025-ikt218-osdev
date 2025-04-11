[BITS 32]

global isr32
extern isr32_handler
isr32:
    pusha
    call isr32_handler
    popa
    iret

global isr33
extern isr33_handler
isr33:
    pusha
    call isr33_handler
    popa
    iret

global isr34
extern isr34_handler
isr34:
    pusha
    call isr34_handler
    popa
    iret
