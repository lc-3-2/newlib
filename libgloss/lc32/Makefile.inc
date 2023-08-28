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
# Backend for TRAP-based syscalls
#
# These work based off the TRAPs defined in the book. They don't interact with
# MMIO at all. As such, these may be unsuited for supervisor-mode programming.
# Still, it's useful since it's easy to simulate.

multilibtool_LIBRARIES += %D%/liblc32glossTrap.a
%C%_liblc32glossTrap_a_SOURCES = \
	%D%/x/_start.S \
	%D%/Trap/_exit.S
