#include <drivers/io/io.h>

/* The I/O ports */
#define FB_COMMAND_PORT		0x3d4
#define FB_DATA_PORT		0x3d5

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

char *fb = (char *) FB_START;

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
 *
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

/** fb_write:
 * Writes the buffer of size len to the screen, moving the cursor automatically
 * and scrolling the page if necessary. This is the function to interact with
 * this driver.
 *
 * @param buf: Buffer with contents to write on the screen.
 * @param len: Size of the buffer
 * @ ret: < 0 if fails or 0 if worked
 */
int fb_write(char *buf, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len; i++)
		fb_write_cell(i * 2, buf[i], FB_WHITE, FB_BLACK);

	fb_move_cursor(len - 1); // -1 to ignore the last byte '/0'

	return 0;
}
