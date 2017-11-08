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

// Global Variables 
int *array;
int *array_fitness;
int *pop_array;
int *offspring_array;

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

//Main Function
int main()
{
	int small_str_len=5;
	int small_str_no=24;
	int population_size=4;
	int total_length=small_str_no*small_str_len;
	int pop_length=population_size*small_str_len;
	int mutation_probablity=0.03;
	int no_of_generations=100;
	
	//Intializing array[total_length+1];
	array=(int*)malloc((total_length+1)*sizeof(int));

	//Initializing array_fitness[small_str_no+1];
	array_fitness=(int*)malloc((small_str_no+1)*sizeof(int));

	//Initializing pop_array[pop_length+1];
	pop_array=(int*)malloc((pop_length+1)*sizeof(int));


	//Initializing offspring_array[pop_length+1];
	offspring_array=(int*)malloc((pop_length+1)*sizeof(int));

	//Initializing The Main Array By Random Number Function
	random_generate_main_array(array,total_length);
	print_array(array,total_length);







	return 0;
}