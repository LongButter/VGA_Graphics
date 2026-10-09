#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define MAX_ROW 199
#define MAX_COL 319

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

void init_vga(){

	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x13;

	int86(0x10, &regs, &regs);
}

void plot_pixel(int column, int row, int colour){

	union REGS regs;
	regs.h.ah = 0x0C;
	regs.h.al = colour;
	regs.x.cx = column;/*between 0 - 319*/
	regs.x.dx = row;/*between 0 - 199 */
	int86(0x10, &regs, &regs);
}

void line(int column, int colour){
	int row;
	for (row = 0; row <= MAX_ROW; row++){
		plot_pixel(column, row, colour);
	}

}

void main(){
	/*var init*/
	int i;
	/*main*/
	init_vga();
	for (i = 0; i <= MAX_COL; i++){
		line(i, VGA_LRED);
		line(i-1, VGA_BLACK);
	}

}