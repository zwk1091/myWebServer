#include"hello.h"
#include<iostream>
using namespace std;

int main()
{
	hello my=hello();
	int n;
	cin>>n;
	for(int i=0;i<n;i++) {
		cout<<i<<endl;
	}
	cout<<"ggg"<<endl;
	my.myprint();
	return 0;
}