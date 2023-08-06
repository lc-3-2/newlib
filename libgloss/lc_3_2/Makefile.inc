###
# Register the linker scripts
#
# We have one linker script for each "backend". So to select between them, we
# link the right library and choose the right linker script.
#
# As for what this target does, it seems to install the linker scripts.

multilibtool_DATA += \
	%D%/lc32trapnewlib.ld

###
# Library for TRAP-based syscalls
#
# These work based off the TRAPs defined in the book. They don't interact with
# MMIO at all. As such, these may be unsuited for supervisor-mode programming.
# Still, it's useful since it's easy to simulate.

multilibtool_LIBRARIES += %D%/liblc32trapnewlib.a
%C%_liblc32trapnewlib_a_SOURCES = \
	%D%/crt0.S
