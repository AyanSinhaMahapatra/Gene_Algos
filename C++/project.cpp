#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

//Function Declarations
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void assign_result_arrays(Eigen::VectorXd& array);
void random_generate(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
int random_number(int start,int end,int length,int order);
void random_parents(int *parents_index,int p_length);
void simple_crossover_reproduction(int *parent1,int *parent2,int *offspring1,int *offspring2,int length);
void copy_parents_from_array(Eigen::VectorXd& array,int *parent1,int *parent2,int length,int *parents_index);
void copy_offsprings_to_array(Eigen::VectorXd& array,int *offspring1,int *offspring2,int length,int *parents_index);
void print_result_new(Eigen::VectorXd& array)
{
	FILE* fp_rslt;
	fp_rslt=fopen("results_found_by_me.txt","a");
	// Printing Here
}

int main()
{
	srand(time(NULL));
	int genetic_till=100000;
	int flag_test_1 =1;
	int flag_test_2 =1;
	int result_flag=0;
	int count_2=0;

	//Load Co-efficient Matrix, Initialize Arrays and Variables
	Eigen::MatrixXd coeff_mat(116,116);
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_test(116);
	assign_coeff_mat(coeff_mat);
	int p1_length=10;
	int p1_no=2;     //Has to be Even //Constant
	int p2_length=5;
	int p2_no=2;     //Has to be Even //Constant 
	int cost_prev=0,cost_after=0;

	//Testing Already Found Results
	//assign_result_arrays(array);
	//cout<<"Cost = "<<cost_full(coeff_mat,array)<<endl;
	//flag=is_okay(array);

//Main Loop Starts
while(flag_test_1) //Remove these Comments In Production	
{
//               Random Generate Array Following Guidelines
	random_generate(array);
	array_test=array;
	count_2=0;
	flag_test_2=1;
	int lowest_cost=cost_full(coeff_mat,array);
	while(flag_test_2)
	{
// Part 1         
		count_2++;
//				  Randomly Select 2 Parents of Length 10 ( Except 1,115,94 ) (Ring GA)
		int p1_parents_index[4];
		random_parents(p1_parents_index,p1_length);
		array_test=array;
//                Copy To Small Parent Arrays
		int p1_parent1[p1_length];
		int p1_parent2[p1_length];
		copy_parents_from_array(array,p1_parent1,p1_parent2,p1_length,p1_parents_index);
//                Make Crossover Pairs ( 1 from front 1 from Back )
		int p1_offspring1[p1_length];
		int p1_offspring2[p1_length];
		simple_crossover_reproduction(p1_parent1,p1_parent2,p1_offspring1,p1_offspring2,p1_length);
//				  Copy Offspring Arrays to Test Array
		copy_offsprings_to_array(array_test,p1_offspring1,p1_offspring2,p1_length,p1_parents_index);
// 				  Mutation Step (Brute Force Checking)
//                Check Cost of Test Array (#ToDo Enhancement - Of only That Fraction) 
		cost_prev=cost_full(coeff_mat,array);
		cost_after=cost_full(coeff_mat,array_test);
		if(cost_after<=cost_prev)
		{
			if(is_okay(array_test))
				array=array_test;
			else{
				cout<<"Problem In Array : Fatal Error"<<endl;
				return 0;
			}
			//cout<<"Less Cost"<<endl;
		}
		else{
			array_test=array;
		}
//Part 2          
//				  Randomly Select 2 Parents of Length 5 ( Except 1,115,94 ) (Ring GA)
		int p2_parents_index[4];
		random_parents(p2_parents_index,p2_length);
//                Copy To Small Parent Arrays
		int p2_parent1[p2_length];
		int p2_parent2[p2_length];
		copy_parents_from_array(array,p2_parent1,p2_parent2,p2_length,p2_parents_index);
//                Make Crossover Pairs ( 1 from front 1 from Back )
		int p2_offspring1[p2_length];
		int p2_offspring2[p2_length];
		simple_crossover_reproduction(p2_parent1,p2_parent2,p2_offspring1,p2_offspring2,p2_length);
//				  Copy Offspring Arrays to Test Array
		copy_offsprings_to_array(array_test,p2_offspring1,p2_offspring2,p2_length,p2_parents_index);
// 				  Mutation Step (Brute Force Checking)
//                Check Cost (#ToDo Enhancement - Of only That Fraction)
		cost_prev=cost_full(coeff_mat,array);
		cost_after=cost_full(coeff_mat,array_test);
		
		if(cost_after<=cost_prev)
		{
			if(is_okay(array_test))
				array=array_test;
			else{
				cout<<"Problem In Array : Fatal Error"<<endl;
				return 0;
			}
			//cout<<"Less Cost"<<endl;
		}
		else{
			array_test=array;
			cost_after=cost_prev;
		}

		//Print Into Screen If Update In Cost Or At Certain Values of Generations
		if((cost_after<lowest_cost)||((count_2%(genetic_till/10))==0))
		{
			cout<<"Cost at Generation "<<count_2<<" = "<<cost_after<<endl; //Implement If Not Same As Before Then Print
			lowest_cost=cost_after;
		}

// Check Part     Check IF Cost Decreased in last 10,000 Iteraations 
		if((count_2%genetic_till)==0)
		{
			if(lowest_cost==cost_after){
				if(cost_after==0)
				{
					cout<<"--------------------------------One Result Found-------------------------------"<<endl;
					print_result_new(array);
					cout<<endl<<array<<endl;
					flag_test_2=0;
					result_flag=1;
					return 0;
				}
				else{
					cout<<"--------------------------------Stuck Somewhere-------------------------------"<<endl;
					cout<<"Cost = "<<cost_after<<endl<<endl<<"-New Random Run-"<<endl;
					flag_test_2=0;
				}
			}
		}
//                If Yes, Continue
//                If No, {     Check If It is A result ,
//                    If yes, Print Message AND Print Result to File
//                    If No , Continue Main Loop ( Generating another Array Randomly ) 
	}
//
    if(result_flag)
		flag_test_1=0;
}  //Main Loop Ends
	return 0;
}

void simple_crossover_reproduction(int *parent1,int *parent2,int *offspring1,int *offspring2,int length)
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

void copy_parents_from_array(Eigen::VectorXd& array,int *parent1,int *parent2,int length,int *parents_index)
{
	int temp=parents_index[0];
	int i=0;
	while(temp!=parents_index[1]+1)
	{
		if(temp==94)
			temp++;
		if(temp==115)
			temp=2;
		parent1[i]=array(temp);
		temp++;
		i++;
	}
	temp=parents_index[2];
	i=0;
	while(temp!=parents_index[3]+1)
	{
		if(temp==94)
			temp++;
		if(temp==115)
			temp=2;
		parent2[i]=array(temp);
		temp++;
		i++;
	}
}
void copy_offsprings_to_array(Eigen::VectorXd& array_test,int *offspring1,int *offspring2,int length,int *parents_index)
{
	int temp=parents_index[0];
	int i=0;
	while(temp!=(parents_index[1]+1))
	{
		if(temp==94)
			temp++;
		if(temp==115)
			temp=2;
		array_test(temp)=offspring1[i];
		temp++;
		i++;
	}
	temp=parents_index[2];
	i=0;
	while(temp!=(parents_index[3]+1))
	{
		if(temp==94)
			temp++;
		if(temp==115)
			temp=2;
		array_test(temp)=offspring2[i];
		temp++;
		i++;
	}
}

int random_number(int start,int end,int length,int order)
{
	//srand(time(NULL));
	int rand_no =0;
	int rand_range =0;

	if(start>end)
		rand_range = (115-start) + (end-2);
	else
		rand_range = end-start;
	if(order==2)
		rand_range = rand_range - length ;

	rand_no = rand() % rand_range;
	rand_no = rand_no + start;	

	if(rand_no==94)
		rand_no++;
	if(rand_no>114)
		rand_no = 1 + rand_no % 114;

	return rand_no;
}

void random_parents(int *parents_index,int p_length)
{
	//Parent 1
	int p1_start = random_number(2,114,p_length,1);
	if(p1_start==94)
		p1_start++;
	int p1_end = p1_start + p_length - 1;
	if(p1_end>=94&&p1_start<94)
		p1_end++;
	if(p1_end>114)
		p1_end = p1_end % 114+1;
	//Parent 2
	int p2_start = random_number(p1_end+1,p1_start-1,p_length,2);
	if(p2_start==94)
		p2_start++;
	int p2_end = p2_start + p_length - 1;
	if(p2_end>=94&&p2_start<94)
		p2_end++;
	if(p2_end>114)
		p2_end = p2_end % 114 + 1;

	parents_index[0] = p1_start;
	parents_index[1] = p1_end;
	parents_index[2] = p2_start;
	parents_index[3] = p2_end;
}

int is_okay(Eigen::VectorXd& array)  //Returns 1 if okay 0 if not okay
{
	int flag=1,total_ones=0;
	if(array(1)!=0)
		flag=0;
	if(array(115) != 1 - array(94))
		flag=0;
	for(int i=2;i<=114;i++)
	{
		if(i==94)
			i++;
		if(int(array(i))==1)
			total_ones++;
	}
	if(total_ones!=56)
		flag=0;
	return flag;
}

void random_generate(Eigen::VectorXd& array) //Generates A Random Array which obeys all the specifications
{
	srand(time(NULL));
	array.fill(0);
	int rand_gen=0;
	for(int i=0;i<56;i++)
	{
		rand_gen = rand() % 112;
		rand_gen +=2;
		if(rand_gen==94)
		{
			do{
				rand_gen++;
				if(rand_gen==115)
					rand_gen=2;
			}
			while(array(rand_gen)==1);
		}
		while(array(rand_gen)==1)
		{
			rand_gen++;
			if(rand_gen==115)
				rand_gen=2;
			if(rand_gen==94)
				rand_gen++;
		}
		if(array(rand_gen)!=1)
			array(rand_gen)=1;
		else
			i--;
	}
	array(94)= int(rand() % 2);
	array(115)= 1 - array(94);
}

int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array)
{
	Eigen::VectorXd array_temp(116);
	array_temp = coeff_mat * array;
	//cout<<array_temp;
	for(int i=1;i<=115;i++)
	{
		if((array_temp(i)>=57)&&(array_temp(i)<=72))
			array_temp(i)=0;
		else if(array_temp(i)<57)
			array_temp(i)=(57-int(array_temp(i)))^2;
		else if(array_temp(i)>72)
			array_temp(i)=(int(array_temp(i))-72)^2;
	}
	return array_temp.sum();
}

