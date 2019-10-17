#ifndef COLOUR_C
#define COLOUR_C

#include <stdint.h>
#include <stdio.h>
#define CLAMP(val, min, max) ((val < min) ? (val = min) : (val > max) ? (val = max) : val)

typedef struct
{
	uint8_t r, g, b;
} colour;

typedef colour color;
typedef colour color_t;
typedef colour colour_t;

char* getGREY(uint8_t g)
{
	CLAMP(g, 0U, 255U);
	char* buff;
	sprintf(buff, "\033[38;2;%u;%u;%um", g, g, g);
	return buff;
}

char* getRGB(uint8_t r, uint8_t g, uint8_t b)
{
	CLAMP(r, 0U, 255U);
	CLAMP(g, 0U, 255U);
	CLAMP(b, 0U, 255U);
	char* buff;
	sprintf(buff, "\033[38;2;%u;%u;%um", r, g, b);
	return buff;
}

const static char RESET[] = "\033[m", BLACK[] = "\033[38;2;0;0;0m",
				  WHITE[] = "\033[38;2;255;255;255m", RED[] = "\033[38;2;255;0;0m",
				  GREEN[] = "\033[38;2;0;255;0m", BLUE[] = "\033[38;2;0;0;255m",
				  YELLOW[] = "\033[38;2;255;255;0m", MAGENTA[] = "\033[38;2;255;0;255m",
				  CYAN[] = "\033[38;0;2;255;255m", PURPLE[] = "\033[38;2;127;32;183m",
				  LIME[] = "\033[38;2;111;255;8m", BROWN[] = "\033[38;2;142;69;23m",
				  ORANGE[] = "\033[38;2;255;127;8m", COLOUR_FILE[] = "\033[38;2;8;255;12m",
				  COLOUR_DIR[] = "\033[38;2;21,162,252m";
#endif
