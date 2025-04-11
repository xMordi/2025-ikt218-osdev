[bits 32]
global isr33

extern isr33_handler

isr33:
    cli
    pushad
    call isr33_handler
    popad
    sti
    iret
