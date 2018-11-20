#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>

//Linear Algebra Library
#include<Eigen/Dense>
#include<Eigen/Eigenvalues>
#include <Eigen/Cholesky>
#include <Eigen/Cholesky>
#include <Eigen/LU>
#include <Eigen/QR>
#include <Eigen/SVD> 

using namespace std;
using namespace Eigen; 

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
void write_coeff_mat(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);

int main()
{
	Eigen::MatrixXd coeff_mat(116,116);
	Eigen::MatrixXd coeff_mat_svd(115,115);
    Eigen::MatrixXd coeff_mat_1(116,116);
	Eigen::VectorXd array(116);
	Eigen::VectorXd array_svd(115);
	assign_coeff_mat(coeff_mat);
    assign_results_old(coeff_mat_1);

    for(int i=1;i<=115;i++)
    	for(int j = 1;j<=115;j++)
    		coeff_mat_svd(i-1,j-1) = coeff_mat(i,j);

    //cout<<"eigenvalues == "<<coeff_mat_svd.eigenvalues()<<endl;

    //write_coeff_mat(coeff_mat_svd);

    //cout<<coeff_mat_svd.computeU()<<endl;  This Doesn't Work

    // For Average and Variance calculation For Costs of Already found results
    /*
    cout<<"After taking out main Matrix"<<endl;

    for(int i=1;i<=4;i++)
    {
    	array = coeff_mat_1.row(i);
    	for(int i=1;i<=115;i++)
    		array_svd(i-1)=array(i);

    	cout<<"Cost == "<<cost_full(coeff_mat_svd,array_svd)<<endl;
    }
    */
}

int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array)
{
	Eigen::VectorXd array_temp(115);
    float temp_cost=0;
	array_temp = coeff_mat * array;

	float sum = 0;
	float average,variance = 0;
	for(int i=0;i<=114;i++)
		sum += array_temp(i);
	average = sum/115;

	for(int i=0;i<=114;i++)
	{
		temp_cost=array_temp(i);
		if(temp_cost>average)
			variance += (temp_cost - average);
		else if(temp_cost<average)
			variance += (average - temp_cost);
	}
	cout<<average<<" "<<(variance/115)<<endl;

	for(int i=0;i<=114;i++)
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

void write_coeff_mat(Eigen::MatrixXd& coeff_mat)
{
	int temp = 0;
	FILE* fp;
	fp = fopen("Coefficient_Matrix_mat.txt","w");

	for(int i=0;i<=114;i++)
    {
    	for(int j=0;j<=114;j++)
    	{
    		temp = coeff_mat(i,j);
    	    fprintf(fp, "%d ",temp);
    	}
    	fprintf(fp, "\n");
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