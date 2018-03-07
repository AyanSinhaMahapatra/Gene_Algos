#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>


	int case_cross = 1; 
	int array_length = 10;
	int crossover_lenght = 4;
	int check_array[1025];
	int level_limit = 5;
	int no_of_rec = 10;

void random_generate_array(int *array,int length);
void apply_crossover(int *array);

// All types of Crossover Reproduction Functions
void one_point_crossover(int *parent1,int *parent2,int *offspring1,int *offspring2,int length);

int main()
{
	int array[array_length+1];
	random_generate_array(array,array_length);

	recursive_trav(array,1);

	check_and_print_results(array);

}



// All types of Crossover Reproduction Functions

void one_point_crossover(int *parent1,int *parent2,int *offspring1,int *offspring2,int length)
{
	int rand_boundary = length-2 ;
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
	while(temp<=length)
	{
		offspring2[temp]=parent1[temp];
		offspring1[temp]=parent2[temp];
		temp++;
	}
}

// Other Functions 

void recursive_trav(int *array,int rec_level)
{
	if(rec_level == level_limit)
		return;

	apply_crossover(array);
	check_history(array);

	for(int i=1;i<=no_of_rec;i++)
		recursive_trav(array,rec_level+1);

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

	return sum;
}

void apply_crossover(int *array)
{
	int parents_index[4];
	int parent1[p_length];
	int parent2[p_length];
	int offspring1[p_length];
	int offspring2[p_length];

	choose_parents(parents_index);
	
	copy_parents_from_array(array,parent1,parent2,parents_index);

	switch(case_cross)
	{
		case 1 : 
				break;
		case 2 : 
				break;
	}

	copy_offsprings_to_array(array,offspring1,offspring2,parents_index);

	return;
}

void random_generate_array(int *array,int length) // Of Length "length" half 1 half 0
{
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