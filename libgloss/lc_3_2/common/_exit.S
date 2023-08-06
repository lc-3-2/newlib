    .file "_exit.S"
;; Halt the machine
;;
;; For debugging, we also put the exit code in R0.

    .section .text._exit, "ax"
    .global _exit
_exit:
    LDW R0, R6, 0
    HALT