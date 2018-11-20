#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

int array_length = 20;
int crossover_length = 8;
int k_point;

void choose_parents(int *parents_index);
int random_number(int start,int end,int order);
void copy_parents_from_array(int *array,int *parent1,int *parent2,int *parents_index);
void copy_offsprings_to_array(int *array,int *offspring1,int *offspring2,int *parents_index);
void random_generate_array(int *array);
void print_array(int *array);
void check_array_integrity(int *array);

void k_point_crossover(int *parent1,int *parent2,int *offspring1,int *offspring2)
{
	k_point = 4;
	int crossover[k_point+1];
	float crossover_float[k_point+1];
	float diff = crossover_length/k_point;
	//cout<<diff<<endl;
	int temp=1,cross;
	int start,end,rand_var,pointer,flag=0,flag2=0;
	crossover_float[1] = diff; 
	crossover[1] = diff;

	for(int i=2;i<=k_point;i++)
	{
		crossover_float[i]=crossover_float[i-1]+diff;
		crossover[i]=crossover_float[i];
		//cout<<crossover_float[i]<<" "<<crossover[i]<<endl;
	}

	for(int i=1;i<=k_point;i++)
	{
		if(i==1)
			start = 1;
		else
			start = crossover[i-1]+1;

		if(i==k_point)
			end = crossover_length;
		else
			end = crossover[i+1];

		if((end-start)!=0)
			rand_var = rand() % (end-start);
		else
			rand_var = 0 ; 
		rand_var+=start;

		crossover[i] = rand_var;
		cout<<crossover[i]<<" ";
	}
	cout<<endl;

	pointer = 1;
	cross = crossover[pointer];
	
	for(int i=1;i<=k_point+1;i++)
	{
		while(temp<=crossover_length)
		{
			if(temp==cross)
				flag2 = 1;
			if(flag == 0)
			{
				offspring1[temp]=parent1[temp];
				offspring2[temp]=parent2[temp];
				temp++;
			}
			else
			{
				offspring2[temp]=parent1[temp];
				offspring1[temp]=parent2[temp];
				temp++;
			}
			cout<<endl<<"Temp == "<<temp;
			if(flag2==1)
			{
				flag2 = 0;
				break;
			}
		}
		if((temp==cross)&&(temp!=k_point))
		{
			pointer++;
			cross = crossover[pointer];
			cout<<endl<<" Pointer == "<<pointer<<endl;
			flag = 1 - flag;
		}
	}
	return;
}

int main()
{


	int array[array_length+1];
	int parents_index[4];
	int parent1[crossover_length];
	int parent2[crossover_length];
	int offspring1[crossover_length];
	int offspring2[crossover_length];

	random_generate_array(array);
	print_array(array);

	choose_parents(parents_index);
	//cout<<parents_index[0]<<" "<<parents_index[1]<<" "<<parents_index[2]<<" "<<parents_index[3]<<endl;
	
	copy_parents_from_array(array,parent1,parent2,parents_index);

		//one_point_crossover(parent1,parent2,offspring1,offspring2);

		k_point_crossover(parent1,parent2,offspring1,offspring2);

		//shuffle_crossover(parent1,parent2,offspring1,offspring2);

		//uniform_crossover(parent1,parent2,offspring1,offspring2);

	copy_offsprings_to_array(array,offspring1,offspring2,parents_index);
	print_array(array);

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
		rand_no = rand() % (array_length - 2*crossover_length);
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

void copy_parents_from_array(int *array,int *parent1,int *parent2,int *parents_index)
{
	int temp=parents_index[0];
	int i=0;
	while(temp!=parents_index[1]+1)
	{
		if(temp==array_length+1)
			temp=1;
		parent1[i]=array[temp];
		temp++;
		i++;
	}
	temp=parents_index[2];
	i=0;
	while(temp!=parents_index[3]+1)
	{
		if(temp==array_length+1)
			temp=1;
		parent2[i]=array[temp];
		temp++;
		i++;
	}
	return;
}

void copy_offsprings_to_array(int *array,int *offspring1,int *offspring2,int *parents_index)
{
	int temp=parents_index[0];
	int i=0;
	while(temp!=(parents_index[1]+1))
	{
		if(temp==array_length+1)
			temp=1;
		array[temp]=offspring1[i];
		temp++;
		i++;
	}
	temp=parents_index[2];
	i=0;
	while(temp!=(parents_index[3]+1))
	{
		if(temp==array_length+1)
			temp=1;
		array[temp]=offspring2[i];
		temp++;
		i++;
	}
	return;
}

void random_generate_array(int *array) // Of Length "length" half 1 half 0
{
	int length = array_length;
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

void print_array(int *array)
{
	cout<<endl;
	for(int i=1;i<=array_length;i++)
		cout<<array[i]<<" ";
	cout<<endl;

	return;
}

void check_array_integrity(int *array)
{
	int count = 0;
	for(int i=1;i<=array_length;i++)
		if(array[i]==1)
			count++;

	if(count != array_length/2)
		cout<<"Integrity Error"<<endl;
	return;
}