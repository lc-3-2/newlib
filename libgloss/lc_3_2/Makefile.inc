###
# Register the linker scripts
#
# We have one linker script for each "backend". So to select between them, we
# link the right library and choose the right linker script.
#
# As for what this target does, it seems to install the linker scripts.

multilibtool_DATA += \
	%D%/lc32newlibTrap.ld

###
# Backend for TRAP-based syscalls
#
# These work based off the TRAPs defined in the book. They don't interact with
# MMIO at all. As such, these may be unsuited for supervisor-mode programming.
# Still, it's useful since it's easy to simulate.

multilibtool_LIBRARIES += %D%/liblc32newlibTrap.a
%C%_liblc32newlibTrap_a_SOURCES = \
	%D%/common/_start.S \
	%D%/lc32newlibTrap/_exit.S
