#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<string>
#include <bits/stdc++.h>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 




// is_this : 0 - Left of Parent, 1 - Right of Parent, 2 - Root
// data : At which Depth the Node is in the main Tree
// status : 0 - open, 1 - closed
struct treenode
{
    int data;
    bool status;
    int is_this;
    struct treenode* left;
    struct treenode* right;
    struct treenode* parent;
};

	struct treenode *archive;
	long long int pop_counter = 0;
	double rand_find_thr = 0.5;
	int num_arr = 8;


// Helper Functions
void random_generate(Eigen::VectorXd& array);
void random_generate_arrays(Eigen::MatrixXd& arrays);
int is_okay(Eigen::VectorXd& array);
void assign_result_arrays(Eigen::VectorXd& array);

// Tree Functions
struct treenode* newNode(int data,int is_this);
void insert_array_and_prune(struct treenode *root,Eigen::VectorXd& array);
string change_ds(Eigen::VectorXd& array);
int is_present(struct treenode *root,Eigen::VectorXd& array);  // 1 - present , 0 - not present
void find_open(struct treenode *root,Eigen::VectorXd& array,Eigen::VectorXd& new_array);
void find_open_okay(struct treenode *root,Eigen::VectorXd& array,Eigen::VectorXd& new_array);
void delete_array(struct treenode *root,Eigen::VectorXd& array); 
long long int population(struct treenode *root); // How many arrays are stored here
void delete_tree(struct treenode *root);
void pop_rec(struct treenode *temp);
void del_rec(struct treenode *temp);
void follow_print(struct treenode *root,Eigen::VectorXd& array);
void check_stray_node(struct treenode *root);
void stray_rec(struct treenode *temp);
int check_number_01(struct treenode *root,struct treenode *temp,int no_of_zeros,int no_of_ones,Eigen::VectorXd& new_array);


int main()
{
	srand(time(NULL));
	
	struct treenode current;
    archive = &current;
	archive = newNode(0,2);

	Eigen::VectorXd array(116);
	Eigen::VectorXd array2(116);

	Eigen::MatrixXd arrays(num_arr+1,116);
    random_generate_arrays(arrays);

    // Find_open test 
	/*

    array = arrays.row(1);
    for(int i=1;i<=8;i++)
    {
    	array(115) = 1 - array(115);
    	if(i%2==0)
    		array(114) = 1 - array(114);
    	if(i%4==0)
    		array(113) = 1 - array(113);
    	insert_array(archive,array);
    	for(int j=1;j<=115;j++)
            cout<<array(j);
        cout<<endl;
    }

    for(long int i=1;i<=100000000;i++)
    {
    	find_open(archive,array,array2);
    	insert_array(archive,array2);
    	for(int j=1;j<=115;j++)
            cout<<array2(j);
        cout<<endl;
    	array = array2;
    	cout<<i<<endl;
    }

    cout<<"Population == "<<population(archive)<<endl;
    //array = arrays.row(1);
    //follow_print(archive,array);
	*/ 

    array = arrays.row(1);
    for(int i=1;i<=8;i++)
    {
    	array(115) = 1 - array(115);
    	if(i%2==0)
    		array(114) = 1 - array(114);
    	if(i%4==0)
    		array(113) = 1 - array(113);
    	insert_array_and_prune(archive,array);
    	for(int j=1;j<=115;j++)
            cout<<array(j);
        cout<<endl;
    }

    for(long int i=1;i<=10000000;i++)
    {
    	find_open_okay(archive,array,array2);
    	if(is_okay(array2)==0)
    		cout<<"----------------------------------------------------------------"<<endl;
    	insert_array_and_prune(archive,array2);
    	//for(int j=1;j<=115;j++)
            //cout<<array2(j);
        //cout<<endl;
    	array = array2;
    	//cout<<i<<endl;
    }

    cout<<"Population == "<<population(archive)<<endl;

	return 0;
}

// Tree Operation Functions
struct treenode* newNode(int data,int is_this) // Done
{
  struct treenode* newnode = (struct treenode*)malloc(sizeof(struct treenode));

  newnode->data = data;
  newnode->is_this = is_this;
  newnode->status = 0;
  newnode->left = NULL;
  newnode->right = NULL;
  newnode->parent = NULL;

  return(newnode);
}

