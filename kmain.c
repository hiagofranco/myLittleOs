#include <drivers/fb/fb.h>

void kmain(void)
{
	int i;
	char hello_world_a[] = "Hello World! A                                                                  ";
	char hello_world_b[] = "Hello World! B                                                                  ";

	for (;;) {
		for (i = 0; i < 30000000; i++);
		fb_write(hello_world_a, sizeof(hello_world_a));
		for (i = 0; i < 30000000; i++);
		fb_write(hello_world_b, sizeof(hello_world_b));
	}
	for(;;);
}
