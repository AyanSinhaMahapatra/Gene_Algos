#include<stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
void assign_results_old(Eigen::MatrixXd& coeff_mat);

int main()
{
	Eigen::MatrixXd coeff_mat(116,116);
    Eigen::VectorXd array(116);
    assign_coeff_mat(coeff_mat);
	
	Eigen::MatrixXd results_old(3,116); 
    assign_results_old(results_old);

    //array = results_old.row(1);
    //cout<<array<<endl;
    //cout<<is_okay(array)<<endl;
    //cout<<cost_full(coeff_mat,array)<<endl;

    int i = 0;

    i = -0.12;
    cout<<i<<endl;

    i = 0.49999;
    cout<<i<<endl;

    i = 0.554556;
    cout<<i<<endl;

    i = 1.054556;
    cout<<i<<endl;

    i = 0.954556;
    cout<<i<<endl;
}

void assign_results_old(Eigen::MatrixXd& results_mat)
{
	//cout<<coeff_mat<<endl;
    FILE* fp;
    fp = fopen("Results_Prob.txt","r");
    if(fp==NULL)
        cout<<"Error Not Loading"<<endl;
    else
    	cout<<"Okay"<<endl;
    int count=0;
    char ch;
    int flag = 1;
    int rows=1,columns=1;
    while(flag)
    {
        ch=fgetc(fp);
        //cout<<"Ch == "<<ch<<endl;
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
            results_mat(rows,columns)=temp;
            //cout<<rows<<" -- "<<columns<<endl;
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
	{
		flag=0;
		cout<<"Problem 1"<<endl;
	}
	if(array(115) != 1 - array(94))
	{
		flag=0;
		cout<<"Problem 2"<<endl;
		cout<<array(115)<<" -- "<<array(94)<<endl;
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
		cout<<total_ones<<endl;
	}
	return flag;
}

int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array)
{
	Eigen::VectorXd array_temp(116);
    int temp_cost=0;
	array_temp = coeff_mat * array;
	cout<<array_temp<<endl;
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

		temp_cost=array_temp(i);
        cout<<i<<"  --  "<<temp_cost<<endl;
	}
	return array_temp.sum();
}

// Opening The File Containing The Co-efficient Matrix
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat)
{
	FILE* fp;
	fp = fopen("Coefficient Matrix.txt","r");
	if(fp==NULL)
        cout<<"Error Not Loading"<<endl;
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