void insert_array_and_prune(struct treenode *root,Eigen::VectorXd& array) //Done
{
	if(root == NULL)
	{
		cout<<"Invalid Root"<<endl;
		return;
	}

	struct treenode *temp;
	temp = root;
	int temp_int;

	for(int i=1;i<=115;i++)
	{
		temp_int = array(i);

		if(temp->status == 1)
		{
			//cout<<"Array Already Present in insert_array"<<endl;
			break;
		}
		
		if(temp_int == 0)
		{
			if(temp->left == NULL){
				temp->left = newNode(i,0);
				temp->left->parent = temp;
			}
			temp = temp->left;
		}
		else if(temp_int == 1)
		{
			if(temp->right == NULL){
				temp->right = newNode(i,1);
				temp->right->parent = temp;
			}
			temp = temp->right;
		}
	}

	temp->status = 1;
	temp = temp->parent;

	bool rec_flag = 1;

	while(rec_flag && temp!=NULL)
	{
		
		if (temp->left != NULL && temp->right != NULL && temp->left->status == 1 && temp->right->status == 1)
		{
			temp->status = 1;
			temp = temp->parent;
		}
		else 
			rec_flag = 0;
	}

	if (temp == NULL)
	{
		cout<<"Insert_Array -> Tree Overflow "<<endl;
		return;
	}

	if(temp->left != NULL && temp->left->status == 1)
		temp = temp->left;
	else
		temp = temp->right;

	struct treenode *left, *right;
	left = temp->left;
	right = temp->right;
	temp->left = NULL;
	temp->right = NULL;
	

	del_rec(left);
	del_rec(right);

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
		{
			cout<<"End at == "<<i-2<<endl;	
			return;
		}

		temp_int = array(i);

		if(temp->left!=NULL)
			cout<<i-1<<"  LEFT TRUE   ";
		else
			cout<<i-1<<"  LEFT FALSE   ";
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

void check_stray_node(struct treenode *root) //Done
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	stray_rec(root);

	return;
}

void stray_rec(struct treenode *temp) //Done
{
	if(temp == NULL)
		return;

	if((temp->right == NULL)&&(temp->left == NULL))
	{
		if(temp->data!=115 && temp->status==0)
			cout<<"Stray at "<<temp->data<<endl;
	}
	else{
		stray_rec(temp->left);
		stray_rec(temp->right);
	}

	return;
}

int is_present(struct treenode *root,Eigen::VectorXd& array) // Done
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
		if(temp->status == 1)
		{	
			flag = 1;
			break;
		}

		temp_int = array(i);
		
		if(temp_int == 0)
			temp = temp->left;
		else if(temp_int == 1)
			temp = temp->right;

		if(temp == NULL)
		{
			flag = 0;
			break;
		}
	}
	return flag;
}

