#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

	//Hyperparameters Initialization

	int genetic_till=10000; 
	//int swapper_gen=0;
	//int flag_test_1 =1;
	int flag_test_2 =1;
	//int result_flag=0;
	int count_2=0;
	int flag_swap=1;
	int lowest_cost_whole=100;
	int random_run_no=1;
	//float changes_total=0;
	float mutation_probablity=0; 
	float mutation_total=0;
	int p1_length=30;
	int p1_no=2;     //Has to be Even //Constant
	int p2_length=10;
	int p2_no=2;     //Has to be Even //Constant 
	int p3_length=20;
	int cost_prev=0,cost_after=0;
	int cost_before = 0;
	int lowest_cost=100; //Value Doesn't Matter
	int max_shake = 10;
	int max_trench = 50;
	int max_neigh = 30;
	int shake_no = 1;
	int trench_no=1; 
	int neighbourhood_no = 20;
	int lowest_differance = 115;

//Function Declarations
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void assign_result_arrays(Eigen::VectorXd& array);
void random_generate(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
void print_matrix(Eigen::MatrixXd& mat);
int random_number(int start,int end,int length,int order);
void random_parents(int *parents_index,int p_length);
void simple_crossover_reproduction(int *parent1,int *parent2,int *offspring1,int *offspring2,int length);
void copy_parents_from_array(Eigen::VectorXd& array,int *parent1,int *parent2,int length,int *parents_index);
void copy_offsprings_to_array(Eigen::VectorXd& array,int *offspring1,int *offspring2,int length,int *parents_index);
void print_result_new(Eigen::VectorXd& array);
void print_result_lowest(int cost,Eigen::VectorXd& array);
int perform_swap(Eigen::VectorXd& array,Eigen::VectorXd& array_test);
int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test);
void genetic_algorithm(int p_length,Eigen::VectorXd& array,Eigen::VectorXd& array_test,Eigen::MatrixXd& coeff_mat);
void check_and_swap(Eigen::VectorXd& array,Eigen::VectorXd& array_test,Eigen::MatrixXd& coeff_mat);
int mutate_array(Eigen::VectorXd& array,float mutation_probablity,Eigen::MatrixXd& coeff_mat);
int check_array(int rand_gen,int *array_random,int size);
void shake(Eigen::VectorXd& array_main,Eigen::VectorXd& array,int k_neighbourhood);
void print_vns(int flag_random,int flag_neigh,int flag_tren,int flag_shake,int random_run_no,int neighbourhood_no,int trench_no,
	int shake_no,int cost_trench,int cost_shake);
void swap_fixed_pos(Eigen::VectorXd& array);
void try_swap_fixed(Eigen::VectorXd& array,Eigen::MatrixXd& coeff_mat);
void assign_results_new(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& results_old_mat);
void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results);

