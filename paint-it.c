/*
    Paint-it -- Program file
    A flood-it game written in SDCC for MSX computer.

    Copyright (C) 2020  Giovanni Nunes

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include<stdio.h>
#include<stdlib.h>
#include "types.h"

#include "paint-it.h"

// Arena dimensions
#define HEIGHT 16
#define WIDTH 16
#define MAX_MOVES 28

// must be global to simplify data transfdering between functions...
uint8_t arena[WIDTH][HEIGHT];
uint8_t current_color, new_color;

/*
    -------------------------------------------------------------------
    GENERAL USE ROUTINES
    -------------------------------------------------------------------
*/
void color(uint8_t fore, uint8_t back, uint8_t brdr){
    /*
        Do color changing on any MSX screeen mode. Receive foreground
        color as `fore`, background color as `back` and border color
        as `brdr`.

        0 >= `fore`|`back`|`brdr` <= 15
    */
    *forclr = fore;
    *bakclr = back;
    *bdrclr = brdr;
    set_colors();

} // color()

void locate(uint8_t x, uint8_t y){
    /*
        Set cursor position in (X,Y) coordinates. Receive column as `x`
        and line as `y`. The first position is (1,1).

        1 >= x <= 32, 40, 64, 80
        1 >= y <= 24
    */
    *csrx = x;
    *csry = y;

} // locate()

/*
    -------------------------------------------------------------------
    GENERIC ROUTINES (not related with game logic)
    -------------------------------------------------------------------
*/
void init_screen(void){
    /*
        Change VDP registers and properly populate VRAM.
    */
    uint16_t third, i, j;
    uint8_t k, l, m;

    // change VDP registers SCREEN 1 act like SCREEN 2...
    write_vdp((uint8_t) 0, (uint8_t) RG0SAV | 2);
    write_vdp((uint8_t) 1, (uint8_t) RG1SAV & 0xE7);
    write_vdp((uint8_t) 3, (uint8_t) 0xDF);
    write_vdp((uint8_t) 4, (uint8_t) 3);

    // set attributes for all thirds of screen...
    for (third = 0; third < 6144; third = third + 2048){

        // paint the first 128 characters in white/black (ASCII table)
        for (i = 8192; i < 9216; i++){
            write_vram((uint16_t) third + i, (uint8_t) 0xF1);
        }

        // paint each block with its own color
        k = 0;
        for (i = 9216; i < 9280; i = i + 8){
            // color selection...
            switch (k){
                case BLOCK_BLUE:
                    l = 0x51;
                    break;

                case BLOCK_CYAN:
                    l = 0x71;
                    break;

                case BLOCK_GREEN:
                    l = 0x21;
                    break;

                case BLOCK_MAGENTA:
                    l = 0xD1;
                    break;

                case BLOCK_RED:
                    l = 0x81;
                    break;

                case BLOCK_YELLOW:
                    l = 0xA1;
                    break;

                default:
                    l = 0xF1;
            }

            for (j = 0; j < 8; j++){
                write_vram((uint16_t) third + i + j, (uint8_t) l);
            }
            k++;
        }

        // paint game main title ("PAINT-IT")
        k = 0;
        m = 0;
        for (i = 9280; i < 9376; i++){
            // color selection...
            switch (k){
                case 0:
                    l = 0x51;
                    m = 0xD1;
                    break;

                case 2:
                    l = 0x71;
                    m = 0x81;
                    break;

                case 5:
                    l = 0xC1;
                    m = 0xA1;
                    break;
            }
            // paint upper tile and lower tile as well
            write_vram((uint16_t) third + i, (uint8_t) l);
            write_vram((uint16_t) third + i + 96, (uint8_t) m);

            k = (k + 1) & 7;
        }

        // border elements
        for (i = 9472; i < 9536; i++){
            write_vram((uint16_t) third + i, (uint8_t) 0xE1);
        }
    }

    // copy tiles to all thirds of screen...
    set_tiles(0);
    set_tiles(2048);
    set_tiles(4096);

} // init_screen()

