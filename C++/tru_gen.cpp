// Complete Genetic Algorithms Implementation
// A pool of Arrays, matched up randomly in pairs, 
// Crossover (a certain percentage of the crossover happening) between two whole arrays
// only those position of the arrays which are not the same, 
// and crossovers in multiple of two, to maintain the property of same 0's and 1's
// without 1st, 94th, 115th entries, and check if offsprings better
// Replaced with lowest Cost Array  


#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

    int num_arr = 10; //Number of Arrays in the Pool
    double mutation_probablity = 0.1;

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void random_generate(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
void assign_results_new(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
void random_generate_arrays(Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays,int *cost_array);
void create_random_pairs(int *pairs);
void cost_init_gen(Eigen::MatrixXd& coeff_mat,double *cost_array,Eigen::MatrixXd& arrays);
void genetic_algo_inv(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2);

int main()
{
    int cost_array[num_arr];
    int pair_no = num_arr/2;
    int pairs[pair_no];
    int flag = 1;

    srand(time(NULL));

    //Load Co-efficient Matrix, Initialize Arrays and Variables
    Eigen::MatrixXd coeff_mat(116,116);
    Eigen::MatrixXd arrays(num_arr,116);
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_main(116);

    assign_coeff_mat(coeff_mat);

    random_generate_arrays(coeff_mat,arrays,cost_array);

    cout<<"Starts"<<endl;

    while(1)
    {
        step_genetic(coeff_mat,arrays);
        count++;

        if(count%1000==0)
        {
            print_cost_all_arrays(coeff_mat,arrays);    
        }
        //cout<<"Step Main"<<endl;
    }
    return 0;
}

void step_genetic(Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays)
{
    int pair_no = num_arr/2;
    int pairs[pair_no];
    int temp=0;
    int flag1=0;
    int flag2=0;

    create_random_pairs(pairs);

    Eigen::VectorXd array(116);
    Eigen::VectorXd array_2(116);

    Eigen::VectorXd array_off(116);
    Eigen::VectorXd array_off_2(116);

    double cost_array[num_arr+1];
    double new_cost[2];

    cost_init_gen(coeff_mat,cost_array,arrays);

    for(int i=0;i<pair_no;i++)
    {
        flag1 = 0;
        flag2 = 0;
        temp = pairs[i]; 
        array = arrays.row(temp);
        array_2 = arrays.row(i+pair_no);

        genetic_algo_inv(coeff_mat,array,array_2,array_off,array_off_2);

        new_cost[0]=cost_full(coeff_mat,array_off);
        new_cost[1]=cost_full(coeff_mat,array_off_2);

        for(int j=1;j<=num_arr;j++)
        {
            if(new_cost[0]==cost_array[j])
                flag1=1;
            if(new_cost[1]==cost_array[j])
                flag2=1;
        }

        for(int j=1;j<=num_arr;j++)
        {
            if((cost_array[j]>new_cost[0])&&(flag1==0))
            {
                for(int k=1;k<=115;k++)
                    arrays(j,k)=array_off(k);
                flag1=1;
                continue;
            }
            if((cost_array[j]>new_cost[1])&&(flag2==0))
            {
                for(int k=1;k<=115;k++)
                    arrays(j,k)=array_off_2(k);
                flag2=1;
            }
        }
    }
    return;
}

void genetic_algo_inv(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2)
{
    array_off = array;
    array_off_2 = array_2;

    int array_diff_1at1[116];
    int array_diff_1at2[116];

    int place_1 = 1;
    int place_2 = 1;
    for(int i=1;i<=115;i++)
    {
        if(array(i)!=array_2(i))
        {
            if(array(i)==1)
            {
                array_diff_1at1[place_1] = i;
                place_1++;
            }
            else if(array_2(i)=1)
            {
                array_diff_1at2[place_2] = i;
                place_2++;
            }
        }
    }



    // Mutation 
    for(int i=0;i<=115;i++)
    {
        double p1 = rand() / (double)RAND_MAX;
        double p2 = rand() / (double)RAND_MAX;

        if(p1 < mutation_probablity)
            mutate_arr_at_loc(array_off,i);
        if(p2 < mutation_probablity)
            mutate_arr_at_loc(array_off_2,i);
    }
    
    if(check_similar_arrays(array,array_off)||check_similar_arrays(array_2,array_off_2))
        cout<<" PROBLEM ARRAY NOT CHANGING "<<endl;

    return;
}

void cost_init_gen(Eigen::MatrixXd& coeff_mat,double *cost_array,Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);

    for(int i=1;i<=num_arr;i++)
    {
        array = arrays.row(i);
        cost_array[i] = cost_full(coeff_mat,array);
    }

    return;
}


void print_cost_all_arrays(Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);

    for(int i=1;i<=num_arr;i++)
    {
        array = arrays.row(i);
        //cout<<array<<endl<<"-------------------"<<endl;
        cout<<cost_full(coeff_mat,array)<<"  ";
    }
    cout<<endl<<endl;

    return;
}

void create_random_pairs(int *pairs)
{
    //srand(time(NULL));
    int pair_no = num_arr/2;
    int rand_no = 0;
    int flag = 1;

    for(int j=0;j<pair_no;j++)
        pairs[j]=0;

    for(int i=0;i<pair_no;i++)
    {
        while(1)
        {
            flag = 1;
            rand_no = rand()%pair_no + 1;
            for(int j=0;j<pair_no;j++)
            {    
                if(pairs[j]==rand_no)
                    flag = 0;
            }
            if(flag==1)
            { 
                pairs[i]=rand_no;
                break;
            }
        }
    }
}

void random_generate_arrays(Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays,int *cost_array)
{
    Eigen::VectorXd array(116);
    arrays.fill(0);

    for(int i=0;i<num_arr;i++)
    {
        random_generate(array);
        for(int j=1;j<=115;j++)
        {
            arrays(i,j)=array(j);
        }
        array = arrays.row(i);
        //cout<<is_okay(array)<<endl;
    }

    for(int i=0;i<num_arr;i++)
    {
        array = arrays.row(i);
        cost_array[i]=cost_full(coeff_mat,array);
        //cout<<cost_array[i]<<endl;
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
    //srand(time(NULL));
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

void assign_results_old(Eigen::MatrixXd& coeff_mat)
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