int main()
{
	srand(time(NULL));

	//Load Co-efficient Matrix, Initialize Arrays and Variables
	Eigen::MatrixXd coeff_mat(116,116);
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_main(116);
    Eigen::VectorXd array_test(116);
    Eigen::VectorXd array_temp(116);
	assign_coeff_mat(coeff_mat);

    //Old Results
    Eigen::MatrixXd results_old(116,116); 
    assign_results_old(results_old);

    Eigen::MatrixXd results_new(116,116);     // New Results
    assign_results_new(results_new);

    //array_main = results_new.row(3);
    //shake(array_main,array,5);
    //array_main=array;
	random_generate(array_main);
	array=array_main;

	int cost_main = cost_full(coeff_mat,array_main);
	int cost_vns;
	//print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,0,cost_main);

	while(cost_main!=0)
	{
		shake(array_main,array,neighbourhood_no);  // Shaked To K'th Neighbourhood
		lowest_cost = cost_full(coeff_mat,array);
		//cout<<"Cost Before Shaking == "<<cost_main<<endl;
		//cout<<"Cost After Shaking == "<<lowest_cost<<endl;

		// Local Search Starts Here
		flag_test_2=1;
		count_2=0;
		while(flag_test_2)
		{
			count_2++;

			array_temp = array;
			cost_before=cost_full(coeff_mat,array);

			genetic_algorithm(p1_length,array,array_test,coeff_mat);
			genetic_algorithm(p2_length,array,array_test,coeff_mat);
			genetic_algorithm(p3_length,array,array_test,coeff_mat);

			try_swap_fixed(array,coeff_mat);
			cost_after=cost_full(coeff_mat,array);

			if((cost_after<lowest_cost)||((count_2%(genetic_till/10))==0))
			{
				//cout<<"Cost at Generation "<<count_2<<" = "<<cost_after<<endl; //Implement If Not Same As Before Then Print
				lowest_cost=cost_after;
			}

			if(cost_after>cost_before)
				array=array_temp;

			if((count_2%genetic_till)==0)
				check_and_swap(array,array_test,coeff_mat);
		}
		// Till Here Local Search 
		if(is_okay(array)==0)
		{
			cout<<"Problem In Array : Fatal Error"<<endl;
			return 0;
		}

		cost_vns = cost_full(coeff_mat,array);
		//cout<<"Cost After Local Search : "<<cost_vns<<endl;

		if(cost_vns<cost_main)
		{
			//print_vns(0,0,0,1,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
			array_main = array;
			cost_main = cost_vns;
			trench_no++;
			shake_no = 1;
			//print_vns(0,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
			print_result_lowest(cost_main,array_main);
		}
		else{	
			if(cost_vns<13)
				print_result_lowest(cost_vns,array);
			try_difference(array,results_old);   //Fill Here
			//cout<<"Numbers = "<<random_run_no<<" "<<neighbourhood_no<<" "<<trench_no<<" "<<shake_no<<"Cost == "<<cost_vns<<endl;
			shake_no++;
		}

		if(shake_no>=max_shake) //max_shake = 20
		{
			//shake(array_main,array,neighbourhood_no);  // Shaked To K'th Neighbourhood
			lowest_cost = cost_full(coeff_mat,array);
			trench_no++;
			shake_no = 1;
			//print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
		}

		if(trench_no>=max_trench) //max_trench = 10
		{
			neighbourhood_no++;
			trench_no = 1;
			shake_no = 1;
			//print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,cost_main,cost_vns);
		}

		if(neighbourhood_no>=max_neigh) //max_neigh = 20
		{
			neighbourhood_no = 1;
			trench_no = 1;
			shake_no = 1; 
			random_run_no++;
			//array_main = results_new.row(random_run_no);
			random_generate(array_main);
			cost_main = cost_full(coeff_mat,array_main);
			//print_vns(1,1,1,0,random_run_no,neighbourhood_no,trench_no,shake_no,0,cost_main);
		}
	}

	print_result_new(array_main);
	print_vns(0,0,0,0,0,0,0,0,0,0);
	cout<<endl<<array_main<<endl;
	getchar();
	return 0;
}

void genetic_algorithm(int p_length,Eigen::VectorXd& array,Eigen::VectorXd& array_test,Eigen::MatrixXd& coeff_mat) //array,test_array
{
		int p_parents_index[4];
		random_parents(p_parents_index,p_length);
		array_test=array;
//                Copy To Small Parent Arrays
		int p_parent1[p_length];
		int p_parent2[p_length];
		copy_parents_from_array(array,p_parent1,p_parent2,p_length,p_parents_index);
//                Make Crossover Pairs ( 1 from front 1 from Back )
		int p_offspring1[p_length];
		int p_offspring2[p_length];
		simple_crossover_reproduction(p_parent1,p_parent2,p_offspring1,p_offspring2,p_length);
//				  Copy Offspring Arrays to Test Array
		copy_offsprings_to_array(array_test,p_offspring1,p_offspring2,p_length,p_parents_index);
// 				  Mutation Step (Brute Force Checking)
//                Check Cost of Test Array (#ToDo Enhancement - Of only That Fraction) 
		cost_prev=cost_full(coeff_mat,array);
		cost_after=cost_full(coeff_mat,array_test);
		//changes_total+=change_in_array(array,array_test);
		array=array_test;
		//if(((cost_prev-cost_after)>5)||((cost_after<=cost_prev)&&(cost_prev<8)))
		/*if(cost_after<=cost_prev)
		{
			if(is_okay(array_test))
				array=array_test;
			else{
				cout<<"Problem In Array : Fatal Error"<<endl;
				return;
			}
			//cout<<"Less Cost"<<endl;
		}
		else{
			array_test=array;
			cost_after=cost_prev;
		}*/

			if(is_okay(array_test)==0)
			{
				cout<<"Problem In Array : Fatal Error"<<endl;
				return;
			}


		mutation_total+=mutate_array(array,mutation_probablity,coeff_mat);
		//cout<<" Mutation Total = "<<mutation_total<<endl;
		array_test=array;
		cost_after = cost_full(coeff_mat,array_test);
		//if(lowest_cost >= cost_after)
		//{
			//cout<<"Cost at Generation (after Mutation)"<<count_2<<" = "<<cost_after<<endl;
			//lowest_cost = cost_after;
		//}
}

