/* w.c - polecenie w przyjmuje jako argument 1 albo 0 */
/* wlaczajac/wylaczajac algorytm worst fit w systemie Minix */
#include <stdlib.h>
#include <unistd.h>
#include <lib.h>


PUBLIC int worst_fit( int w )
{
	message m;
    m.m1_i1 = w;

    if (_syscall(MM, WORSTFIT, &m) != 0) {
        printf("%s\n", "Couldn't change alloc mode");
    }
    return 0;
}

int
main( int argc, char *argv[] )
{
	if( argc < 2 )
		return 1;
	worst_fit( atoi( argv[1] ) );
	return 0;
}