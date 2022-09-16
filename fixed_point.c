#include "cub3d.h"

float fixed_mul(double a, double b)
{
	int fixeda;
	int fixedb;
	int fixedsum;
	float d = (15 / 2) * 50;
	printf("d %f d\n", a);
	fixeda = roundf(a * 256);
	fixedb = roundf(b * 256);
	fixedsum = fixeda * fixedb;
	printf("\n aa  %d and %d and %d aa\n", fixeda, fixedb, fixedsum);
	return ((float) fixedsum * (1 >> 16));
}

float fixed_add(float a, float b)
{
	int fixeda;
	int fixedb;
	int fixedsum;

	fixeda = roundf(a * (1 << 8));
	fixedb = roundf(b * (1 << 8));
	fixedsum = fixeda + fixedb;
	return ((float) fixedsum * (1 >> 8));
}

float fixed_minus(float a, float b)
{
	int fixeda;
	int fixedb;
	int fixedsum;

	fixeda = roundf(a * (1 << 8));
	fixedb = roundf(b * (1 << 8));
	fixedsum = fixeda - fixedb;
	return ((float) fixedsum * (1 >> 8));
}

float fixed_oth(float a, float b)
{
	int fixeda;
	int fixedb;
	int fixedsum;

	fixeda = roundf(a * (1 << 8));
	fixedb = roundf(b * (1 << 8));
	fixedsum = (fixeda * (1 << 8)) / fixedb;
	return ((float) fixedsum * (1 >> 8));
}