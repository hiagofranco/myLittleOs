#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT		0x3D4
#define FB_DATA_PORT		0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND	14
#define FB_LOW_BYTE_COMMAND	15

#define FB_START		0x000b8000
#define FB_BLACK		0
#define FB_BLUE			1
#define FB_GREEN		2
#define FB_CYAN			3
#define FB_RED			4
#define FB_MAGENTA		5
#define FB_BROWN		6
#define FB_LIGHT_GREY		7
#define FB_DARK_GREY		8
#define FB_LIGHT_BLUE		9
#define FB_LIGHT_GREEN		10
#define FB_LIGHT_CYAN		11
#define FB_LIGHT_RED		12
#define FB_LIGHT_MAGENTA	13
#define FB_LIGHT_BROWN		14
#define FB_WHITE		15

char *fb = (char *) FB_START; // for now, global defined frame buffer

/** fb_move_cursor:
 * Moves the cursor of the framebuffer to the give position
 *
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
	outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	outb(FB_DATA_PORT, ((pos >> 8) & 0x00ff));
	outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
	outb(FB_DATA_PORT, pos & 0x00ff);
}

/** fb_write_cell:
 * Writes a character with the given foreground and background to position i in
 * the framebuffer.
 * @param i: Location in the framebuffer
 * @param c: Character
 * @param fg: Foreground color
 * @param bg: Background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
	fb[i] = c;
	fb[i + 1] = ((bg & 0x0f) << 4) | (fg & 0x0f);
}

void kmain(void)
{
	char hello_world[] = "Hello World!";
	unsigned int i;

	for (i = 0; i < sizeof(hello_world); i++)
		fb_write_cell(i * 2, hello_world[i], FB_WHITE, FB_BLACK);

	fb_move_cursor(0x50);

	for(;;);
}