void find_open_okay(struct treenode *root,Eigen::VectorXd& array,Eigen::VectorXd& new_array)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	//cout<<"Function Starts"<<endl;

	struct treenode *temp;
	struct treenode *temp_check;
	temp = root;
	int temp_int;
	int is_not_found = 1; 

	for(int i=1; i<=115; i++)
	{
		if(temp->status == 1)
			break;

		temp_int = array(i);

		if(temp_int == 0)
			temp = temp -> left;
		else if(temp_int == 1)
			temp = temp -> right;
	}

	new_array = array;
	temp = temp -> parent;
	double rand_find = 0;	

	int no_of_zeros = 0;
	int no_of_ones = 0;
	int is_94_1 = 2;
	int backtrack_needed = 0;
	
	for(int c = 2;c <= temp-> data;c++)
	{
		temp_int = new_array(c);

		if(c == 94)
		{
			if( temp_int == 0 )
				is_94_1 = 0;
			else
				is_94_1 = 1;
		}
		else if( temp_int == 0 )
			no_of_zeros++;
		else
			no_of_ones++;
	}

	if(check_number_01(root,temp,no_of_zeros,no_of_ones,new_array) == 0)
	{
		cout<<"Error No of Zeroes/Ones not matching at Start "<<endl;
		return;
	}

	//cout<<"While Loop In Function Starts"<<endl;

	while(is_not_found)
	{
		if(temp->left!=NULL&&temp->right!=NULL&&temp->left->status==1&&temp->right->status==1)
		{
			cout<<"Error Inconsistent Tree FUCK FUCK FUCK"<<endl;
			return;
		}
		else if(temp->data == 0) // Always value 0 at position 1
		{
			if(temp->left!=NULL && temp->left->status == 0)
			{
				temp = temp->left;
				new_array(temp->data) = 0;
			}
			else if(temp->left!=NULL && temp->left->status == 1)
			{
				cout<<" Overflow at 1 Left Subtree Full "<<endl;
				return;
			}
			else if(temp->left==NULL)
			{
				temp->left = newNode(temp->data+1,0);
				temp->left->parent = temp;
				temp = temp->left;
				new_array(temp->data) = 0;
			}
		}
		else if(temp->data == 114) // opposite of 94 at 115
		{
			//cout<<"114 statement Starts"<<endl;

			if(temp->status == 1)
				backtrack_needed = 1;
			else if( is_94_1 == 1 )
			{
				if(temp->left != NULL && temp->left->status == 1)
				{	
					backtrack_needed = 1;
				}
				else
				{
					if( temp->left == NULL )
					{
						temp->left = newNode(temp->data+1,0);
						temp->left->parent = temp;
						temp = temp->left;
						new_array(temp->data) = 0;
					}
					else if( temp->left != NULL )
					{
						temp = temp->left;
						new_array(temp->data) = 0;
					}

					is_not_found = 0;
				}
			}
			else if( is_94_1 == 0 )
			{
				if( temp->right != NULL && temp->right->status == 1)
				{	
					backtrack_needed = 1;
				}
				else
				{
					if( temp->right == NULL )
					{
						temp->right = newNode(temp->data+1,1);
						temp->right->parent = temp;
						temp = temp->right;
						new_array(temp->data) = 1;
					}
					else if( temp->right != NULL )
					{
						temp = temp->right;
						new_array(temp->data) = 1;
					}	

					is_not_found = 0;
				}
			}
			else if( is_94_1 == 2 )
			{	
				cout<<" Error is_94_1 is 2 "<<endl;
				return;
			}

			if(temp->data == 115)
				temp = temp->parent;
			if(temp->is_this == 1)
			{
				while(temp->is_this != 0)
					temp = temp->parent;
			}
			else if(temp->is_this == 0)
			{
				while(temp->is_this != 1)
					temp = temp->parent;
			}

			temp->status = 1;

			struct treenode *left, *right;
			if( temp->left != NULL )
				left = temp->left;
			if( temp->right != NULL )
				right = temp->right;
			temp->left = NULL;
			temp->right = NULL;

			if( temp->left != NULL )
				del_rec(left);
			if( temp->right != NULL )
				del_rec(right);

			//cout<<"114 Statement ends"<<endl;

		}
		else if(no_of_zeros==56||no_of_ones==56) // Only one possibility of Okay Array
		{
			//cout<<"0/1 56 statement starts"<<endl;

			int is_only_okay_present = 0;

			temp_check = temp;

			if(no_of_zeros==56)
			{
				while(temp != NULL && temp->data != 114)
				{
					if(temp -> right != NULL)
						temp = temp -> right;
					else
					{
						if(temp->status == 1)
						{
							is_only_okay_present = 1;
							break;
						}
						else
						{
							is_only_okay_present = 0;
							break;
						}
					}
				} 
			}
			else if(no_of_ones==56)
			{
				while(temp != NULL && temp->data != 114)
				{
					if(temp -> left != NULL)
						temp = temp -> left;
					else
					{
						if(temp->status == 1)
						{
							is_only_okay_present = 1;
							break;
						}
						else
						{
							is_only_okay_present = 0;
							break;
						}
					}
				} 
			}

			if(is_only_okay_present == 0)
			{
				//cout<<"only okay is not present "<<endl;

				temp = temp_check;

				if(no_of_zeros==56)
				{
					while(temp!= NULL && temp->data != 114)
					{
						if(temp -> right != NULL)
							temp = temp -> right;
						else
						{
							temp->right = newNode(temp->data+1,1);
							temp->right->parent = temp;
							temp = temp->right;
						}
						new_array(temp->data) = 1;
					}
				}
				else if(no_of_ones==56)
				{
					while(temp!= NULL && temp->data != 114)
					{
						if(temp -> left != NULL)
							temp = temp -> left;
						else
						{
							temp->left = newNode(temp->data+1,0);
							temp->left->parent = temp;
							temp = temp->left;
						}
						new_array(temp->data) = 0;
					}
				}

				if(temp->data != 114)
					cout<<"Error 114 not reached"<<endl;
			}
			else
			{	
				//cout<<"Only okay is present go back"<<endl;

				backtrack_needed = 1;
				temp = temp_check;

				// Make This Status 1 and Delete from this if not NULL
				temp->status = 1;

				struct treenode *left, *right;
				if( temp->left != NULL )
					left = temp->left;
				if( temp->right != NULL )
					right = temp->right;
				temp->left = NULL;
				temp->right = NULL;

				if( temp->left != NULL )
					del_rec(left);
				if( temp->right != NULL )
					del_rec(right);
			}

			//cout<<"0/1 56 statement ends"<<endl;

		}
		else if((temp->left != NULL && temp->left->status == 1) || (temp->right != NULL && temp->right->status == 1))
		{	
			//cout<<"Normal statement starts "<<endl;

			if(temp->left != NULL && temp->left->status == 1)
			{
				if( temp->right == NULL)
				{	
					temp->right = newNode(temp->data+1,1);
					temp->right->parent = temp;
				}
				temp = temp->right;
				new_array(temp->data) = 1;
				if(temp->data == 94)
					is_94_1 = 1;
				else
					no_of_ones++;
			}
			else 
			{
				if( temp->left == NULL)
				{
					temp->left = newNode(temp->data+1,0);
					temp->left->parent = temp;
				}
				temp = temp->left;
				new_array(temp->data) = 0;
				if(temp->data == 94)
					is_94_1 = 0;
				else
					no_of_zeros++;
			}

			//cout<<"Normal statement ends "<<endl;
		}
		else
		{
			//cout<<"Normal random statement starts "<<endl;

			rand_find = rand() / (double)RAND_MAX;

			if(rand_find < rand_find_thr)   //Left
			{
				if( temp->left == NULL)
				{	
					temp->left = newNode(temp->data+1,0);
					temp->left->parent = temp;
				}
				temp = temp->left;
				new_array(temp->data) = 0;
				if(temp->data == 94)
					is_94_1 = 0;
				else
					no_of_zeros++;
			}
			else //Right
			{
				if( temp->right == NULL)
				{
					temp->right = newNode(temp->data+1,1);
					temp->right->parent = temp;
				}
				temp = temp->right;
				new_array(temp->data) = 1;
				if(temp->data == 94)
					is_94_1 = 1;
				else
					no_of_ones++;
			}

			//cout<<"Normal random statement ends "<<endl;
		}

		if(backtrack_needed == 1)
		{
			//cout<<"Backtrack starts "<<endl;

			bool rec_flag = 1;

			temp = temp->parent;

			while(rec_flag && temp!=NULL)
			{
				if (temp->left != NULL && temp->right != NULL && temp->left->status == 1 && temp->right->status == 1)
				{
					temp->status = 1;

					if(temp->data != 94)
					{
						if(temp->is_this == 0)
							no_of_zeros--;
						else
							no_of_ones--;
					}

					temp = temp->parent;
				}
				else 
					rec_flag = 0;
			}

			if (temp == NULL)
			{
				cout<<"Insert_Array -> Tree Overflow "<<endl;
				return;
			}

			struct treenode *temp_delete;
			temp_delete = temp;

			if(temp_delete->left != NULL && temp_delete->left->status == 1)
				temp_delete = temp_delete->left;
			else
				temp_delete = temp_delete->right;

			struct treenode *left, *right;
			if(temp_delete->left != NULL)	
				left = temp_delete->left;
			if(temp_delete->right != NULL)	
				right = temp_delete->right;
			temp_delete->left = NULL;
			temp_delete->right = NULL;
			
			if(temp_delete->left != NULL)	
				del_rec(left);
			if(temp_delete->right != NULL)
				del_rec(right);

			backtrack_needed = 0;

			//cout<<"Backtrack ends"<<endl;
		}

		/*
		if(check_number_01(root,temp,no_of_zeros,no_of_ones,new_array) == 0)
		{
			cout<<"Error No of Zeroes/Ones not matching "<<endl;
		}
		else
		{
			cout<<"0/1 matching at last "<<endl;
		}*/
	}

	//cout<<"Function ends"<<endl;

	return;
}

