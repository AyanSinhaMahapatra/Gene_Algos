#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

int binary_to_dec(int *array,int array_length)
{
	int sum = 0;

	for(int i=array_length;i>0;i--)
		sum += array[array_length - i + 1]*pow(2,i-1) ;

	return sum;
}

void random_generate_array(int *array,int length) // Of Length "length" half 1 half 0
{
	srand(time(NULL));
	
	for(int i=0;i<=length;i++)
		array[i]=0;

	int rand_gen=0;

	for(int i=1;i<=(length/2);i++)
	{
		rand_gen = rand() % length;
		rand_gen += 1;

		while(array[rand_gen]==1)
		{
			rand_gen = rand() % length;
			rand_gen += 1;
		}

		if(array[rand_gen]!=1)
			array[rand_gen]=1;
		else
			i--;
	}
	return;
}

int main()
{
	int length = 10;
	int array[length+1];
	random_generate_array(array,length);

	int count = 0;
	for(int i=1;i<=length;i++)
		if(array[i]==1)
			count++;

	if(count!=(length/2))
		cout<<count<<endl;
	else 
		cout<<"Cool"<<endl;

	cout<<binary_to_dec(array,length)<<endl;

	for(int i=1;i<=length;i++)
		cout<<array[i]<<" ";

	return 0;
}

/* FOR RANDOM NUMBER RANGE
int main()
{

	srand(time(NULL));
	int highest = 0;
	int lowest = 1000;
	int rand_gen = 0;
	int length = 40;
	int add = 1;

    for(int i=1;i<1000000;i++)
    {
    	rand_gen = rand() % length;
    	rand_gen += add;

    	if(rand_gen<lowest)
    		lowest = rand_gen;
    	if(rand_gen>highest)
    		highest = rand_gen;
    }

    cout<<highest<<" -- "<<lowest<<endl;

	return 0;
}
*/