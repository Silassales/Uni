#include<stdio.h>
#include<stdlib.h>
float f;
struct E {
	float abc;
};

void constructorE (struct E * the_E) {
}

/* complex class structure with initialized variable */
int main (int argc, char *argv[]) {
	struct E myE ;
	constructorE(&myE);
	struct mystruct *p ;
	p = myE .method ( ) ;
	if ( p->f ==  1.2) return ( 0) ;
	else return ( 1) ;
	}

