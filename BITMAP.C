#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define MAX_Y 199
#define MAX_X 319
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

/*set macros for colours*/
enum Colours{
	VGA_BLACK,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LGRAY,
	VGA_DGRAY,
	VGA_LBLUE,
	VGA_LGREEN,
	VGA_LCYAN,
	VGA_LRED,
	VGA_LMAGENTA,
	VGA_YELLOW,
	VGA_WHITE
};
typedef unsigned char byte;
byte far *vga = (byte far*)0xA0000000L;

void init_vga(){

	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x13;

	int86(0x10, &regs, &regs);
}

void plot_pixel(int x, int y, int colour){
	unsigned short offset;
	offset = SCREEN_WIDTH *y + x;
	vga[offset] = colour;

}

void vert_line(int x, int colour){
	int y;
	for (y = 0; y <= MAX_Y; y++){
		plot_pixel(x, y, colour);
	}

}

void line(int x1, int x2, int y1, int y2, int colour){
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d = 2 * dy - dx;
	int y = y1;
	int x = x1;
	int endpoint;

	if (dx != 0){
		endpoint = (x1 < x2) ? x2 : x1;
		for (; x <= endpoint; x++){ /*real*/
			plot_pixel(x, y, colour);
			if (d < 0){
				d += 2 * dy;
			}
			else{
				d += 2 * (dy - dx);
				y++;
			}
		}
	}
	else{

		endpoint = (y1 < y2) ? y2 : y1;
		d = 2 * dx - dy;
		for (; y <= endpoint; y++){  /*my solution for dx = 0*/
			plot_pixel(x, y, colour);
			if (d < 0){
				d += 2 * dx;
			}
			else{
				d += 2 * (dx - dy);
				x++;
			}
		}
	}
}

void hori_line(int y, int colour){
	int x;
	for (x=0;x <= MAX_X; x++){
		plot_pixel(x, y, colour);
	}
}

void main(){
	/*var init*/
	int i;
	/*main*/
	init_vga();
	for (i = 0; i <= MAX_X; i++){
		line(i, i, 0, MAX_Y, VGA_RED);
		line(i, i, 0, MAX_Y - i, VGA_CYAN);
	}
	getch();

}