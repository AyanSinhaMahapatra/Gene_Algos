// A Basic Binary Genetic Algorithm Implementation
// Takes a Random Input 
// Fitness Function is defined as Sum of all elements in the string
// Desired output is A Binary String with all 1's
// By Ayan Sinha Mahapatra
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;
// Global Array Pointers 
	int *array;
	int *array_fitness;
	float *count_calculator;
	int *roulette_count;
	int *fitness_temp;
	float *roulette_result;
	int *pop_array;
	int *offspring_array;
	int *offspring_fitness;
// Initializing Global Default Test Parameters 
	int small_str_len=20;
	int small_str_no=240;
	int population_size=40;
	int total_length=small_str_no*small_str_len;
	int pop_length=population_size*small_str_len;
	float mutation_probablity=0.01;
	int no_of_generations=500;
	char condition='n';
// Randomly Initialize The Input Array
void random_generate_main_array(int *array_random,int length);
//Print Functions
void print_array(int *array_to_print,int length);
void print_array_float(float *array_to_print,int length);
//Calculate Fitness Of a Given Array And Output It to specified destination
void calculate_fitness(int *arr_source,int *arr_dest,int start_s,int start_d,int no_of_steps);
//Copying One Smallest Unit Of The String
void small_str_copy(int *arr_dest,int dest_start,int *arr_src,int src_start);
//Offspring Fromation By Crossover From Parents Population
void crossover_reproduction(int *population,int *offspring);
//Mutation Function
void perform_muation(int *to_mutate,int length_mutate);
int find_minimum(int *array,int length);
int find_maximum(int *array,int length);
// Mating Pool Formation Functions 
void mating_pool_form_roulette(int fitness_sum);
void mating_pool_form_max();
//Total Fitness Calculation In a Generation
int calcu_fitness_sum();
void alloc_memory();
void take_input();

//Main Function
int main()
{
	char choice;
	cout<<"To Follow Default Confg. Press 'n' else Press 'y' "<<endl;
	cin>>choice;

	if(choice=='y')
		take_input();

	//Allocating Memory to Arrays
	alloc_memory();

	//Initializing The Main Array By Random Number Function
	random_generate_main_array(array,total_length);

	//The Reproduction-Crossover-Mutation Loop Starts Here
	float fitness_sum = 0,temp_counting=0; 
	for(int loop=1;loop<=no_of_generations;loop++)
	{
		cout<<"Generation : "<<loop<<endl;
		fitness_sum=calcu_fitness_sum();
		cout<<"Fitness Sum At First = "<<fitness_sum<<endl;

		//Formation of the Mating Pool
		if(condition =='y')
			mating_pool_form_roulette(fitness_sum);
		else if(condition =='n')
			mating_pool_form_max();

		// Crossover 
		crossover_reproduction(pop_array,offspring_array);

		// Mutation In The Offspring Array
		perform_muation(offspring_array,pop_length);

		//Replacing Low Fitness Parents with High Fitness Offsprings  
		calculate_fitness(offspring_array,offspring_fitness,1,1,population_size);

		//Putting Offsprings in Main Array
		for(int i=1;i<=population_size;i++)
		{
			int minimum_parent = find_minimum(array_fitness,small_str_no);
			if(offspring_fitness[i]>array_fitness[minimum_parent])
			{
				small_str_copy(array,((minimum_parent-1)*small_str_len +1),offspring_array,((i-1)*small_str_len +1));
				array_fitness[minimum_parent]=offspring_fitness[i];
			}
		}

		cout<<"Fitness Sum At Last = "<<calcu_fitness_sum()<<endl;
	}
	return 0;
}
void take_input()
{
	cout<<"Input Length Of One Gene (Smallest String) "<<endl;
	cin>>small_str_len;
	cout<<"Input Total No of Genes in Main String "<<endl;
	cin>>small_str_no;
	cout<<"Input Population Size"<<endl;
	cin>>population_size;
}
void random_generate_main_array(int *array_random,int length)
{
	srand(time(NULL));
	for(int i=1;i<=length;i++)
	{
		array_random[i]= rand() % 2;
	}
}
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
void calculate_fitness(int *arr_source,int *arr_dest,int start_s,int start_d,int no_of_steps)
{
	int start_source_temp = start_s;
	int start_fitness_temp = start_d;
	for(int i=1;i<=no_of_steps;i++)
	{
		int total_fitness = 0;
		for(int j=0;j<small_str_len;j++)
			total_fitness += arr_source[start_source_temp+j];
		arr_dest[start_fitness_temp] = total_fitness; //Edits
		start_source_temp += small_str_len;
		start_fitness_temp ++;
	}
}
void small_str_copy(int *arr_dest,int dest_start,int *arr_src,int src_start)
{
	for(int i=0;i<small_str_len;i++)
		arr_dest[dest_start+i]=arr_src[src_start+i];
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
int find_maximum(int *array,int length)
{
	int maximum_value=array[1];
	int maximum_index=1;
	for(int i=2;i<=length;i++)
	{
		if(maximum_value<array[i])
		{
			maximum_index=i;
			maximum_value=array[i];
		}
	}
	return maximum_index;
}
void mating_pool_form_roulette(int fitness_sum)
{
	srand(time(NULL));
	float temp_counting;
	//cout<<"Fitness Sum = "<<fitness_sum<<endl;
	//cout<<"Fitness Array = "<<endl;
	//		print_array(array_fitness,small_str_no);
	count_calculator[0]=0;
	for(int i=1;i<=small_str_no;i++)
	{
		temp_counting = float(array_fitness[i] * population_size)/fitness_sum;
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
	//	print_array_float(roulette_result,population_size);
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
	//	print_array(roulette_count,small_str_no);
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
}
void mating_pool_form_max()
{
	for(int i=1;i<=small_str_no;i++)
	{
		fitness_temp[i]=array_fitness[i];
	}
	int pop_index = 1;
	for(int i=1;i<=population_size;i++)
	{
		int max_index=find_maximum(fitness_temp,small_str_no);
		small_str_copy(pop_array,pop_index,array,((max_index-1)*small_str_len +1 ));
		fitness_temp[max_index]=0;
		pop_index += small_str_len;
	}	
}
int calcu_fitness_sum()
{
	calculate_fitness(array,array_fitness,1,1,small_str_no);
	int fitness_sum=0;
	for(int i=1;i<=small_str_no;i++)
		fitness_sum+= array_fitness[i];
	return fitness_sum;
}
void alloc_memory()
{
	array = (int*)malloc((total_length+1)*sizeof(int));
	array_fitness = (int*)malloc((small_str_no+1)*sizeof(int));
	count_calculator = (float*)malloc((small_str_no+1)*sizeof(float));
	roulette_count = (int*)malloc((small_str_no+1)*sizeof(int));
	fitness_temp = (int*)malloc((small_str_no+1)*sizeof(int));
	roulette_result = (float*)malloc((population_size+1)*sizeof(float));
	pop_array = (int*)malloc((pop_length+1)*sizeof(int));
	offspring_array = (int*)malloc((pop_length+1)*sizeof(int));
	offspring_fitness = (int*)malloc((population_size+1)*sizeof(int));
}