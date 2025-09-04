#include "fb.h"

void kmain(void)
{
	char hello_world[] = "Hello World!";

	fb_write(hello_world, sizeof(hello_world));

	for(;;);
}
