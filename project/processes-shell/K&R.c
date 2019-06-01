int foo() ;

main() {
	printf("%d\n",foo(1,2)) ;
}

foo(a,b)
int a ;
int b ;
{
	printf("%d,%d\n",a,b) ;
	return a + b ;
}
