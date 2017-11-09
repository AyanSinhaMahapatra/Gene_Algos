
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
using namespace std;

// Global Array Pointers 
	int *array;
	int *array_fitness;
	float *count_calculator;
	int *roulette_count;
	float *roulette_result;
	int *pop_array;
	int *offspring_array;

// Initializing Global Default Test Parameters 
	int small_str_len=5;
	int small_str_no=24;
	int population_size=4;
	int total_length=small_str_no*small_str_len;
	int pop_length=population_size*small_str_len;
	int mutation_probablity=0.03;
	int no_of_generations=1;

// Randomly Initialize The Input Array
void random_generate_main_array(int *array_random,int length)
{
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

//Main Function
int main()
{
	//Allocating Memory to Arrays
	array = (int*)malloc((total_length+1)*sizeof(int));
	array_fitness = (int*)malloc((small_str_no+1)*sizeof(int));
	count_calculator = (float*)malloc((small_str_no+1)*sizeof(float));
	roulette_count = (int*)malloc((small_str_no+1)*sizeof(int));
	roulette_result = (float*)malloc((population_size+1)*sizeof(float));
	pop_array = (int*)malloc((pop_length+1)*sizeof(int));
	offspring_array = (int*)malloc((pop_length+1)*sizeof(int));


	//Initializing The Main Array By Random Number Function
	random_generate_main_array(array,total_length);

	//Initial Fitness Calculation
	calculate_fitness(array,array_fitness,1,1,small_str_no);
	
	int fitness_sum = 0,temp_counting=0;


	//The Reproduction-Crossover-Mutation Loop Starts Here
	for(int loop=1;loop<=no_of_generations;loop++)
	{
		cout<<"Generation : "<<loop<<endl;

		//Formation of the Mating Pool
		for(int i=1;i<=small_str_no;i++)
			fitness_sum+= array_fitness[i];

		count_calculator[0]=0;
		for(int i=1;i<=small_str_no;i++)
		{
			temp_counting = (array_fitness[i]/fitness_sum) * population_size;
			count_calculator[i] = count_calculator[i] + temp_counting;
		}

		//Roulette Allocation
		for(int i=1;i<=population_size;i++)
		{
			roulette_result[i] = rand() * population_size;
		}
		sort(roulette_result+1,roulette_result+population_size)

		int rlt=1;
		for(int i=1;i<=small_str_no;i++)
			roulette_count[i]=0;
		for(int i=1;i<=small_str_no;i++)
		{
			if((count_calculator[i]<roulette_result[rlt])&&(roulette_result[rlt]<count_calculator[i+1]))
			{
				roulette_count[i]++;
				rlt++;
				i--;
			}
		}

		//Mating Pool Formation






	}






	return 0;
}