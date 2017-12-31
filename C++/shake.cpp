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
void random_generate(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test);
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
}

int main()
{
	srand(time(NULL));
	//Load Co-efficient Matrix, Initialize Arrays and Variables
	Eigen::MatrixXd coeff_mat(116,116);
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_main(116);
	assign_coeff_mat(coeff_mat);

	for(int i=0;i<=10000;i++)
	{
		int rand_gen = rand() % 21;
		rand_gen++;
		random_generate(array_main);
	    shake(array_main,array,rand_gen);
	    if(is_okay(array)==0)
	    	cout<<"Error Integrity"<<endl;
	    if(change_in_array(array,array_main)!=(rand_gen*2))
	    	cout<<"Error In Changes"<<endl;
	}
	return 0;
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