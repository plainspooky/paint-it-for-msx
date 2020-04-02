    .area _CODE

_write_vram::

    push ix

    ld ix,#0
    add ix,sp

    ld l,4(ix)
    ld h,5(ix)
    ld a,6(ix)

    ld iy,(#0xFCC1-1)
    ld ix,#0x4D
    call #0x001C

    pop ix
    ret

_read_vram::
    push ix

    ld ix,#0
    add ix,sp

    ld l,4(ix)
    ld h,5(ix)

    ld iy,(#0xFCC1-1)
    ld ix,#0x4A
    call #0x001C

    ld l,a

    pop ix
    ret

_write_vdp::
    push ix

    ld ix,#0
    add ix,sp

    ld c,4(ix)
    ld b,5(ix)

    ld iy,(#0xFCC1-1)
    ld ix,#0x47
    call #0x001C

    pop ix
    ret
