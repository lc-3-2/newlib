    .file "crt0.S"
;; C Runtime Code
;;
;; This file defines the entrypoint for the program. It zeroes out the .bss
;; section, then it calls all the constructors, then calls `main`. Once `main`
;; terminates, we call `exit`, which calls all the destructors and halts the
;; machine.

    .section .crt0, "ax"
    .global _start
_start:

    ; Initialize the global pointer
    ; Use a symbol defined in the linker script to do this
    PSEUDO.LOADCONSTW R4, __gp

    ; Initialize the stack pointer
    ; Recall that the argument is 32-bit *signed*, so we have to flip the bits
    ; R6 = 0xf0000000
    PSEUDO.LOADCONSTW R6, ~0x0fffffff

    ; Zero-out the .bss section
    ; Assume both of these are word-aligned
    AND R0, R0, 0
    LEA R1, __bss_start
    LEA R2, __bss_end
.Lwhile_zero_bss:
    XOR R3, R1, R2
    BRz .Lendwhile_zero_bss
    STW R0, R1, 0
    ADD R1, R1, 4
    BR .Lwhile_zero_bss
.Lendwhile_zero_bss:

    ; Call `__libc_init_array` to call the constructors
    JSR __libc_init_array
    ADD R6, R6, 4

    ; Call `main`
    ; Pass argc = 0 and argv = NULL
    ; Remember the return value in R0
    AND R0, R0, 0
    ADD R6, R6, -8
    STW R0, R6, 0
    STW R0, R6, 1
    JSR main
    LDW R0, R6, 0
    ADD R6, R6, 12

    ; Call `exit`
    ADD R6, R6, -4
    STW R0, R6, 0
    JSR exit

    ; Calling `exit` should stop execution
    ; If we somehow still got here, `CRASH`
    ; If that doesn't work, spinlock
.Lspinlock:
    CRASH
    BR .Lspinlock