void check_and_swap(Eigen::VectorXd& array,Eigen::VectorXd& array_test,Eigen::MatrixXd& coeff_mat)
{
	if(lowest_cost==cost_after){
				if(cost_after==0)
				{
					cout<<"--------------------------------One Result Found-------------------------------"<<endl;
					print_result_new(array);
					cout<<endl<<array<<endl;
					flag_test_2=0;
					//result_flag=1;
					//return;
				}
				else{
					//cout<<"--------------------------------Stuck Somewhere-------------------------------"<<endl;
					//cout<<"Cost = "<<cost_after<<endl<<endl;

					// Swapper Stuff Goes Here
					//cout<<" Swapper Mode Enabled "<<endl;
					flag_swap=1;
					while((cost_after!=0)&&(flag_swap!=0))
					{
						//swapper_gen=1;
						if(perform_swap(array,array_test))
						{
							cost_after=cost_full(coeff_mat,array);   //Changed Here
							//cout<<" Swap Not Reducing Cost -- ERROR ERROR"<<endl;
							if(cost_after<=lowest_cost_whole)
							{
								lowest_cost_whole=cost_after;
								//print_result_lowest(lowest_cost_whole,array);
							}
							flag_swap=0;
							//cout<<"Mutation Average = "<<float(mutation_total/count_2)/2<<endl;
							//cout<<"Average Changes by Genetic Algo in One go == "<<(changes_total/(count_2*2))<<endl;
							//cout<<"Cost Here Is, After Swapper --  "<<cost_after<<endl;
							//cout<<"Lowest Cost Till Now Is  --  "<<lowest_cost_whole<<endl; 
						}
						cost_after=cost_full(coeff_mat,array);
						//cout<<"After Peforming "<<swapper_gen<<" of Batches of Swap, Cost is  -  "<<cost_after<<endl;
						if(cost_after==0)
						{
							cout<<"------------O Cost Achieved-------------"<<endl;
							lowest_cost_whole=0;
							print_result_new(array); //Print To File 
						}
					}
					flag_test_2=0;
				}
			}
}

int check_array(int rand_gen,int *array_random,int size)
{
	int flag = 1; 
	//cout<<"Size = "<<size<<endl;
	for(int i=1;i<=size;i++)
	{
		if(array_random[i]==rand_gen)
			flag=0;
	}
	return flag;
}

void shake(Eigen::VectorXd& array_main,Eigen::VectorXd& array,int k_neighbourhood)
{
	//cout<<"Stirred Not Shaken"<<endl;
	int array_loc1[57];
	int array_loc0[57];
	int array_random1[k_neighbourhood+1];
	int array_random0[k_neighbourhood+1];
	int pointer_0=0;
	int pointer_1=0;
	int rand_gen=0;
	for(int i=1;i<=k_neighbourhood;i++)
	{
		array_random1[i]=0;
		array_random0[i]=0;
	}

	for(int i=2;i<=114;i++)
	{
		if(i==94)
			i++;
		if(array_main(i)==0)
		{
			pointer_0++;
			array_loc0[pointer_0] = i; 
		}
		else if(array_main(i)==1)
		{
			pointer_1++;
			array_loc1[pointer_1] = i;
		}
	}
	if((pointer_0==56)&&(pointer_1==56))
	{
		//cout<<" First Task Okay "<<endl;
	}
	else
	{
		cout<<" Error : Pointers at :"<<endl;
		cout<<pointer_1<<" == "<<pointer_0<<endl;
	}


	for(int i=1;i<=k_neighbourhood;i++)
	{
		rand_gen = rand() % 56;
		rand_gen++;
		while(check_array(rand_gen,array_random1,k_neighbourhood)==0)
		{
			rand_gen = rand() % 56;
			rand_gen++;
		}
		array_random1[i]=rand_gen;
		rand_gen = rand() % 56;
		rand_gen++;
		while(check_array(rand_gen,array_random0,k_neighbourhood)==0)
		{
			rand_gen = rand() % 56;
			rand_gen++;
		}
		array_random0[i]=rand_gen;
	}

	array=array_main;

	for(int i=1;i<=k_neighbourhood;i++)
	{
		array(array_loc1[array_random1[i]])=0;
		array(array_loc0[array_random0[i]])=1;
	}

	if(trench_no%2==0)   //For Alternates Of Fixed Positions 94 and 115 
		swap_fixed_pos(array);

}
void print_vns(int flag_random,int flag_neigh,int flag_tren,int flag_shake,int random_run_no,int neighbourhood_no,int trench_no,
	int shake_no,int cost_trench,int cost_shake)
{
	FILE *fp_log;
	fp_log = fopen("vns_log_orig.txt","a"); 
	if(flag_random==1)
	{
		cout<<"***********************************************************************************"<<endl;
		cout<<"***********************************************************************************"<<endl;
		cout<<"***********************************************************************************"<<endl;
		cout<<"                              Random Run Number == "<<random_run_no<<endl;
		cout<<"Initial Cost Of Randomly Generated Array = "<<cost_shake<<endl;
		fprintf(fp_log,"                              Random Run Number == %d \n",random_run_no);
	}
	if(flag_neigh==1)
	{
		cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		cout<<"                   Neighbourhood Number == "<<neighbourhood_no<<endl;
		fprintf(fp_log,"                   Neighbourhood Number == %d \n",neighbourhood_no);
	}
	if(flag_tren==1)
	{
		cout<<"=================================================="<<endl;
		cout<<"        Trench Number =="<<trench_no<<"  Cost = "<<cost_trench<<endl;
		fprintf(fp_log,"        Trench Number == %d  Cost = %d \n",trench_no,cost_trench);
	}
	if(flag_shake==1)
	{
		
		cout<<"Shake Number ::::::::: "<<shake_no<<"   Cost = "<<cost_shake<<endl;
		fprintf(fp_log,"Shake Number ::::::::: %d  Cost = %d \n",shake_no,cost_shake);
		if(cost_shake<cost_trench)
		{
			cout<<"<<<<<<<<<<<<<<<<<<<<Lower Cost Found.>>>>>>>>>>>>>>>>>>>>>"<<endl<<" Cost == "<<cost_shake<<endl;
			fprintf(fp_log,"<<<<<<<<Lower Cost Found.>>>>>>>>>>> Cost == %d \n",cost_shake);
		}
	}
	if((flag_random==0)&&(flag_neigh==0)&&(flag_tren==0)&&(flag_shake==0))
	{
		cout<<"--------------------------------One Result Found-------------------------------"<<endl;
		fprintf(fp_log,"--------------------------------One Result Found------------------------------- \n");
	}
	fclose(fp_log);
}

