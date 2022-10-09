#include <stdio.h>

void	f()
{
	static int k = 1;
	printf("%d\n", k);
	k++;
	if(k < 10)
		f();
	else
			return;
}
int main()
{
	f();
}
