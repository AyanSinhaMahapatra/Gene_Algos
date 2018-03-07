#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
void assign_results_new(Eigen::MatrixXd& coeff_mat);
void random_generate(Eigen::VectorXd& array);
void change_array_to_whatever(Eigen::VectorXd& array,int index,double value);
void swap_array_to_whatever(Eigen::VectorXd& array,int index1,double index2);
double cost_full_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array);
int is_okay_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,int tolerance);
int is_okay(Eigen::VectorXd& array);  //Returns 1 if okay 0 if not okay
int check_if_close(double to_check,int check_with,int tolerance);
double give_value_with_tol(double value, int tolerance, char sign);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void random_generate_inv(Eigen::VectorXd& array); //Generates A Random Array which obeys all the specifications
void convert_binary_to_normal(Eigen::VectorXd& array_binary,Eigen::VectorXd& array_normal);
int check_results(Eigen::VectorXd& array_normal)
{
	Eigen::VectorXd array_binary(116);
	Eigen::MatrixXd coeff_mat(116,116);
	Eigen::MatrixXd inv_coeff_mat(116,116);
	assign_coeff_mat(coeff_mat);
	inv_coeff_mat = coeff_mat.inverse();
	array_binary = inv_coeff_mat * array_normal;
	find_closest_binary(array_binary);

	int cost_result = cost_full(coeff_mat,array_binary);
	if(cost_result==0)
		cout<<endl<<"-----0 Cost Found -----"<<endl;
	return cost_result;
}
void find_closest_binary(Eigen::VectorXd& array_binary)
{
	double temp = 0;
	for(int i=1;i<=115;i++)
	{
		temp = array_binary(i);
		if(check_if_close(temp,1,3))
			array_binary(i)=1;
		else if(check_if_close(temp,0,3))
			array_binary(i)=0;
	}
	if(is_okay(array_binary))
		return;
	else
	{
		cout<<"Problem In Array - Couldn't Find Closest Binary Array"<<endl;
		estimate_binary(array_binary);
		return;
	}
}
void estimate_binary(Eigen::VectorXd& array_binary)
{
	double temp = 0;
	for(int i=1;i<=115;i++)
	{
		temp = array_binary(i);
		if(temp>=0.5)
			array_binary(i)=1;
		else if(temp<0.5)
			array_binary(i)=0;
	}
	if(is_okay(array_binary))
		return;
	else
	{
		cout<<"Correcting Array :: "<<endl;
		correct_binary(array_binary);
	}
}
void correct_binary(Eigen::VectorXd& array_binary)
{
	cout<<"Corrected"<<endl;
	return;
}
void genetic_algo_inv(int length,Eigen::VectorXd& array)
{

    Eigen::VectorXd array_test(116);
	int index[2];
	index[0]=0;index[1]=0;
	index[0]=random_number_inv(length);
	index[1]=index[0]+length;


}


// ------------------------------------------------------------------------------------------------
int main()
{
	srand(time(NULL));
    Eigen::VectorXd array(116);
    Eigen::VectorXd array2(116);
    array.fill(0);
    array2.fill(0);

    Eigen::MatrixXd coeff_mat(116,116);
	Eigen::MatrixXd inv_coeff_mat(116,116);
	assign_coeff_mat(coeff_mat);
	inv_coeff_mat = coeff_mat.inverse();

	Eigen::MatrixXd results_old(116,116); 
    assign_results_old(results_old);

    Eigen::MatrixXd results_new(116,116);     // New Results
    assign_results_new(results_new);

    //random_generate_inv(array);
    //cout<<array;
    array = results_old.row(1);
    //cout<<cost_full(coeff_mat,array)<<endl;
    //cout<<array<<endl;
    convert_binary_to_normal(array,array2);
    //change_array_to_whatever(array2,index,value);
	//swap_array_to_whatever(array2,94,115);
    //cout<<array2<<endl;
    cout<<is_okay_inv(inv_coeff_mat,array2,4)<<endl;

	return 0;
}
// ------------------------------------------------------------------------------------------------
// 2 - Problem In Array. 1 - Problem in Binary Array. 0 - Okay.
int is_okay_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,int tolerance)
{
	Eigen::VectorXd array_temp(116);
    double temp_okay=0;
	array_temp = inv_coeff_mat * array;
	//swap_array_to_whatever(array_temp,94,115);
	change_array_to_whatever(array_temp,115,0.00001);

	// Checking Array (That has Numbers between 57 - 72) 
	for(int i=1;i<=115;i++)
	{
		temp_okay = array(i);
		if((temp_okay>72)||(temp_okay<57))
		{
			cout<<i<<endl;
			cout<<temp_okay<<endl;
			return 2;
		}
	}

	// Checking Binary Array
	int count[2];
	count[0]=0;
	count[1]=0;
	double tol1 = 1 + pow(0.1,tolerance);
	double tol0 = 0 - pow(0.1,tolerance);
	for(int i=1;i<=115;i++)
	{
		temp_okay = array_temp(i);
		if((temp_okay>tol1)||(temp_okay<tol0))
		{	
			cout<<"i == "<<i<<endl;
			cout<<"Error 1"<<endl;
			return 1;
		}
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
		{
			cout<<"Error 2"<<endl;
			return 1;
		}

	if((count[1]!=56)||(count[0]!=56))
	{	
		cout<<"Error 3  ->  "<<count[0]<<" -- "<<count[1]<<endl;
		return 1;
	}

	// If Here Than Both Arrays are Okay 
	return 0;
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
void convert_binary_to_normal(Eigen::VectorXd& array_binary,Eigen::VectorXd& array_normal)
{
	Eigen::MatrixXd coeff_mat(116,116);
	assign_coeff_mat(coeff_mat);
	array_normal = coeff_mat * array_binary;
	array_normal(0)=0;
	return;
}
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
void change_array_to_whatever(Eigen::VectorXd& array,int index,double value)
{
	array(index)=value;
	return;
}
void swap_array_to_whatever(Eigen::VectorXd& array,int index1,double index2)
{
	int temp;
	temp          = array(index1);
	array(index1) = array(index2);
	array(index2) = temp;
	return;
}
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
int check_if_close(double to_check,int check_with,int tolerance)
{
	if((to_check<give_value_with_tol(check_with,tolerance,'p'))&&(to_check>give_value_with_tol(check_with,tolerance,'n')))
		return 1;
	else 
		return 0;
}
double give_value_with_tol(double value, int tolerance, char sign)
{
	if(sign=='p')
		value = value + pow(0.1,tolerance);
	else if(sign=='n')
		value = value - pow(0.1,tolerance);
	return value;
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