int check_number_01(struct treenode *root,struct treenode *temp,int no_of_zeros,int no_of_ones,Eigen::VectorXd& new_array)
{
	struct treenode *temp_check;

	temp_check = root;
	int no_1 = 0;
	int no_0 = 0;
	int is_okay = 1;

	for(int i=1; i<=temp->data; i++)
	{
		int temp_now = new_array(i);
		
		if(temp_now == 0)
		{
			temp_check = temp_check->left;
			if(i!=1&&i!=94&&i!=115)
				no_0++;
		}
		else
		{
			temp_check = temp_check->right;
			if(i!=1&&i!=94&&i!=115)
				no_1++;
		}

		if(temp_check->is_this != temp_now)
		{	
			cout<<"Is This an Error! XD "<<endl;
			is_okay = 0;
		}
	}

	if(no_of_ones!=no_1||no_of_zeros!=no_0)
		is_okay = 0;

	return is_okay;
}

void find_open(struct treenode *root,Eigen::VectorXd& array,Eigen::VectorXd& new_array)
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	struct treenode *temp;
	temp = root;
	int temp_int;

	for(int i=1; i<=115; i++)
	{
		if(temp->status == 1)
			break;

		temp_int = array(i);

		if(temp_int == 0)
			temp = temp -> left;
		else if(temp_int == 1)
			temp = temp -> right;
	}

	new_array = array;
	temp = temp -> parent;
	double rand_find = 0;

	if(temp->left != NULL && temp->left->status == 1)
	{
		if(temp->right == NULL)
			temp->right = newNode(temp->data + 1,1);
		temp = temp -> right;
		new_array(temp->data) = 1;
	}
	else
	{
		if(temp->left == NULL)
			temp->left = newNode(temp->data + 1,0);
		temp = temp -> left;
		new_array(temp->data) = 0;
	}	

	while(temp->data != 115)
	{
		if((temp->left != NULL && temp->left->status == 1) || (temp->right != NULL && temp->right->status == 1))
		{	
			if(temp->left != NULL && temp->left->status == 1)
			{
				if( temp->right == NULL)
					temp->right = newNode(temp->data+1,1);
				temp = temp->right;
				new_array(temp->data) = 1;
			}
			else 
			{
				if( temp->left == NULL)
					temp->left = newNode(temp->data+1,0);
				temp = temp->left;
				new_array(temp->data) = 0;
			}
		}
		else
		{
			rand_find = rand() / (double)RAND_MAX;

			if(rand_find < rand_find_thr)   //Left
			{
				if( temp->left == NULL)
					temp->left = newNode(temp->data+1,0);
				temp = temp->left;
				new_array(temp->data) = 0;
			}
			else //Right
			{
				if( temp->right == NULL)
					temp->right = newNode(temp->data+1,1);
				temp = temp->right;
				new_array(temp->data) = 1;
			}
		}
	}

	if(temp->data != 115)
		cout<<"Problem in Open_FIND while loop"<<endl;
}


