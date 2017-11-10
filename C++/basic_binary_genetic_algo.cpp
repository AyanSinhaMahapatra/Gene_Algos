
// A Basic Binary Genetic Algorithm Implementation
// Takes a Random Input 
// Fitness Function is defined as Sum of all elements in the string
// Desired output is A Binary String with all 1's
// By Ayan Sinha Mahapatra

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<vector>
#include<ctime>
using namespace std;

// Global Array Pointers 
	int *array;
	int *array_fitness;
	float *count_calculator;
	int *roulette_count;
	float *roulette_result;
	int *pop_array;
	int *offspring_array;
	int *offspring_fitness;

// Initializing Global Default Test Parameters 
	int small_str_len=5;
	int small_str_no=24;
	int population_size=4;
	int total_length=small_str_no*small_str_len;
	int pop_length=population_size*small_str_len;
	float mutation_probablity=0.01;
	int no_of_generations=30;

// Randomly Initialize The Input Array
void random_generate_main_array(int *array_random,int length)
{
	srand(time(NULL));
	for(int i=1;i<=length;i++)
	{
		array_random[i]= rand() % 2;
	}
}

//Print Function
void print_array(int *array_to_print,int length)
{
	cout<<"[ ";
	for(int i=1;i<=length;i++)
		cout<<array_to_print[i]<<" , ";
	cout<<" ]"<<endl;
}

void print_array_float(float *array_to_print,int length)
{
	cout<<"[ ";
	for(int i=1;i<=length;i++)
		cout<<array_to_print[i]<<" , ";
	cout<<" ]"<<endl;
}

//Calculate Fitness Of a Given Array And Output It to specified destination
void calculate_fitness(int *arr_source,int *arr_dest,int start_s,int start_d,int no_of_steps)
{
	int start_source_temp = start_s;
	int start_fitness_temp = start_d;

	for(int i=1;i<=no_of_steps;i++)
	{
		int total_fitness = 0;

		for(int j=0;j<small_str_len;j++)
			total_fitness += arr_source[start_source_temp+j];

		arr_dest[start_fitness_temp] = total_fitness;
		start_source_temp += small_str_len;
		start_fitness_temp ++;
	}
}

void small_str_copy(int *arr_dest,int dest_start,int *arr_src,int src_start)
{
	for(int i=0;i<small_str_len;i++)
	{
		arr_dest[dest_start+i]=arr_src[src_start+i];
	}
}

void crossover_reproduction(int *population,int *offspring)
{
	srand(time(NULL));
	int no_of_crossovers = population_size/2;
	for(int i=0;i<no_of_crossovers;i++)
	{
		int crossover_site =(rand() % 4) + 1;
		int parent_1_start = i*small_str_len*2 + 1;
		int parent_2_start = parent_1_start + small_str_len;
		int temp_crossover=parent_1_start;
		while((temp_crossover-parent_1_start)!=crossover_site)
		{
			offspring[temp_crossover]=population[temp_crossover];
			offspring[temp_crossover+small_str_len]=population[temp_crossover+small_str_len];
			temp_crossover++;
		}
		while(temp_crossover<parent_2_start)
		{
			offspring[temp_crossover]=population[temp_crossover+small_str_len];
			offspring[temp_crossover+small_str_len]=population[temp_crossover];
			temp_crossover++;
		}
	}
}

void perform_muation(int *to_mutate,int length_mutate)
{
	srand(time(NULL));
	for(int i=1;i<=length_mutate;i++)
	{
		double p = rand() / (double)RAND_MAX;
		if (p < mutation_probablity)
			to_mutate[i]= 1 -to_mutate[i];
	}
}

int find_minimum(int *array,int length)
{
	int minimum_value=array[1];
	int minimum_index=1;
	for(int i=2;i<=length;i++)
	{
		if(minimum_value>array[i])
		{
			minimum_index=i;
			minimum_value=array[i];
		}
	}
	return minimum_index;
}

