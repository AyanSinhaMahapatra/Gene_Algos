// Uses a tree like search method to look for maximum number of variations near a point in the search space 
// The variation is based on random probability and variation of pairs
// And to aggresively search for lower costs based on an binary-tree like true/false storing of the binary arrays

#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

// 0 - Left, 1 - Right 
struct treenode
{
    int data;
    struct treenode* left;
    struct treenode* right;
};

	struct treenode *his_loc;
	struct treenode *cur_loc;
	long long int pop_counter = 0;
	int case_cross = 1; // Crossover Method Selection
	int level_limit = 5; // Depth
	int no_of_rec = 20; // Bredth
	double swap_prob = 0.02;
	int best_cost = 100;

struct treenode* newNode(int data);
void insert_array(struct treenode *root,Eigen::VectorXd& array);
int is_present(struct treenode *root,Eigen::VectorXd& array);  // 1 - present , 0 - not present
void delete_array(struct treenode *root,Eigen::VectorXd& array); 
long long int population(struct treenode *root); // How many arrays are stored here
void delete_tree(struct treenode *root);
void pop_rec(struct treenode *temp);
void del_rec(struct treenode *temp);
void follow_print(struct treenode *root,Eigen::VectorXd& array);
void check_stray_node(struct treenode *root);
void stray_rec(struct treenode *temp);

void random_generate(Eigen::VectorXd& array);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int is_okay(Eigen::VectorXd& array);

void recursive_trav(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd array_val,int rec_level);
void random_change(Eigen::VectorXd array_val);
void check_history_and_store(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);

 
int main()
{
	srand(time(NULL));
	
	struct treenode history, current;
	his_loc = &history;
    cur_loc = &current;
	his_loc = newNode(0);
	cur_loc = newNode(0);
	
	Eigen::VectorXd array(116);

	for(int i=1;i<=10000;i++)
	{
		random_generate(array);
		//random_change(array);
		recursive_trav(array,1);
		//find_best_cost();
		break;
	}

	return 0;
}

// Recursive Search Functions 

void vns_recursive()
{
	


	
	return;
}

void recursive_trav(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd array_val,int rec_level)
{
	if(rec_level == level_limit)
		return;

	Eigen::VectorXd array(116);
	array = array_val;
	random_change(array);
	check_history_and_store(coeff_mat,array);

	for(int i=1;i<=no_of_rec;i++){
		recursive_trav(array,rec_level+1);
	}

	return;
}

void random_change(Eigen::VectorXd array_val)
{
	int array_1[57];
	int array_0[57];
	int temp_arr;
	int pointer_0 = 1;
	int pointer_1 = 1;
	int val_94 = array_val(94);
	int val_115 = array_val(115);

	for(int i=2;i<=114;i++)
	{
		if(i!=94)
		{
			temp_arr = array_val(i);

			if(temp_arr == 1)
			{
				array_1[pointer_1] = i;
				pointer_1++;
			}
			else
			{
				array_0[pointer_0] = i;
				pointer_0++;
			}
		}
	}

	if(pointer_0!=pointer_1)
		cout<<" Error Pointer "<<endl;

	int temp_shift;
	int counter_swap = 0;
	for(int i=1;i<=57;i++)
	{
		double p = rand() / (double)RAND_MAX;

		if(p<swap_prob)
		{
			if(i == 57)
			{
				array_val(94) = val_115;
				array_val(115) = val_94;
			}
			else
			{
				temp_shift = array_1[i];
				array_1[i] = array_0[i];
				array_0[i] = temp_shift;
			}
			counter_swap++;
		}
	}

	array_val.fill(0);
	for(int i=1;i<=56;i++)
	{
		array_val(array_1[i]) = 1;
	}
	array_val(94) = val_94;
	array_val(115) = val_115;

	if(counter_swap == 0)
	{
		int rand_index = rand() % 56 + 1;

		temp_shift          = array_1[rand_index];
		array_1[rand_index] = array_0[rand_index];
		array_0[rand_index] = temp_shift;
	}

	if(is_okay(array_val) == 0)
		cout<<"Error Random "<<endl;

	return;
}

void check_history_and_store(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array)
{


	return;
}