/*
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
		if(temp->status == 1)
			break;

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
*/

void delete_tree(struct treenode* root) //Done
{
	if(root == NULL)
	{
		cout<<"Tree Empty"<<endl;
		return;
	}

	del_rec(root);

	return;	
}

void del_rec(struct treenode* temp) //Done
{
	if(temp == NULL)
		return;
	
	del_rec(temp->left);
	del_rec(temp->right);
	free(temp);

	return;
}

long long int population(struct treenode *root) //Done
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

void pop_rec(struct treenode *temp) //Done
{
	if(temp == NULL)
		return;
	else if(temp->status == 1)
	{
		pop_counter += pow(2,(115 - temp->data));
		//if(temp->data != 115)
			cout<<"Pop Data = "<<temp->data<<endl;
		return;
	}
	else
	{
		pop_rec(temp->left);
		pop_rec(temp->right);
	}
}

string change_ds(Eigen::VectorXd& array)
{
	string str;
	for(int i=1;i<116;i++){
		if(array(i) == 1)
			str.push_back('1');
		else  str.push_back('0');
	}
	return str;
}


// Helper Functions

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

void random_generate_arrays(Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);
    arrays.fill(0);

    for(int i=1;i<=num_arr;i++)
    {
        random_generate(array);
        for(int j=1;j<=115;j++)
        {
            arrays(i,j)=array(j);
        }
        array = arrays.row(i);
        if(is_okay(array)==0)
            cout<<"ARRAY NOT OKAY ERROR"<<endl;
    }
    return;
}

int is_okay(Eigen::VectorXd& array)  //Returns 1 if okay 0 if not okay
{
    int flag=1,total_ones=0,total_zeros=0;
    if(array(1)!=0)
    {
        flag=0;
        cout<<"Problem 1"<<endl;
    }
    if(array(115) != 1 - array(94))
    {
        flag=0;
        cout<<"Problem 2"<<endl;
    }
    for(int i=2;i<=114;i++)
    {
        if(i==94)
            i++;
        if(int(array(i))==1)
            total_ones++;
        if(int(array(i))==1)
            total_zeros++;
    }
    if(total_ones!=56)
    {
        flag=0;
        cout<<"Problem 3"<<endl;
    }
    if(total_zeros!=56)
    {
        flag=0;
        cout<<"Problem 4"<<endl;
    }
    return flag;
}

void assign_result_arrays(Eigen::VectorXd& array)
{
	array.fill(0);
	FILE* fp;
	fp = fopen("nonrevisit_check.txt","r");
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