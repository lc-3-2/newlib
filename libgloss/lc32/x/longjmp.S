    .file "longjmp.S"
; Non-local goto using the information saved by `setjmp`.
;
; See: setjmp.S

    .section .text.longjmp, "ax"
    .global longjmp
longjmp:

    ; Load the jump buffer address and the return value into registers
    ; These will not be available after R6 is restored
    LDW R0, R6, 0
    LDW R1, R6, 1

    ; Correct the return value
    ; If it is zero, increment it
    BRnp .Lendif
    ADD R1, R1, 1
.Lendif:

    ; Restore the registers from the buffer
    LDW R4, R0, 0
    LDW R5, R0, 1
    LDW R6, R0, 2
    LDW R7, R0, 3

    ; Return the corrected value
    STW R1, R6, 0
    RET
