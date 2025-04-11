[BITS 32]

global isr33

isr33:
    pusha                  ; Push all general-purpose registers
    call isr33_handler     ; Call C handler
    popa                   ; Restore registers
    iret                   ; Return from interrupt
