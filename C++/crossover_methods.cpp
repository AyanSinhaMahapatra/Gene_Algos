#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

//Implemented Crossovers and Their Case Numbers
/*
	Case 1 : One Point Crossover
*/

    int case_cross = 1;   //Change this for change in Crossover Technique
	int array_length = 10;
	int crossover_length = 4;
	int possible_outcomes = 210;
	int check_array[1025];
	int level_limit = 5;
	int no_of_rec = 10;
	int result[2];

void apply_crossover(int *array);
void init_check_array();
void check_and_print_results(int *array);
void recursive_trav(int *array,int rec_level);
void check_history(int *array);
int binary_to_dec(int *array);
void choose_parents(int *parents_index);
int random_number(int start,int end,int order);
void copy_parents_from_array(int *array,int *parent1,int *parent2,int *parents_index);
void copy_offsprings_to_array(int *array,int *offspring1,int *offspring2,int *parents_index);
void random_generate_array(int *array);
void check_array_integrity(int *array);

// All types of Crossover Reproduction Functions
void one_point_crossover(int *parent1,int *parent2,int *offspring1,int *offspring2);

int main()
{
	int array[array_length+1];
	double total_results=0;
	random_generate_array(array);

	for(int i=1;i<=10000;i++)
	{
		init_check_array();
		recursive_trav(array,1);
		check_and_print_results(check_array);
		total_results+=result[0];
	}

	total_results /= 10000;
	cout<<total_results<<endl;
	


	return 0;
}



// All types of Crossover Reproduction Functions

void one_point_crossover(int *parent1,int *parent2,int *offspring1,int *offspring2)
{
	int rand_boundary = crossover_length-2 ;
	//Left To Right
	int rand_var = rand() % rand_boundary;
	rand_var+=2;
	//cout<<"Crossover at - "<<rand_var<<endl;
	int temp = 0;
	while(temp<=rand_var)
	{
		offspring1[temp]=parent1[temp];
		offspring2[temp]=parent2[temp];
		temp++;
	}
	while(temp<=crossover_length)
	{
		offspring2[temp]=parent1[temp];
		offspring1[temp]=parent2[temp];
		temp++;
	}
}

// Other Functions 

void check_and_print_results(int *check_array)
{
	int count_visited = 0;
	int count_total = 0;

	for(int i=1;i<=1024;i++)
	{
		if(check_array[i]!=0)
		{
			count_visited++;
			count_total += check_array[i];
		}
	}

	result[0]=count_visited;
	result[1]=count_total;
	return;
}

void recursive_trav(int *array,int rec_level)
{
	if(rec_level == level_limit)
		return;

	apply_crossover(array);
	check_history(array);

	for(int i=1;i<=no_of_rec;i++)
	{
		//cout<<rec_level<<"  --  "<<i<<endl;
		recursive_trav(array,rec_level+1);
	}
	return;
}

void check_history(int *array)
{
	int number = 0;
	number = binary_to_dec(array);
	
	check_array[number]++;

	return;
}

int binary_to_dec(int *array)
{
	int sum = 0;

	for(int i=array_length;i>0;i--)
		sum += array[array_length - i + 1]*pow(2,i-1) ;

	return sum;
}

void apply_crossover(int *array)
{
	int parents_index[4];
	int parent1[crossover_length];
	int parent2[crossover_length];
	int offspring1[crossover_length];
	int offspring2[crossover_length];

	choose_parents(parents_index);
	
	copy_parents_from_array(array,parent1,parent2,parents_index);

	switch(case_cross)
	{
		case 1 : one_point_crossover(parent1,parent2,offspring1,offspring2);
				break;
		case 2 : one_point_crossover(parent1,parent2,offspring1,offspring2);
				break;
	}

	copy_offsprings_to_array(array,offspring1,offspring2,parents_index);

	return;
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

void init_check_array()
{
	for(int i=0;i<=1024;i++)
		check_array[i]=0;
	return;
}

void random_generate_array(int *array) // Of Length "length" half 1 half 0
{
	int length = array_length;
	srand(time(NULL));
	
	for(int i=0;i<=length;i++)
		array[i]=0;
	for(int i=0;i<=1024;i++)
		check_array[i]=0;

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