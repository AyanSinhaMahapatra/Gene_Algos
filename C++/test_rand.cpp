#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

int check_array(int rand_gen,int *array_random,int size)
{
	int flag = 1; 
	cout<<"Size = "<<size<<endl;
	for(int i=1;i<=size;i++)
	{
		if(array_random[i]==rand_gen)
			flag=0;
	}
	return flag;
}

int main()
{
	 int array_int[7];
    for(int i=1;i<7;i++)
    {
    	array_int[i]=i+1;
    }
    for(int i=1;i<7;i++)
    {
    	cout<<i+1<<" = "<<check_array(i+1,array_int,6)<<endl;
    }
	return 0;
}