// Tree Operation Functions
struct treenode* newNode(int data)
{
  struct treenode* newnode = (struct treenode*)malloc(sizeof(struct treenode));

  newnode->data = data;
  newnode->left = NULL;
  newnode->right = NULL;

  return(newnode);
}

void insert_array(struct treenode *root,Eigen::VectorXd& array)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	struct treenode *temp;
	temp = root;
	int temp_int;

	for(int i=1;i<=115;i++)
	{
		temp_int = array(i);
		
		if(temp_int == 0)
		{
			if(temp->left == NULL)
				temp->left = newNode(i);
			temp = temp->left;
		}
		else if(temp_int == 1)
		{
			if(temp->right == NULL)
				temp->right = newNode(i);
			temp = temp->right;
		}
	}
	return;
}

void follow_print(struct treenode *root,Eigen::VectorXd& array)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	struct treenode *temp;
	temp = root;
	int temp_int;

	for(int i=1;i<=115;i++)
	{
		if(temp == NULL)
			return;

		temp_int = array(i);

		if(temp->left!=NULL)
			cout<<i<<"  LEFT TRUE   ";
		else
			cout<<i<<"  LEFT FALSE   ";
		if(temp->right!=NULL)
			cout<<"RIGHT TRUE"<<endl;
		else 
			cout<<"RIGHT FALSE"<<endl;
		
		if(temp_int == 0)
		{
			temp = temp->left;
		}
		else if(temp_int == 1)
		{
			temp = temp->right;
		}
	}

	return;
}

void check_stray_node(struct treenode *root)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	stray_rec(root);

	return;
}

void stray_rec(struct treenode *temp)
{
	if(temp == NULL)
		return;

	if((temp->right == NULL)&&(temp->left == NULL))
	{
		if(temp->data!=115)
			cout<<"Stray at "<<temp->data<<endl;
	}
	else{
		stray_rec(temp->left);
		stray_rec(temp->right);
	}

	return;
}

int is_present(struct treenode *root,Eigen::VectorXd& array)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return 0;
	}

	int flag = 1;
	struct treenode *temp;
	temp = root;
	int temp_int;

	for(int i=1;i<=115;i++)
	{
		temp_int = array(i);
		
		if(temp_int == 0)
			temp = temp->left;
		else if(temp_int == 1)
			temp = temp->right;

		if(temp == NULL)
		{
			flag = 0;
			cout<<"Break at "<<i<<endl;
			break;
		}
	}
	return flag;
}

void delete_array(struct treenode *root,Eigen::VectorXd& array)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	struct treenode *temp;
	temp = root;
	int temp_int,max_index = 0;

	for(int i=1;i<=115;i++)
	{
		temp_int = array(i);
		
		if(temp_int == 0)
		{
			if(temp->right!=NULL)
				max_index = i;
			temp = temp->left;
		}
		else if(temp_int == 1)
		{
			if(temp->left!=NULL)
				max_index = i;
			temp = temp->right;
		}
	}

	cout<<"Max Index == "<<max_index<<endl;
	temp = root;

	for(int i=1;i<=115;i++)
	{
		temp_int = array(i);
		cout<<i<<" ";
		
		if(temp_int == 0)
			temp = temp->left;
		else if(temp_int == 1)
			temp = temp->right;

		if(i == max_index)
		{
			del_rec(temp);
			break;
		}
	}

	return;
}

void delete_tree(struct treenode *root)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	del_rec(root);

	return;	
}

void del_rec(struct treenode *temp)
{
	if(temp == NULL)
		return;

	del_rec(temp->left);
	del_rec(temp->right);
	free(temp);

	return;
}

long long int population(struct treenode *root)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return 0;
	}

	long long int total;
	pop_counter = 0;
	pop_rec(root);
	total = pop_counter;
	pop_counter = 0;

	return total;
}

void pop_rec(struct treenode *temp)
{
	if(temp == NULL)
		return;
	else if(temp->data == 115)
	{
		pop_counter++;
		return;
	}
	else
	{
		pop_rec(temp->left);
		pop_rec(temp->right);
	}
}


// Functions Copied from vns_genetic.cpp are below
void random_generate(Eigen::VectorXd& array) 
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