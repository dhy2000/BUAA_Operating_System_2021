#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int m = 0, n1 = n;
	while (n1) {
	    m = (m << 3) + (m << 1) + (n1 % 10);
	    n1 /= 10;
	}
	if(m == n){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
