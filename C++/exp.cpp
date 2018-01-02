#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

    int lowest_differance = 115;
    long long int total_diff = 0;

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
void random_generate(Eigen::VectorXd& array);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void assign_result_arrays(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
void swap_fixed_pos(Eigen::VectorXd& array);
void try_swap_fixed(Eigen::VectorXd& array,Eigen::MatrixXd& coeff_mat);
void print_result_new(Eigen::VectorXd& array);
void assign_results_new(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
void print_matrix(Eigen::MatrixXd& mat);
int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test);
void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results);


int main()
{
	srand(time(NULL));
	//Load Co-efficient Matrix, Initialize Arrays and Variables
	Eigen::MatrixXd coeff_mat(116,116);
    assign_coeff_mat(coeff_mat);

    Eigen::MatrixXd results(116,116);     // New Results
    assign_results_new(results);

    // Already Found Results
    Eigen::MatrixXd results_old(116,116); 
    assign_results_old(results_old);
	
    Eigen::VectorXd array(116);


	for(int i=1;i<=200000;i++)
    {
        random_generate(array);
        //cout<<"Cost == "<<cost_full(coeff_mat,array)<<endl;
        try_difference(array,results);
    }
    cout<<"Lowest == "<<lowest_differance<<endl;

	return 0;
}

void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results) 
{
    FILE* fp_diff;
    fp_diff = fopen("diff_log.txt","a"); 
    Eigen::VectorXd result(116);
    int diff = 0;
    for(int i=1;i<=4;i++)
    {
       result = results.row(i); 
       diff = change_in_array(result,array); 
       total_diff+= diff;
       if(diff<lowest_differance)
            {
                lowest_differance=diff;
                for(int j=1;j<=115;j++)
                    fprintf(fp_diff, "%d",array(j));
                fprintf(fp_diff,"\n  Difference == %d  \n",lowest_differance);
            }
    }
    fclose(fp_diff);
}

void assign_results_new(Eigen::MatrixXd& coeff_mat)
{
    FILE* fp;
    fp = fopen("results_found_by_me_lowest_main.txt","r");
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
    else 
        {
            cout<<"Swapped"<<endl;
            cout<<swap_cost_1<<endl;
            cout<<swap_cost_2<<endl;
        }
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
        //cout<<array_temp(i);
	}
    //cout<<endl;
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