void display_cw_logo(){
    /*
        Write "Crunchworks" logo at center of screen.
    */
    uint8_t i;
    uint16_t j;

    // first line, upper part from "C", "h" and "k"
    write_vram((uint16_t) 6144 + 32 * 11 +  9, (uint8_t) 1);
    write_vram((uint16_t) 6144 + 32 * 11 + 10, (uint8_t) 2);
    write_vram((uint16_t) 6144 + 32 * 11 + 15, (uint8_t) 3);
    write_vram((uint16_t) 6144 + 32 * 11 + 20, (uint8_t) 3);

    j = (uint16_t) 6144 + 32 * 12 + 9;

    // second line, lower part from "Crunchworks"
    for (i = 4; i < 18; i++){
        write_vram(j, i);
        j++;
    }

} // display_cw_logo()

void display_gpl_message(){
    /*
        Makes GPL message scroll up on screen
    */
    puts("Paint-it\r\nCopyright (C) 2020 Giovanni Nunes\r");
    puts("This program comes with ABSOLUTELY NO WARRANTY; This is free software, and you are welcome to redistribute it under certain conditions;");

} // display_gpl_message()

/*
    -------------------------------------------------------------------
    GAME DISPLAY ROUTINES
    -------------------------------------------------------------------
*/
void draw_game_title(){
    /*
        Draw "PAINT-IT" title on screen at (9,2)
    */
    locate(9, 2);
    puts("\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x8C\x8D\x90\x91");

    locate(9, 3);
    puts("\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F\x98\x99\x9C\x9D");

} // draw_game_title()

void draw_arena_frame(){
    /*
        Draw window frame that contains game arena.
    */
    uint8_t i;
    uint16_t j;

    j = (uint16_t) 6144 + 32 * 3;

    // left upper corner
    write_vram(j + 7, 0xA0);

    for (i = 8; i < 24; i++){
        write_vram(j + i, 0xA1);
    }

    // right upper corner
    write_vram(j + 24, 0xA2);

    // the middle....
    for (j = 4 * 32; j < 32 * 20; j = j + 32){

        write_vram((uint16_t) 6151+j, 0xA3);

        for (i = 8; i < 24; i++){
            write_vram(j + 6144 + i, 0x80);
        }

        write_vram((uint16_t) 6168 + j, 0xA4);
    }

    j = (uint16_t) 6144 + 32 * 20;

    // left lower corner
    write_vram(j + 7, 0xA5);

    for (i = 8; i < 24; i++){
        write_vram(j + i, 0xA6);
    }

    // right lower corner
    write_vram(j + 24, 0xA7);

} // draw_arena_frame()

void draw_arena(){
    /*
        Format and display content of `arena` array on screen.
    */
    uint8_t x, y;

    for (y = 0; y < 16; y++){
        for (x = 0; x < 16; x++){
            write_vram(
                (uint16_t) 6144 + 32 * (y + 4) + x + 8,
                128 + arena[x][y]
            );
        }
    }

} // draw_arena()

/*
    -------------------------------------------------------------------
    GAME ROUTINES
    -------------------------------------------------------------------
*/
uint8_t check_arena(){
    /*
        Check if there is only one color on `arena` array.
        Return `0` if there is only one color or `1` if not.
    */
    uint8_t x, y;

    for (y = 0; y < 16; y++){
        for (x = 0; x < 16; x++){
            if (arena[x][y] != current_color){
                // different color, end routine!
                return 1;
            }
        }
    }
    return 0;

} // check_arena()

void display_game_screen(){
    /*
        Display game main (and unique) screen...
    */

    // clear screen and put cursor at home
    puts("\x0c");

    // write "PAINT-IT" and arena frame as well...
    draw_game_title();
    draw_arena_frame();

} // display_game_screen()

void display_help(){
    /*
        Display game usage on arena space and wait for «ESC» to exit.
    */

    // reduce screen width, this is a nasty trick but works! :)
    *linlen = 16;

    locate(1, 5);

    printf("                ");
    printf("  *** HELP ***  ");
    printf("                ");
    printf("----------------");
    printf(" Use:           ");
    printf(" [B]lue,[C]yan, ");
    printf(" [G]reen,[M]a-  ");
    printf(" genta,[R]ed or ");
    printf(" [Y]ellow.      ");
    printf("                ");
    printf(" <CTRL>+<X> to  ");
    printf(" exit program.  ");
    printf("----------------");
    printf("                ");
    printf(" <ESC> to close ");
    printf("               ");

    // wait for ESC key...
    while (getchar() != 27) {};

    // return screen width to default value
    *linlen = 32;

} // display_help()

