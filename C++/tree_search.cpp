// Uses a tree like search method to look for maximum number of variations near a point in the search space 
// And to aggresively search for lower costs based on an binary-tree like true/false storing of the binary arrays

#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

	long long int pop_counter = 0;

// 0 - Left, 1 - Right 
struct treenode
{
    int data;
    struct treenode* left;
    struct treenode* right;
};

struct treenode *his_loc;
struct treenode *cur_loc;

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


int main()
{
	srand(time(NULL));
	
	struct treenode history, current;
	his_loc = &history;
	cur_loc = &current;
	his_loc = newNode(0);
	cur_loc = newNode(0);
	
	Eigen::VectorXd array(116);
	
	random_generate(array);
	insert_array(his_loc,array);
	array(56) = 1 - array(56);
	insert_array(his_loc,array);
	
	cout<<" Answer 1 "<<population(his_loc)<<endl;

	delete_array(his_loc,array);

	cout<<" Answer 2 "<<population(his_loc)<<endl;

	return 0;
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
		temp_int = array(i);

		if(temp->left!=NULL)
			cout<<i<<"  LEFT TRUE";
		else
			cout<<i<<"  LEFT FALSE";
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