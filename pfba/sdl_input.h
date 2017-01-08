/*
 * FinalBurn Alpha for Dingux/OpenDingux
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _SDL_INPUT_H_
#define _SDL_INPUT_H_

#define INPUT_DELAY 150
#define JOY_AXIS_LX 0
#define JOY_AXIS_LY 1
#define JOY_AXIS_RX 2
#define JOY_AXIS_RY 3

#define KEYPAD_UP       0x0001
#define KEYPAD_DOWN     0x0002
#define KEYPAD_LEFT     0x0004
#define KEYPAD_RIGHT    0x0008
#define KEYPAD_COIN     0x0010
#define KEYPAD_START    0x0020
#define KEYPAD_FIRE1    0x0040
#define KEYPAD_FIRE2    0x0080
#define KEYPAD_FIRE3    0x0100
#define KEYPAD_FIRE4    0x0200
#define KEYPAD_FIRE5    0x0400
#define KEYPAD_FIRE6    0x0800
#define KEYPAD_QUIT     0x1000

enum {
    PSP2_LEFT		= 7,
    PSP2_DOWN		= 6,
    PSP2_RIGHT		= 9,
    PSP2_UP			= 8,
    PSP2_START		= 11,
    PSP2_SELECT		= 10,
    PSP2_SQUARE		= 3,
    PSP2_CROSS		= 2,
    PSP2_CIRCLE		= 1,
    PSP2_TRIANGLE	= 0,
    PSP2_R1			= 5,
    PSP2_L1			= 4
};

void do_keypad();
void sdl_input_init();
int sdl2_input_read();
int sdl2_input_wait_button();
void sdl2_input_clear();

#endif // _SDL_INPUT_H_
