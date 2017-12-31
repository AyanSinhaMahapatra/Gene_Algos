#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void assign_result_arrays(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
void print_result_new(Eigen::VectorXd& array);
void assign_result_arrays_all(Eigen::VectorXd& array);

int main()
{
	srand(time(NULL));
	//Load Co-efficient Matrix, Initialize Arrays and Variables
	Eigen::MatrixXd coeff_mat(116,116);
	Eigen::VectorXd result_1(116);
	Eigen::VectorXd result_2(116);
	Eigen::VectorXd result_3(116);
	Eigen::VectorXd result_4(116);
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_test(116);
	assign_coeff_mat(coeff_mat);

	//Testing Already Found Results
	//assign_result_arrays(array);
	//cout<<"Cost = "<<cost_full(coeff_mat,array)<<endl;
	//flag=is_okay(array);
	return 0;
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

void assign_result_arrays_all(Eigen::VectorXd& array)
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
