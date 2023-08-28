###
# Register the linker scripts
#
# We have one linker script for each "backend". So to select between them, we
# link the right library and choose the right linker script.
#
# As for what this target does, it seems to install the linker scripts.

multilibtool_DATA += \
	%D%/lc32Trap.ld

###
# Barebones backend
#
# This backend assumes nothing about the machine, not even that TRAPs have their
# usual meaning. As such, it functions very closely to a bare-metal environment.
#
# There is no corresponding linker script for this backend. It doesn't make much
# sense to provide one, since we don't even know where the program will be.

multilibtool_LIBRARIES += %D%/liblc32glossx.a
%C%_liblc32glossx_a_SOURCES = \
	%D%/x/_start.S \
	%D%/x/_exit.S \
	%D%/x/_sbrk.c \
	%D%/x/_open.c \
	%D%/x/_close.c \
	%D%/x/_isatty.c \
	%D%/x/sigprocmask.c

###
# Backend for TRAP-based syscalls
#
# These work based off the TRAPs defined in the book. They don't interact with
# MMIO at all. As such, these may be unsuited for supervisor-mode programming.
# Still, it's useful since it's easy to simulate.

multilibtool_LIBRARIES += %D%/liblc32glossTrap.a
%C%_liblc32glossTrap_a_SOURCES = \
	%D%/x/_start.S \
	%D%/x/_sbrk.c \
	%D%/x/_open.c \
	%D%/x/sigprocmask.c \
	%D%/Trap/_exit.S \
	%D%/Trap/_close.c \
	%D%/Trap/_isatty.c \
	%D%/Trap/internal/fsstate.c
