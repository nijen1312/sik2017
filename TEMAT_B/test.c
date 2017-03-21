#include<stdio.h>


typedef struct testStructure{int a1,a2;} Testowa;

int main()
{
	Testowa b1;
	b1.a1=50;
	b1.a2=170;
	
	int temp=sizeof(int);
	printf("%d\n",*(&b1+temp));
//	printf("%d\n",b1[1]);

	return 0;
}
