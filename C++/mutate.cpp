#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

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

int mutate_array(Eigen::VectorXd& array,float mutation_probablity,Eigen::MatrixXd& coeff_mat)
{
	//srand(time(NULL));
	double p = rand() / (double)RAND_MAX;
	//cout<<p<<endl;
	int temp_mut=0;
	int mut_1 = random_number(2,114,0,1);
	int mut_2 = random_number(2,114,0,1);
	int cost_before = cost_full(coeff_mat,array);
	if (p < mutation_probablity)  //Swap If Less than Mut_prob
	{
		temp_mut = array(mut_1);
		array(mut_1) = array(mut_2);
		array(mut_2) = temp_mut;
		//cout<<"In Here"<<endl;
	}	
	return (cost_full(coeff_mat,array)-cost_before);
}

int main()
{
	srand(time(NULL));
	Eigen::MatrixXd coeff_mat(116,116);
    Eigen::VectorXd array(116);
	assign_coeff_mat(coeff_mat);
	float mutation_total;
	float mutation_probablity=0.2;
	
	for(int j=1;j<=10;j++)
	{
		mutation_total=0;
		for(int i=1;i<=1000;i++)
		{
			random_generate(array);
			mutation_total+=mutate_array(array,mutation_probablity,coeff_mat);
		}
		cout<<"Mutation Average = "<<(mutation_total/1000)<<endl;
	}
	return 0;
}

