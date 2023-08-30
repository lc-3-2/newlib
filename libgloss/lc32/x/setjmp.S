    .file "setjmp.S"
; Stores the current environment in the address pointed to by the first
; parameter. It stores R4, R5, R6, and R7. The first three are saved because
; callees shouldn't corrupt them, and R7 is saved so we know where to return to
; with `longjmp`.
;
; This function does *not* save the signal mask.
;
; See: newlib/libc/include/machine/setjmp.h

    .section .text.setjmp, "ax"
    .global setjmp
setjmp:

    ; Load the address of the jump buffer
    LDW R0, R6, 0

    ; Move the stack pointer up for the return value
    ; This is the stack pointer that will be saved
    ADD R6, R6, -4

    ; Store the registers in the buffer
    STW R4, R0, 0
    STW R5, R0, 1
    STW R6, R0, 2
    STW R7, R0, 3

    ; Return 0
    AND R1, R1, 0
    STW R1, R6, 0
    RET
