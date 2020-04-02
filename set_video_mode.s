    .area _CODE

_set_video_mode::

    push ix

    ld ix,#0
    add ix,sp

    ld a,4(ix)

    ld iy,(#0xFCC1-1)
    ld ix,#0x5f
    call #0x001C

    pop ix
    ret
