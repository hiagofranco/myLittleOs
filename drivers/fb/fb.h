#ifndef INCLUDE_FB_H
#define INCLUDE_FB_H

void fb_move_cursor(unsigned short pos);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
int fb_write(char *buf, unsigned int len);

#endif /* INCLUDE_FB_H */
