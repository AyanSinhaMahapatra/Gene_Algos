#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

	int array_length = 10;
	int crossover_length = 4;

void choose_parents(int *parents_index);
int random_number(int start,int end,int order);
void check_overlap(int *parents_index)
{
	int array1[crossover_length];
	int array2[crossover_length];
	for(int i=0;i<4;i++)
	{

	}
	return;
}

int main()
{
	int parents_index[4];

	for(int i=1;i<=1000000;i++)
	{
		//cout<<i<<" ";
		choose_parents(parents_index);
		if((parents_index[0]>array_length||parents_index[0]<1)||(parents_index[1]>array_length||parents_index[1]<1)||(parents_index[2]>array_length||parents_index[2]<1)||(parents_index[3]>array_length||parents_index[3]<1))
			cout<<"ERROR OUT OF BOUNDS"<<endl;

		//check_overlap(parents_index);
		//cout<<parents_index[0]<<" - "<<parents_index[1]<<" ----- "<<parents_index[2]<<" - "<<parents_index[3]<<endl;
	}

	return 0;
}

void choose_parents(int *parents_index)
{
	int temp_index = 0;

	parents_index[0] = random_number(0,0,1);
	parents_index[1] = parents_index[0] + crossover_length - 1;
	if(parents_index[1]>array_length)
		parents_index[1] = parents_index [1] % array_length;


	parents_index[2] = random_number(parents_index[1]+1,parents_index[0]-1,2);
	parents_index[3] = parents_index[2] + crossover_length - 1;
	if(parents_index[3]>array_length)
		parents_index[3] = parents_index [3] % array_length;

	return;
}

int random_number(int start,int end,int order)
{
	int rand_no = 0;

	if(order == 1)
	{
		rand_no = rand() % array_length;
		rand_no += 1;
	}
	else if(order == 2)
	{
		rand_no = rand() % (array_length - 2*crossover_length + 1);
		rand_no += 1;
		if(end == 0)
			end = array_length;
		if(start == array_length+1)
			start = 1; 
		rand_no = rand_no + start;
		if(rand_no > array_length)
			rand_no = rand_no % array_length;
	}

	return rand_no;
}