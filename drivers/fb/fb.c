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

#define FB_COLUMNS		80
#define FB_ROWS			25

struct frame_buffer {
	char *buf;		/* buffer contents to write */
	unsigned short c_pos;	/* current cursor position */
};

struct frame_buffer fb = {
	.buf = (char *) FB_START,
	.c_pos = 0,
};

/** fb_scroll_up
 * Scrolls the screen up when it is full
 */
void fb_scroll_up(void)
{
	int i, j, src, dest;

	/* The display is 80 * 25 chars (2000 chars), however for each char we have another byte
	 * for the color, so the array has 4000 bytes (or elements). Therefore, each row has 160
	 * bytes, that is the reason we need to multiply the position by 2.
	 */
	for (i = 0; i < FB_ROWS - 1; i++) {
		for (j = 0; j < FB_COLUMNS; j++) {
			src = (i * FB_COLUMNS + j) * 2;
			dest = ((i + 1) * FB_COLUMNS + j) * 2;
			fb.buf[src] = fb.buf[dest]; // char
			fb.buf[src + 1] = fb.buf[dest + 1]; // colors
		}
	}

	/* Here we need to clear only the last line, so (80 * 24 * 2) */
	for (i = 0; i < FB_COLUMNS; i++) {
		src = ((FB_ROWS - 1) * FB_COLUMNS + i) * 2;
		fb.buf[src] = ' ';
		fb.buf[src + 1] = ((FB_BLACK & 0x0f) << 4) | (FB_WHITE & 0x0f);
	}
}

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
 * @param c: Character
 * @param fg: Foreground color
 * @param bg: Background color
 */
void fb_write_cell(char c, unsigned char fg, unsigned char bg)
{
	if (fb.c_pos == FB_COLUMNS * FB_ROWS) { // If the screen is full, scroll up first
		fb_scroll_up();
		fb.c_pos = (FB_ROWS - 1) * FB_COLUMNS; // Reset to 24 row
	}

	fb.buf[fb.c_pos * 2] = c;
	fb.buf[fb.c_pos * 2 + 1] = ((bg & 0x0f) << 4) | (fg & 0x0f);

	fb.c_pos += 1;
	fb_move_cursor(fb.c_pos);
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

	/* For now, workaround the null byte at the end */
	if (buf[len - 1] == '\0')
		len = len - 1;

	for (i = 0; i < len; i++)
		fb_write_cell(buf[i], FB_WHITE, FB_BLACK);

	return 0;
}