void swap_fixed_pos(Eigen::VectorXd& array)
{
	array(94)=1-array(94);
    array(115)=1-array(115);
}

void try_swap_fixed(Eigen::VectorXd& array,Eigen::MatrixXd& coeff_mat)
{
	int swap_cost_1,swap_cost_2;
	swap_cost_1 = cost_full(coeff_mat,array);
	swap_fixed_pos(array);
	swap_cost_2 = cost_full(coeff_mat,array);
	if(swap_cost_1<swap_cost_2)
		swap_fixed_pos(array);
}

void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results) 
{
    FILE* fp_diff;
    fp_diff = fopen("diff_log_2.txt","a"); 
    Eigen::VectorXd result(116);
    int diff = 0;
    int temp;
    for(int i=1;i<=4;i++)
    {
       result = results.row(i); 
       diff = change_in_array(result,array); 
       if(diff<lowest_differance)
            {
                lowest_differance=diff;
                for(int j=1;j<=115;j++)
                {
                    temp = array(j);
                    fprintf(fp_diff, "%d",temp);
                }
                fprintf(fp_diff,"\n  Difference == %d  \n",lowest_differance);
            }
    }
    fclose(fp_diff);
}
void assign_results_new(Eigen::MatrixXd& coeff_mat)
{
    FILE* fp;
    fp = fopen("results_found_by_me_lowest_org.txt","r");
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
        if(ch!='\n')
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
            ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);
            ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);
            columns=1;
            rows++;
        }
    }
    //cout<<"Number of Characters == "<<count<<endl;
}