void assign_result_arrays(Eigen::VectorXd& array)
{
	array.fill(0);
	FILE* fp;
	fp = fopen("already_Found_results.txt","r");
	int count=0;
    char ch;
    int flag = 1;
    int columns=1;
    int rows=1;
    while(flag)
    {
    	ch=fgetc(fp);
    	if(ch==EOF||rows==2)
    	{
    		flag=0;
    		continue;
    	}
    	else if(ch!='\n')
    	{
    		int temp;
    		temp=ch;
    		temp-=48;
    		array(columns)=temp;
    	}
    	count++;
    	columns++;
    	if(columns==116)
    	{
    		ch=fgetc(fp);
    		columns=1;
    		rows++;
    	}
    }
}

// Opening The File Containing The Co-efficient Matrix
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat)
{
	FILE* fp;
	fp = fopen("Coefficient Matrix.txt","r");
	int count=0;
    char ch;
    int flag = 1;
    int rows=1,columns=1;
    while(flag)
    {
    	ch=fgetc(fp);
    	if(ch==EOF||rows==116)
    	{
    		flag=0;
    		continue;
    	}
    	if(ch=='v')
    		coeff_mat(rows,columns)=21;
    	else if(ch=='u')
    		coeff_mat(rows,columns)=14;
    	else if(ch!='\n')
    	{
    		int temp;
    		temp=ch;
    		temp-=48;
    		coeff_mat(rows,columns)=temp;
    	}
    	count++;
    	columns++;
    	if(columns==116)
    	{
    		ch=fgetc(fp);
    		columns=1;
    		rows++;
    	}
    }
    //cout<<"Number of Characters == "<<count<<endl;
}