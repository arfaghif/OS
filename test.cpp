#include <iostream>
using namespace std;
int div2(int a, int b);
int mod2(int a, int b);
int main(){
	cout << mod2(9,3) << " "<< div2(9,3)<<endl;
	cout << mod2(40,6) << " "<< div2(40,6)<<endl; 
	cout << mod2(1,4) << " "<< div2(1,4)<<endl; 
	return 0;
}
int div2(int a, int b){
	int result=0;
	while(a>=b){
		a-=b;
		result +=1;
	}
	return result;
}
int mod2(int a, int b){
	while (a>=b){
		a-=b;
	}
	return a;
}