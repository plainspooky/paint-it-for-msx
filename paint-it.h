/*
    PAINT-IT for MSX - header file
*/

// MSX-BIOS system variables (read only)
#define RG0SAV (* (char *) 0xf3df)
#define RG1SAV (* (char *) 0xf3e0)
#define RG2SAV (* (char *) 0xf3e1)
#define RG3SAV (* (char *) 0xf3e2)
#define RG4SAV (* (char *) 0xf3e3)
#define RG5SAV (* (char *) 0xf3e4)
#define RG6SAV (* (char *) 0xf3e5)
#define RG7SAV (* (char *) 0xf3e6)
#define OLDSRC (* (char *) 0xfcb0)

// MSX-BIOS system variables
#define CSRY   0xf3dc
#define CSRX   0xf3dd
#define FORCLR 0xf3e9
#define BAKCLR 0xf3ea
#define BDRCLR 0xf3eb
#define SCRMOD 0xfcab
#define LINLEN 0xf3b0
#define CRTCNT 0xf3b1

extern void set_colors(void);
extern void set_video_mode(uint8_t mode);
extern void set_graphic_mode(void);
extern void set_text_mode(void);
extern void set_tiles(uint16_t a);
extern void write_vdp(uint8_t r, uint8_t v);
extern uint8_t read_vram(uint16_t a);
extern void write_vram(uint16_t a, uint8_t v);

// cursor positon
static char* csrx;
static char* csry;

// screen colors
static char* forclr;
static char* bakclr;
static char* bdrclr;

// screen parameters
static char* scrmod;
static char* linlen;
static char* crtcnt;

// block colors
#define BLOCK_BLUE 1
#define BLOCK_CYAN 2
#define BLOCK_GREEN 3
#define BLOCK_MAGENTA 4
#define BLOCK_RED 5
#define BLOCK_YELLOW 6

// game status
#define GAME_RUN 0
#define GAME_WIN 1
#define GAME_LOSE 2
#define GAME_EXIT 3
