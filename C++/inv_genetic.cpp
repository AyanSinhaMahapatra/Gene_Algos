#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void random_generate_inv(Eigen::VectorXd& array);
int random_number(int start,int end,int length,int order);
void random_parents(int *parents_index,int p_length);
double cost_full_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array)
{
	Eigen::VectorXd array_temp(116);
    double temp_cost=0;
	array_temp = inv_coeff_mat * array;
	//cout<<array_temp<<endl;
	
	for(int i=1;i<=115;i++)
	{
        temp_cost=array_temp(i);
        cout<<temp_cost<<" - ";
		if((temp_cost>=0)&&(temp_cost<=1))
		{
			if(temp_cost>=0.5)
				array_temp(i) = (1 - temp_cost)*(1 - temp_cost);
			else if(temp_cost<0.5)
				array_temp(i) = temp_cost*temp_cost;
		}
		if(temp_cost<0)
			array_temp(i) = ((-100)*temp_cost) * ((-100)*temp_cost) * ((-100)*temp_cost);
		if(temp_cost>1)
			array_temp(i) = ((temp_cost - 1)*100) * ((temp_cost - 1)*100) * ((temp_cost - 1)*100);
		temp_cost=array_temp(i);
        cout<<temp_cost<<endl;
	}
	array_temp(0)=0;
	return array_temp.sum();
}

// 2 - Problem In Array. 1 - Problem in Binary Array. 0 - Okay.
int is_okay_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,int tolerance)
{
	Eigen::VectorXd array_temp(116);
    double temp_okay=0;
	array_temp = inv_coeff_mat * array;

	// Checking Array (That has Numbers between 57 - 72) 
	for(int i=1;i<=115;i++)
	{
		temp_okay = array(i);
		if((temp_okay>72)||(temp_okay<57))
			return 2;
	}

	// Checking Binary Array
	int count[2];
	int tol1 = 1 + pow(0.1,tolerance);
	int tol0 = 0 - pow(0.1,tolerance);
	for(int i=1;i<=115;i++)
	{
		temp_okay = array_temp(i);
		if((temp_okay>tol1)||(temp_okay<tol0))
			return 1;
		if((i!=1)&&(i!=94)&&(i!=115))
		{
			if(temp_okay>=0.5)
				count[1]++;
			if(temp_okay<0.5)
				count[0]++;
		}
	}
	if(((check_if_close(array_temp(94),1,3))&&(check_if_close(array_temp(115),0,3)))||((check_if_close(array_temp(115),1,3))&&(check_if_close(array_temp(94),0,3))))
		temp_okay = 0;
	else 
		return 1;

	if((count[1]!=56)||(count[0]!=56))
		return 1;

	// If Here Than Both Arrays are Okay 
	return 0;
}

double give_value_with_tol(double value, int tolerance, char sign)
{
	if(char=='p')
		value = value + pow(0.1,tolerance);
	else if(char=='n')
		value = value - pow(0.1,tolerance);
	return value;
}

int check_if_close(double to_check,int check_with,int tolerance)
{
	if((to_check<give_value_with_tol(check_with,tolerance,'p'))&&(to_check>give_value_with_tol(check_with,tolerance,'n')))
		return 1;
	else 
		return 0;
}

int main()
{
	srand(time(NULL));

	//Load Co-efficient Matrix, Initialize Arrays and Variables
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_main(116);
    Eigen::VectorXd array_test(116);
    Eigen::VectorXd array_temp(116);
	
	Eigen::MatrixXd coeff_mat(116,116);
	Eigen::MatrixXd inv_coeff_mat(116,116);
	assign_coeff_mat(coeff_mat);
	inv_coeff_mat = coeff_mat.inverse();

	Eigen::MatrixXd results_old(116,116); 
    assign_results_old(results_old);

    random_generate_inv(array);

    double cost=0;
    cost = cost_full_inv(inv_coeff_mat,array);
    cout<<"Cost = "<<cost<<endl;

	return 0;
}

// Changed Functions
void random_generate_inv(Eigen::VectorXd& array) //Generates A Random Array which obeys all the specifications
{
	srand(time(NULL));
	array.fill(0);
	int rand_gen=0;
	for(int i=1;i<=115;i++)
	{
		rand_gen = rand() % 16;
		rand_gen +=57;
		array(i)=rand_gen;
	}
}

// New Functions


// Same Functions
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