    .file "_exit.S"
;; Enter an infinite loop

    .section .text._exit, "ax"
    .global _exit
_exit:
.L_exit:
    BR .L_exit
