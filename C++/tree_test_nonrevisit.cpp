#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include <mcheck.h>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 




// 0 - Left, 1 - Right 
// data - At which Depth the Node is in the main Tree
// status : 0 - open, 1 - closed
struct treenode
{
    int data;
    bool status;
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
void no_op(enum mcheck_status status) {}

// Tree Functions
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


int main()
{
	srand(time(NULL));
	mcheck(&no_op);
	
	struct treenode current;
    archive = &current;
	archive = newNode(0);

	Eigen::VectorXd array(116);
	random_generate(array);

	Eigen::MatrixXd arrays(num_arr+1,116);
    random_generate_arrays(arrays);

    array = arrays.row(1);
    for(int i=1;i<=num_arr;i++)
    {
    	cout<<i<<" no Loop"<<endl;
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

    cout<<"Hi"<<endl;

    array = arrays.row(1);
    for(int i=1;i<=num_arr;i++)
    {
    	array(115) = 1 - array(115);
    	if(i%2==0)
    		array(114) = 1 - array(114);
    	if(i%4==0)
    		array(113) = 1 - array(113);
    	cout<<is_present(archive,array)<<endl;
    }

    cout<<"Population == "<<population(archive)<<endl;

    array = arrays.row(1);
    follow_print(archive,array);

	return 0;
}



// Tree Operation Functions
struct treenode* newNode(int data) // Done
{
  struct treenode* newnode = (struct treenode*)malloc(sizeof(struct treenode));

  newnode->data = data;
  newnode->status = 0;
  newnode->left = NULL;
  newnode->right = NULL;
  newnode->parent = NULL;

  return(newnode);
}

// It is assumed that this array is not present
void insert_array(struct treenode *root,Eigen::VectorXd& array) //Done
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
		
		if(temp_int == 0)
		{
			if(temp->left == NULL){
				temp->left = newNode(i);
				temp->left->parent = temp;
			}
			temp = temp->left;
		}
		else if(temp_int == 1)
		{
			if(temp->right == NULL){
				temp->right = newNode(i);
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
		if(temp == NULL /*|| (temp->left!=NULL && temp->right!=NULL)*/)
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
			cout<<"Break at "<<i<<endl;
			break;
		}
	}
	return flag;
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
			temp->right = newNode(temp->data + 1);
		temp = temp -> right;
		new_array(temp->data) = 1;
	}
	else
	{
		if(temp->left == NULL)
			temp->left = newNode(temp->data + 1);
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
					temp->right = newNode(temp->data+1);
				temp = temp->right;
				new_array(temp->data) = 1;
			}
			else 
			{
				if( temp->left == NULL)
					temp->left = newNode(temp->data+1);
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
					temp->left = newNode(temp->data+1);
				temp = temp->left;
				new_array(temp->data) = 0;
			}
			else //Right
			{
				if( temp->right == NULL)
					temp->right = newNode(temp->data+1);
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
//-------------------------------------     MAIN DELETE FUNCTION     ---------------------------------------------------------

void del_rec(struct treenode* temp) //Done
{
	//if(temp == NULL || temp->data >115)
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
		if(temp->data != 115)
			cout<<"Pop Data = "<<temp->data<<endl;
		return;
	}
	else
	{
		pop_rec(temp->left);
		pop_rec(temp->right);
	}
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
    }
    return flag;
}