void assign_results_old(Eigen::MatrixXd& results_old_mat)
{
    FILE* fp;
    fp = fopen("already_Found_results.txt","r");
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
        if(ch!='\n')
        {
            int temp;
            temp=ch;
            temp-=48;
            results_old_mat(rows,columns)=temp;
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

int mutate_array(Eigen::VectorXd& array,float mutation_probablity,Eigen::MatrixXd& coeff_mat)
{
	//srand(time(NULL));
	double p = rand() / (double)RAND_MAX;
	//cout<<" P = "<<p<<endl;
	int temp_mut=0;
	int mut_1 = random_number(2,114,0,1);
	int mut_2 = random_number(2,114,0,1);
	int cost_before_mutation = cost_full(coeff_mat,array);
	int cost_after_mutation = 0;
	if(p < mutation_probablity)  //Swap If Less than Mut_prob
	{
		temp_mut = array(mut_1);
		array(mut_1) = array(mut_2);
		array(mut_2) = temp_mut;
		//cout<<"In Here"<<endl;
	}	
	cost_after_mutation = cost_full(coeff_mat,array);
	if(cost_after_mutation-cost_before_mutation!=0)
		cout<<" Mutation happened, Cost =  "<<cost_after_mutation<<endl;
	return (cost_after_mutation-cost_before_mutation);
}
int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test)
{
	int changes=0;
	for(int i=2;i<116;i++)
	{
		if(array(i)!=array_test(i))
			changes++;
	}
	return changes;
}
int perform_swap(Eigen::VectorXd& array,Eigen::VectorXd& array_test)
{
	Eigen::MatrixXd coeff_mat_in_swap(116,116);
	assign_coeff_mat(coeff_mat_in_swap);
	int cost_before=0;
	int cost_afterr=0;
	int temp = 0;
	int first_cost=cost_full(coeff_mat_in_swap,array);
	array_test=array;
	//Loops
	for(int i=2;i<115;i++)
	{
		if(i==94)
			i++;
		//cout<<"I == "<<i<<endl;
		for(int j=2;j<115;j++)
		{
			if(j==94)
				j++;

			if(array(i)!=array(j))
			{
				//swaps_performed++;
				cost_before = cost_full(coeff_mat_in_swap,array);
				//cout<<"Cost Before == "<<cost_full(coeff_mat,array)<<endl;
				//Swap I and J 
				temp = array(i);
				array(i)=array(j);
				array(j)=temp;
				//cout<<"Cost After == "<<cost_full(coeff_mat,array)<<endl;
				//Compute Cost
				cost_afterr = cost_full(coeff_mat_in_swap,array);
				/*cost_sum+=cost;
				if(cost<cost_lowest)
					cost_lowest=cost;
				if(cost>cost_highest)
					cost_highest=cost;*/

				//Reswap
				if(cost_afterr>cost_before)
				{
					temp = array(i);
					array(i)=array(j);
					array(j)=temp;
					cost_afterr=cost_before;
				}
				//else
					//cout<<endl<<" 1 Swapping Effective "<<endl;
			}
		}
	}

	//cout<<"Swapped"<<endl;
	if(cost_afterr == first_cost)
		return 1;  // This Has to Be Swap Not Reducing Cost 
	else 
		return 0;
}

void print_result_new(Eigen::VectorXd& array)
{
	FILE* fp_rslt;
	fp_rslt=fopen("results_found_by_me.txt","a");
	int temp;
	cout<<endl;
	for(int i=1;i<116;i++)
	{
		temp=array(i);
		cout<<temp;
		fprintf(fp_rslt,"%d",temp);
	}
	cout<<endl;
	fprintf(fp_rslt,"\n");
	fclose(fp_rslt);
}
void print_result_lowest(int cost,Eigen::VectorXd& array)
{
	FILE* fp_rslt_low;
	fp_rslt_low=fopen("results_found_by_me_lowest_org.txt","a+");
	int temp;
	cout<<endl;
	for(int i=1;i<116;i++)
	{
		temp=array(i);
		cout<<temp;
		fprintf(fp_rslt_low,"%d",temp);
	}
	fprintf(fp_rslt_low, " Cost : %d ",cost);
	cout<<endl;
	fprintf(fp_rslt_low,"\n");
	fclose(fp_rslt_low);
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

void print_matrix(Eigen::MatrixXd& mat)
{
    for(int i=1;i<=115;i++)
    {
        for(int j=1;j<=115;j++)
        {
            cout<<mat(i,j);
        }
        cout<<endl;
    }
}

int is_okay(Eigen::VectorXd& array)  //Returns 1 if okay 0 if not okay
{
	int flag=1,total_ones=0;
	if(array(1)!=0)
	{
		flag=0;
		cout<<"Problem 1"<<endl;
	}
	if(array(115) != 1 - array(94))
	{
		flag=0;
		cout<<"Problem 2"<<endl;
	}
	for(int i=2;i<=114;i++)
	{
		if(i==94)
			i++;
		if(int(array(i))==1)
			total_ones++;
	}
	if(total_ones!=56)
	{
		flag=0;
		cout<<"Problem 3"<<endl;
	}
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
    int temp_cost=0;
	array_temp = coeff_mat * array;
	for(int i=1;i<=115;i++)
	{
        temp_cost=array_temp(i);
        //cout<<temp_cost<<endl;
		if((temp_cost>=57)&&(temp_cost<=72))
			array_temp(i)=0;
		if(temp_cost<57)
            array_temp(i)=(57-temp_cost)*(57-temp_cost);
		if(temp_cost>72)
			array_temp(i)=(temp_cost-72)*(temp_cost-72);
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