void flood_fill(int  x, int y){
    /*
        That's very simple, if cell value is `current_color`, change
        it to `new_color` value, and try the same with its neighbours.
        Receive colum as `x` and line as `y`.

        Flood-fill routine that I've copied from Rosetta Code
        https://rosettacode.org/wiki/Bitmap/Flood_fill
    */
    if (0 <= y && y < HEIGHT &&
        0 <= x && x < WIDTH &&
        arena[x][y] == current_color)
    {
        arena[x][y] = new_color;
        flood_fill(x - 1, y);
        flood_fill(x + 1, y);
        flood_fill(x, y - 1);
        flood_fill(x, y + 1);
    }

} // flood_fill()

void initialize_arena(){
    /*
        Populate a new arena in `arena` array.
    */
    uint8_t x, y, i;

    for (y = 0; y < 16; y++){
        for (x = 0; x < 16; x++){
            // pick an integer between 0 and 7
            i = rand() & 7;
            // habdle with values 0 and 7
            if (i == 0) i++;
            if (i == 7) i--;
            // store new color
            arena[x][y] = i;
        }
    }

} // initialize_arena()

/*
    -------------------------------------------------------------------
    GAME
    -------------------------------------------------------------------
*/
int game(){
    /*
        Game Engine
    */
    uint8_t k, game_status, moves;

    *linlen = 32;

    moves = 0;
    game_status = GAME_RUN;

    initialize_arena();
    current_color = arena[0][0];
    new_color = current_color;

    display_game_screen();

    // waith for <SPACE> to start
    locate(9, 23);
    puts(" Press <SPACE>! ");

    while (getchar() != 32) {};

    locate(9, 23);
    puts("          \x87 Help");

    while (game_status == GAME_RUN) {

        draw_arena();

        // update move information
        locate(9, 23);
        printf("%i/%i", moves, MAX_MOVES);

        k = getchar();

        switch (k){
            case 66:    // "B"
            case 98:    // "b"
                new_color = BLOCK_BLUE;
                break;

            case 67:    // "C"
            case 99:    // "c"
                new_color = BLOCK_CYAN;
                break;

            case 71:    // "G"
            case 103:   // "g"
                new_color = BLOCK_GREEN;
                break;

            case 77:    // "M"
            case 109:   // "m"
                new_color = BLOCK_MAGENTA;
                break;

            case 82:    // "R"
            case 114:   // "r"
                new_color = BLOCK_RED;
                break;

            case 89:    // "Y"
            case 121:   // "y"
                new_color = BLOCK_YELLOW;
                break;

            case 63:    // "?"
                display_help();
                break;
            case 24:    // <CTRL>+<X>
                game_status = GAME_EXIT;
                break;
        }

        if (current_color != new_color){

            // fill arena with chosen color...
            flood_fill(0, 0);

            current_color = new_color;

            if (check_arena() == 0) {
                game_status = GAME_WIN;
            }

            moves++;

            if (moves >= MAX_MOVES){
                game_status = GAME_LOSE;
            }

            // if game status changed, update arena for last time...
            if (game_status != GAME_RUN) draw_arena();
        }
    }

    // handle with loop exit...
    switch (game_status){
        case GAME_WIN:
            locate(17, 23);
            puts("YOU WIN!");
            break;

        case GAME_LOSE:
            locate(9, 23);
            printf("%i/%i  YOU LOSE!", moves, MAX_MOVES);
            break;

        default:
            locate(18, 23);
            puts("ABORTED");
    }

    locate(9, 24);
    printf("Play again?  Y/N");

    while (1 == 1) {
        switch (getchar()){
            case 89:
            case 121:
                return 0;

            case 78:
            case 110:
                return 1;
        }
    }

} // game()

int main(void){

    // set system variables
    forclr = (char *) FORCLR;
    bakclr = (char *) BAKCLR;
    bdrclr = (char *) BDRCLR;
    scrmod = (char *) SCRMOD;
    csrx = (char *) CSRX;
    csry = (char *) CSRY;
    linlen = (char*) LINLEN;
    crtcnt = (char*) CRTCNT;

    // change screen colors
    color(15, 0, 0);

    // set SCREEN 1 (32×24 Text) and initialize screen...
    set_video_mode(1);
    init_screen();

    display_cw_logo();

    while (game() == 0) {};

    // return to SCREEN 0 before exit...
    set_video_mode(0);
    display_gpl_message();

    return 0;

} // main()
