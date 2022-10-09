
int isbad(int a)
{
    if(a > 3)
        return (1);
    return (0);
}

void    fill_arr(int **arr, int start, int last)
{
    int i;

    i = 0;
    while (i < last)
    {
        (*arr)[i] = start + 1;
        start++;
    }
}

int f (int  n)
{
    static int start = 1;
    int *arr = new int[n - (start - 1)];
    fill_arr(&arr, start, n);
    int *first = &arr[0];
    int *last = &arr[n - arr[0]];
    if(isbad(arr[(start + n) / 2]))
    {
        start = arr[(start + n) / 2];
        n == arr[n / 2];
        f()
    }
    else
    {
        start = arr[((start + n) / 2) + 1];
        f(n);
    }
}
int main()
{
	
	
}