#include "cub3d.h"

float fixed_mul(double a, double b)
{
	int fixeda;
	int fixedb;
	int fixedsum;
	fixeda = roundf(a * (1 << 8));
	fixedb = roundf(b * (1 << 8));
	fixedsum = (fixeda * fixedb) / (1 << 8);
	return ((float) ((float) fixedsum / (1 << 8)));
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