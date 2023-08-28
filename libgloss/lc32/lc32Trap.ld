TARGET(elf32-lc-3.2)
ENTRY(_start)

MEMORY {
  ram (rwx) : ORIGIN = 0x30000000, LENGTH = 0xc0000000
}

SECTIONS {

  /*
    Place the startup code first. This way, we're fine even if we start
    execution at the start of the data instead of where `_start` points.
  */
  .start : {
    *(.start)
  } >ram

  .text : {
    *(.text)
    *(.text.*)
  } >ram

  .rodata : {
    *(.rodata)
    *(.rodata.*)
  } >ram

  .data : {
    *(.data)
    *(.data.*)
  } >ram

  /*
    Set up constructor and destructor arrays. The names are important, since LLD
    will emit symbols for their start and end. Also make sure to handle
    priorities. Things with priority should run first, and things with lower
    priority numbers should run first. Also make sure these aren't garbage
    collected.

    Note that `.fini_array` uses `SORT_BY_INIT_PRIORITY`. This detects that the
    inputs are finalizers, and automatically sorts in the "reverse" order
    compared to constructors.
  */
  .preinit_array ALIGN(4) : {
    KEEP(*(SORT_BY_INIT_PRIORITY(.preinit_array.*)))
    KEEP(*(.preinit_array))
  }
  .init_array ALIGN(4) : {
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
    KEEP(*(.init_array))
  }
  .fini_array ALIGN(4) : {
    KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
    KEEP(*(.fini_array))
  }

  /*
    Handle `.sdata` for the global pointer. This is assumed to be word-aligned,
    so force it to alignment just like `.bss`.
  */
  .sdata ALIGN(4) : {
    __gp_init = .;
    *(.sdata)
    *(.sdata.*)
  }

  /*
    Handle the symbols that tell `crt0` where to zero out. These are assumed to
    be word-aligned, so we have to force `.bss` to that alignment even if it
    doesn't have to be. We also have to ensure anything afterward doesn't
    encroach on the 4-byte alignment
  */
  .bss ALIGN(4) : {
    __bss_start = .;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  } >ram

  /* Set the heap bounds. Note `malloc` wants 8-byte alignment. */
  . = ALIGN(8);
  __heap_start = .;
  __heap_end = 0x80000000;

  /* Don't forget to define where the stack starts */
  __sp_init = 0xf0000000;

  /*
    Call finalizers
    See: newlib/libc/stdlib/__call_atexit.c
  */
  __libc_fini = __libc_fini_array;
}