//Main Function
int main()
{
	srand(time(NULL));

	//Allocating Memory to Arrays
	array = (int*)malloc((total_length+1)*sizeof(int));
	array_fitness = (int*)malloc((small_str_no+1)*sizeof(int));
	count_calculator = (float*)malloc((small_str_no+1)*sizeof(float));
	roulette_count = (int*)malloc((small_str_no+1)*sizeof(int));
	roulette_result = (float*)malloc((population_size+1)*sizeof(float));
	pop_array = (int*)malloc((pop_length+1)*sizeof(int));
	offspring_array = (int*)malloc((pop_length+1)*sizeof(int));
	offspring_fitness = (int*)malloc((population_size+1)*sizeof(int));


	//Initializing The Main Array By Random Number Function
	random_generate_main_array(array,total_length);
	//cout<<"Main Array = "<<endl;
	//	print_array(array,total_length);

	//Initial Fitness Calculation
	calculate_fitness(array,array_fitness,1,1,small_str_no);
	cout<<"Fitness Array = "<<endl;
		print_array(array_fitness,small_str_no);
	
	float fitness_sum = 0,temp_counting=0;


	//The Reproduction-Crossover-Mutation Loop Starts Here
	for(int loop=1;loop<=no_of_generations;loop++)
	{
		cout<<"Generation : "<<loop<<endl;


		fitness_sum=0;
		//Formation of the Mating Pool
		for(int i=1;i<=small_str_no;i++)
			fitness_sum+= array_fitness[i];

		cout<<"Fitness Sum At First = "<<fitness_sum<<endl;

		count_calculator[0]=0;
		for(int i=1;i<=small_str_no;i++)
		{
			temp_counting = (array_fitness[i]/fitness_sum) * population_size;
			count_calculator[i] = count_calculator[i-1] + temp_counting;
		}

		//cout<<"Count Calculator = "<<endl;
		//print_array_float(count_calculator,small_str_no);

		//Roulette Allocation
		for(int i=1;i<=population_size;i++)
		{
			roulette_result[i] = ( rand() / (double)RAND_MAX ) * population_size;
		}
		sort(roulette_result,roulette_result+population_size+1);

		//cout<<"Roullette Result = "<<endl;
		//print_array_float(roulette_result,population_size);

		int rlt=1;
		for(int i=1;i<=small_str_no;i++)
			roulette_count[i]=0;

		for(int i=0;i<=small_str_no;i++)
		{
			if((count_calculator[i]<roulette_result[rlt])&&(roulette_result[rlt]<count_calculator[i+1]))
			{
				roulette_count[i+1]++;
				rlt++;
				i--;
			}
		}

		//cout<<"Roullette Count = "<<endl;
		//print_array(roulette_count,small_str_no);

		//Mating Pool Formation

		int mating_temp=0;
		for(int i=1;i<=small_str_no;i++)
		{
			if(roulette_count[i]>=1)
			{
				small_str_copy(pop_array,(mating_temp*small_str_len +1),array,((i-1)*small_str_len +1));
				mating_temp++;
				roulette_count[i]--;
				i--;
			}
			if(mating_temp==population_size)
				break;
		}

		//cout<<"Population Array = "<<endl;
		//print_array(pop_array,pop_length);


		// Crossover 
		crossover_reproduction(pop_array,offspring_array);
		
		//cout<<"Offspring Array = "<<endl;
		//print_array(offspring_array,pop_length);

		// Mutation In The Offspring Array
		perform_muation(offspring_array,pop_length);

		//cout<<"Mutated Offspring Array = "<<endl;
		//print_array(offspring_array,pop_length);

		//Replacing Low Fitness Parents with High Fitness Offsprings  
		calculate_fitness(offspring_array,offspring_fitness,1,1,population_size);
		cout<<"Fitness of Offspring Array = "<<endl;
		print_array(offspring_fitness,population_size);

		for(int i=1;i<=population_size;i++)
		{
			int minimum_parent = find_minimum(array_fitness,small_str_no);
			//cout<<"Minimum at Position "<<minimum_parent<<endl;
			if(offspring_fitness[i]>array_fitness[minimum_parent])
			{
				small_str_copy(array,((minimum_parent-1)*small_str_len +1),offspring_array,((i-1)*small_str_len +1));
				array_fitness[minimum_parent]=offspring_fitness[i];
				//cout<<"Array Swapped"<<endl;
			}
		}

		//Fitness Calculation at after Offsprings Added
	    calculate_fitness(array,array_fitness,1,1,small_str_no);
	    cout<<"Fitness Array = "<<endl;
		print_array(array_fitness,small_str_no);

		fitness_sum=0;
		for(int i=1;i<=small_str_no;i++)
			fitness_sum+= array_fitness[i];

		cout<<"Fitness Sum At Last = "<<fitness_sum<<endl;
	}

	return 0;
}