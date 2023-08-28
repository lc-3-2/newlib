    .file "_exit.S"
;; Halt the machine

    .section .text._exit, "ax"
    .global _exit
_exit:
    HALT