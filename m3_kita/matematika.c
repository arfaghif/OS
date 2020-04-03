#include "matematika.h"
int mod(int a, int b){
	while (a>=b){
		a-=b;
	}
	return a;
}
int div(int a, int b){
	int result=0;
	while(a>=b){
		a-=b;
		result +=1;
	}
	return result;
}