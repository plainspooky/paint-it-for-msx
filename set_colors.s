    .area _CODE

_set_colors::

    push ix

    ld iy,(#0xFCC1-1)
    ld ix,#0x62
    call #0x001C

    pop ix

    ret
