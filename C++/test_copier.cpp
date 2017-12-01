#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

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
void double_crossover_reproduction(int *parent1,int *parent2,int *offspring1,int *offspring2,int length)
{
	int rand_boundary = length/2 ;
	//Left To Right
	int rand_var = rand() % rand_boundary;
	rand_var = rand_var + length/2 - 2 ;
	cout<<"Crossover at - "<<rand_var<<endl;
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
	//Right To Left
	rand_var = rand() % rand_boundary;
	rand_var = length - length/2 - rand_var ;
	cout<<"Crossover at - "<<rand_var<<endl;
	temp = length-1;
	while(temp>rand_var)
	{
		offspring1[temp]=parent1[temp];
		offspring2[temp]=parent2[temp];
		temp++;
	}
	while(temp>=1)
	{
		offspring2[temp]=parent1[temp];
		offspring1[temp]=parent2[temp];
		temp++;
	}
}
void simple_crossover_reproduction(int *parent1,int *parent2,int *offspring1,int *offspring2,int length)
{
	int rand_boundary = length-2 ;
	//Left To Right
	int rand_var = rand() % rand_boundary;
	rand_var+=2;
	cout<<"Crossover at - "<<rand_var<<endl;
	int temp = 0;
	while(temp<rand_var)
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
void copy_parents_to_array(Eigen::VectorXd& array,int *offspring1,int *offspring2,int length,int *parents_index)
{
	int temp=parents_index[0];
	int i=0;
	while(temp!=(parents_index[1]+1))
	{
		if(temp==94)
			temp++;
		if(temp==115)
			temp=2;
		array(temp)=offspring1[i];
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
		array(temp)=offspring2[i];
		temp++;
		i++;
	}
}
int main()
{
    Eigen::VectorXd array(116);
	random_generate(array);
	cout<<array<<endl;

	int p1_parents_index[4];
	int p1_length=10;
	random_parents(p1_parents_index,p1_length);
	cout<<"First Parent"<<endl;
	cout<<p1_parents_index[0]<<" - "<<p1_parents_index[1]<<endl;
	cout<<"Second Parent"<<endl;
	cout<<p1_parents_index[2]<<" - "<<p1_parents_index[3]<<endl<<endl; 
	
	int parent1[p1_length];
	int parent2[p1_length];
	copy_parents_from_array(array,parent1,parent2,p1_length,p1_parents_index);

	cout<<"1st Parent - 2nd Parent"<<endl;
	for(int i=0;i<p1_length;i++)
	{
		cout<<parent1[i]<<" - "<<parent2[i]<<endl;
	}

	int offspring1[p1_length];
	int offspring2[p1_length];
	simple_crossover_reproduction(parent1,parent2,offspring1,offspring2,p1_length);

	cout<<"1st offspring - 2nd offspring"<<endl;
	for(int i=0;i<p1_length;i++)
	{
		cout<<offspring1[i]<<" - "<<offspring2[i]<<endl;
	}

    copy_parents_to_array(array,offspring1,offspring2,p1_length,p1_parents_index);
    cout<<array<<endl;

	return 0;
}