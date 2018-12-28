// Complete Genetic Algorithms Implementation
// A pool of Arrays, matched up randomly in pairs, 
// Crossover (a certain percentage of the crossover happening) between two whole arrays
// only those position of the arrays which are not the same, 
// and crossovers in multiple of two, to maintain the property of same 0's and 1's
// without 1st, 94th, 115th entries, and check if offsprings better
// Replaced with lowest Cost Array  


#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
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

    int num_arr = 100; //Number of Arrays in the Pool

// ToDo Make these probabilities adaptive
    double mutation_probablity = 0.1;
    double prob_swap = 0.2;
    double prob_shake = 0.35;

    int max_shake = 10;
    int max_trench = 50;
    int max_neigh = 30;
    int shake_no = 1;
    int trench_no=1; 
    int neighbourhood_no = 1;
    int random_run_no=1;
    int is_debugging_on = 1;


// Tree Functions
struct treenode* newNode(int data,int is_this);
void insert_array_and_prune(struct treenode *root,Eigen::VectorXd& array);
string change_ds(Eigen::VectorXd& array);
int is_present(struct treenode *root,Eigen::VectorXd& array);  // 1 - present , 0 - not present
void find_open_okay(struct treenode *root,Eigen::VectorXd& array,Eigen::VectorXd& new_array);
void find_open_insert_prune(struct treenode *root,Eigen::VectorXd& array);
long long int population(struct treenode *root); // How many arrays are stored here
void delete_tree(struct treenode *root);
void pop_rec(struct treenode *temp);
void del_rec(struct treenode *temp);
void follow_print(struct treenode *root,Eigen::VectorXd& array);
void check_stray_node(struct treenode *root);
void stray_rec(struct treenode *temp);
int check_number_01(struct treenode *root,struct treenode *temp,int no_of_zeros,int no_of_ones,Eigen::VectorXd& new_array);


// Already Present Functions
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void random_generate(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);                                    //Returns 1 if okay 0 if not okay
void assign_results_new(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
void print_all_arrays(Eigen::MatrixXd& arrays);
void random_generate_arrays(Eigen::MatrixXd& arrays);
void create_random_pairs(int *pairs);
void cost_init_gen(Eigen::MatrixXd& coeff_mat,double *cost_array,Eigen::MatrixXd& arrays);
void genetic_algo(Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2);
int check_similar_arrays(Eigen::VectorXd& array,Eigen::VectorXd& array_2);
void print_cost_all_arrays(int *lowest_cost,Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays);
void step_genetic(Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays);
int check_array(int rand_gen,int *array_random,int size);
void swap_fixed_pos(Eigen::VectorXd& array);

void shake(Eigen::VectorXd& array_main,Eigen::VectorXd& array,int k_neighbourhood);
void shake_population(Eigen::MatrixXd& arrays);



int main()
{
    int count = 0;
    srand(time(NULL));
    int lowest_cost_arrays;

    //Load Co-efficient Matrix, Initialize Arrays and Variables
    Eigen::MatrixXd coeff_mat(116,116);
    Eigen::MatrixXd arrays(num_arr+1,116);
    Eigen::VectorXd array(116);
    Eigen::VectorXd array_main(116);

    struct treenode archive_curr;
	archive = &archive_curr;
	archive = newNode(0,2);

    assign_coeff_mat(coeff_mat);
    random_generate_arrays(arrays);
    print_cost_all_arrays(&lowest_cost_arrays,coeff_mat,arrays);
    cout<<"Starts"<<endl;

    while(lowest_cost_arrays!=0)
    {
        step_genetic(coeff_mat,arrays);
        count++;

        if(count%10==0)
        {
        	print_cost_all_arrays(&lowest_cost_arrays,coeff_mat,arrays); 
        }

        if(count%100000==0)
        {
            count = 1;
            shake_no++;
            print_cost_all_arrays(&lowest_cost_arrays,coeff_mat,arrays); 
            shake_population(arrays);
            cout<<"After Shake "<<endl;
            print_cost_all_arrays(&lowest_cost_arrays,coeff_mat,arrays); 

            if(shake_no>=max_shake) 
            {
                trench_no++;
                shake_no = 1;
            }

            if(trench_no>=max_trench)
            {
                neighbourhood_no++;
                trench_no = 1;
                shake_no = 1;
            }

            if(neighbourhood_no>=max_neigh)
            {
                neighbourhood_no = 1;
                trench_no = 1;
                shake_no = 1; 
                random_run_no++;

    			random_generate_arrays(arrays);
    			print_cost_all_arrays(&lowest_cost_arrays,coeff_mat,arrays);
    			cout<<"Starts"<<endl;
            }

        }
    }



    return 0;
}

// ToDo Make this non-elitist and allow same-cost different arrays to co-exist in the population
void step_genetic(Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays)
{
    int pair_no = num_arr/2;
    int pairs[pair_no];
    int temp=0;
    int flag1=0;
    int flag2=0;

    create_random_pairs(pairs);

    Eigen::VectorXd array(116);
    Eigen::VectorXd array_2(116);

    Eigen::VectorXd array_off(116);
    Eigen::VectorXd array_off_2(116);

    double cost_array[num_arr+1];
    double new_cost[2];

    cost_init_gen(coeff_mat,cost_array,arrays);

    for(int i=0;i<pair_no;i++)
    {
        flag1 = 0;
        flag2 = 0;
        temp = pairs[i]; 
        array = arrays.row(temp);
        array_2 = arrays.row(temp+pair_no);

        genetic_algo(array,array_2,array_off,array_off_2);
        new_cost[0]=cost_full(coeff_mat,array_off);
        new_cost[1]=cost_full(coeff_mat,array_off_2);

        for(int j=1;j<=num_arr;j++)
        {
            if(new_cost[0]==cost_array[j])
                flag1=1;
            if(new_cost[1]==cost_array[j])
                flag2=1;
        }

        for(int j=1;j<=num_arr;j++)
        {
            if((cost_array[j]>new_cost[0])&&(flag1==0))
            {
                for(int k=1;k<=115;k++)
                    arrays(j,k)=array_off(k);
                flag1=1;
                continue;
            }
            if((cost_array[j]>new_cost[1])&&(flag2==0))
            {
                for(int k=1;k<=115;k++)
                    arrays(j,k)=array_off_2(k);
                flag2=1;
            }
        }
    }
    return;
}

//ToDo Make the probablity of Swap Dependent on how different the arrays are so there's always change
void genetic_algo(Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2)
{
    if((is_okay(array)==0)||(is_okay(array_2)==0))
        cout<< " ERROR ARRAY NOT OKAY GEN_ALGO AT FIRST" <<endl;

    array_off = array;
    array_off_2 = array_2;
    int counter = 0;
    int array_diff_1at1[57];
    int array_diff_1at2[57];
    int place_1 = 1;
    int place_2 = 1;
    int temp_swap;

    for(int i=2;i<=114;i++)
    {
        if((array(i)!=array_2(i))&&(i!=94))
        {
            if(array(i)==1)
            {
                array_diff_1at1[place_1] = i;
                place_1++;
            }
            else if(array_2(i)=1)
            {
                array_diff_1at2[place_2] = i;
                place_2++;
            }
        }
    }

    if(place_1!=place_2)
        cout<<"ERROR DIFF"<<endl;
    place_1--;
    //cout<<"No of Diff == "<<place_1<<endl;

    for(int i=1;i<=place_1;i++)
    {
        double p = rand() / (double)RAND_MAX;

        if(p<prob_swap)
        {
            temp_swap = array_diff_1at1[i];
            array_diff_1at1[i] = array_diff_1at2[i];
            array_diff_1at2[i] = temp_swap;
        }
        counter++;
    }

    for(int i=1;i<=place_1;i++)
    {
        temp_swap = array_diff_1at1[i];
        array_off(temp_swap) = 1;
        array_off_2(temp_swap) = 0;

        temp_swap = array_diff_1at2[i];
        array_off(temp_swap) = 0;
        array_off_2(temp_swap) = 1;
    }

    // Making Sure One Swap happened atleast for both arrays

    if(check_similar_arrays(array,array_off))
    {
        while(1)
        {
            place_1 = rand()%113 + 2;
            place_2 = rand()%113 + 2;

            if((place_1!=94)&&(place_2!=94)&&(array_off(place_1)!=array_off(place_2))&&(place_1!=place_2))
            {
                temp_swap = array_off(place_1);
                array_off(place_1) = array_off(place_2);
                array_off(place_2) = temp_swap;

                break;
            }
        } 
    }

    if(check_similar_arrays(array_2,array_off_2))
    {
        while(1)
        {
            place_1 = rand()%113 + 2;
            place_2 = rand()%113 + 2;

            if((place_1!=94)&&(place_2!=94)&&(array_off_2(place_1)!=array_off_2(place_2))&&(place_1!=place_2))
            {
                temp_swap = array_off_2(place_1);
                array_off_2(place_1) = array_off_2(place_2);
                array_off_2(place_2) = temp_swap;

                break;
            }
        }
    }

    if((is_okay(array_off)==0)||(is_okay(array_off_2)==0))
        cout<< " ERROR ARRAY NOT OKAY GEN_ALGO" <<endl;

    if(is_present(archive,array_off))
    {
        find_open_insert_prune(archive,array_off);
    }
    else
    {
        insert_array_and_prune(archive,array_off);
    }

    if(is_present(archive,array_off_2))
    {
        find_open_insert_prune(archive,array_off_2);
    }
    else
    {
        insert_array_and_prune(archive,array_off_2);
    }

    return;
}

// If same Array then 1 else 0 
int check_similar_arrays(Eigen::VectorXd& array,Eigen::VectorXd& array_2)
{
    int flag = 1;

    for(int i=0; i<=115; i++)
    {
        if(array(i)!=array_2(i))
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

void cost_init_gen(Eigen::MatrixXd& coeff_mat,double *cost_array,Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);

    for(int i=1;i<=num_arr;i++)
    {
        array = arrays.row(i);
        cost_array[i] = cost_full(coeff_mat,array);
    }

    return;
}

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

    if(trench_no%2==0)   //For Alternates Of Fixed Positions 94 and 115 
        swap_fixed_pos(array);
    
}

void shake_population(Eigen::MatrixXd& arrays)
{
	Eigen::VectorXd array(116);
	Eigen::VectorXd shaked_array(116);
    int temp;

    for(int i=2;i<=num_arr;i++)
    {
    	double p_shake = rand() / (double)RAND_MAX;

    	if(p_shake<prob_shake)
    	{
        	array = arrays.row(i);

        	shake(array,shaked_array,neighbourhood_no);
        	if(is_present(archive,shaked_array))
            {
                find_open_insert_prune(archive,shaked_array);
            }
            else
            {
                insert_array_and_prune(archive,shaked_array);
            }

        	for(int j=1;j<=115;j++)
        	{
        	    arrays(i,j)=shaked_array(j);
        	}
        }
    }
}

void print_all_arrays(Eigen::MatrixXd& arrays)
{
    for(int i=1;i<=num_arr;i++)
    {
        cout<<"ARRAY NO "<<i<<" is : ";
        for(int j=1;j<=115;j++)
            cout<<arrays(i,j);
        cout<<endl;
    }
}

void swap_fixed_pos(Eigen::VectorXd& array)
{
    array(94)=1-array(94);
    array(115)=1-array(115);
}

void print_cost_all_arrays(int *lowest_cost,Eigen::MatrixXd& coeff_mat,Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);
    int temp;
    *lowest_cost = 10000;

    for(int i=1;i<=num_arr;i++)
    {
        array = arrays.row(i);
        //cout<<array<<endl<<"-------------------"<<endl;
        temp = cost_full(coeff_mat,array);
        cout<<temp<<"  ";
        if(temp<*lowest_cost)
            *lowest_cost = temp;
    }
    cout<<endl<<endl;

    return;
}

void create_random_pairs(int *pairs)
{
    //srand(time(NULL));
    int pair_no = num_arr/2;
    int rand_no = 0;
    int flag = 1;

    for(int j=0;j<pair_no;j++)
        pairs[j]=0;

    for(int i=0;i<pair_no;i++)
    {
        while(1)
        {
            flag = 1;
            rand_no = rand()%pair_no + 1;
            for(int j=0;j<pair_no;j++)
            {    
                if(pairs[j]==rand_no)
                    flag = 0;
            }
            if(flag==1)
            { 
                pairs[i]=rand_no;
                break;
            }
        }
    }
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
        if(is_debugging_on == 1)
            cout<<"Problem 1"<<endl;
    }
    if(array(115) != 1 - array(94))
    {
        flag=0;
        if(is_debugging_on == 1)
            cout<<"Problem 2"<<endl;
    }
    for(int i=2;i<=114;i++)
    {
        if(i==94)
            i++;
        if(int(array(i))==1)
            total_ones++;
        if(int(array(i))==0)
            total_zeros++;
    }
    if(total_ones!=56)
    {
        flag=0;
        if(is_debugging_on == 1)
            cout<<"Problem 3"<<total_ones<<endl;
    }
    if(total_zeros!=56)
    {
        flag=0;
        if(is_debugging_on == 1)
            cout<<"Problem 4"<<total_zeros<<endl;
    }
    return flag;
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

    //ToDo check_archive(array);
    if(is_present(archive,array))
    {
        find_open_insert_prune(archive,array);
    }
    else
    {
        insert_array_and_prune(archive,array);
    }
    
    return;
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

void assign_results_new(Eigen::MatrixXd& coeff_mat)
{
    FILE* fp;
    fp = fopen("results_found_by_me_lowest_org.txt","r");
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
            ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);
            ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);ch=fgetc(fp);
            columns=1;
            rows++;
        }
    }
    //cout<<"Number of Characters == "<<count<<endl;
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

void check_inconsistencies(struct treenode *root)
{

    check_rec(root,0,2);
}

void check_rec(struct treenode *temp,int level, int what_is_this)
{
    int continue_no = 0;

    if(temp == NULL)
        return;

    if(temp->status == 1)
    {
        if(temp->left!=NULL || temp->right!=NULL)
            continue_no = 1;
    }

    if(temp->status == 0)
    {
        if(temp->left==NULL && temp->right==NULL && temp->data != 115)
            continue_no = 1;

        if(temp->left!=NULL && temp->right!=NULL)
        {
            if(temp->left->status == 1 && temp->right->status == 1)
                continue_no = 1;
        }
    }

    if(temp->left!= NULL)
    {
        if(temp->left->parent != temp)
            continue_no = 1;
    }

    if(temp->right!=NULL)
    {
        if(temp->right->parent != temp)
            continue_no = 1;
    }   

    if(temp->data != level)
        continue_no = 1;

    if(temp->is_this != what_is_this)
        continue_no = 1;

    if(continue_no == 0)
    {
        check_rec(temp->left,level+1,0);
        check_rec(temp->right,level+1,1);
    }
    else
    {
        cout<<"Archive Inconsistent Fuck!!!! "<<endl;
        archive_inconsistent = 1;
    }

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

        if(temp->data == 115 && temp->status==0)
            flag = 0;
    }
    return flag;
}

void check_and_amend_count(int temp_data,int prev_zeroes,int prev_ones,int prev_is_94_1,Eigen::VectorXd& prev_array)
{
    int no_of_zeros = 0;
    int no_of_ones = 0;
    int is_94_1 = 2;
    int temp_int;
    
    //Initializing the ones/zeroes counters at start
    for(int c = 2;c <= temp_data;c++)
    {
        temp_int = prev_array(c);

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

    if(no_of_ones!=prev_ones || no_of_zeros!=prev_zeroes || is_94_1!=prev_is_94_1)
    {
        cout<<"Count not okay Changing Count "<<endl;
        cout<<"In function == "<<endl;
        cout<<prev_ones<<" "<<prev_zeroes<<" "<<prev_is_94_1<<endl;
        cout<<"In check function == "<<endl;
        cout<<no_of_ones<<" "<<no_of_zeros<<" "<<is_94_1<<endl;

        no_of_ones=prev_ones;
        no_of_zeros=prev_zeroes; 
        is_94_1=prev_is_94_1;
    }
    else 
    {
        if(is_debugging_on == 1)
            cout<<"Counts Okay Check Done"<<endl;
    }

    return;
}

void find_open_insert_prune(struct treenode *root,Eigen::VectorXd& array)
{
    Eigen::VectorXd new_array(116);
    find_open_okay(root,array,new_array);
    if(is_okay(new_array)==0)
        cout<<"Error Array by find_open_okay is not okay"<<endl;
    insert_array_and_prune(root,new_array);
    array = new_array;
    return;
}

void find_open_okay(struct treenode *root,Eigen::VectorXd& array,Eigen::VectorXd& new_array)
{
    if(root == NULL)
    {
        cout<<"Tree Empty"<<endl;
        return;
    }

    if(is_debugging_on == 1)
        cout<<"Function Starts"<<endl;

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

    // Going to place till where Array matches, steps back 1 step from where find starts
    new_array = array;
    temp = temp -> parent;
    double rand_find = 0;   

    if(is_debugging_on == 1)
        cout<<"Status 1 Discovered at Below -> "<<temp->data<<endl;

    int no_of_zeros = 0;
    int no_of_ones = 0;
    int is_94_1 = 2;
    int backtrack_needed = 0;
    
    //Initializing the ones/zeroes counters at start
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

    if(is_debugging_on == 1)
        cout<<"While Loop In Function Starts"<<endl;

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
            if(is_debugging_on == 1)
                cout<<"114 statement Starts"<<endl;

            if(temp->status == 1)
            {   
                backtrack_needed = 1;

                if(is_debugging_on == 1)
                    cout<<"114 statement status = 1 at first"<<endl;
            }
            else if( is_94_1 == 1 )
            {
                if(is_debugging_on == 1)
                    cout<<"114 statement is_94_1 = 1 at first"<<endl;

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
                if(is_debugging_on == 1)
                    cout<<"114 statement is_94_1 = 0 at first"<<endl;

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
                archive_inconsistent = 1;
                return;
            }

            // ToDo possible breach in counting
            if(temp->data == 115)
                temp = temp->parent;

            if(temp->is_this == 1)
            {
                while(temp->is_this != 0)
                {   
                    if(temp->data!=94)
                        no_of_ones--;
                    temp = temp->parent;
                }
            }
            else if(temp->is_this == 0)
            {
                while(temp->is_this != 1)
                {
                    if(temp->data!=94)
                        no_of_zeros--;
                    temp = temp->parent;
                }
            }

            //check_and_amend_count(temp->data,no_of_zeros,no_of_ones,is_94_1,new_array);

            if(is_debugging_on == 1)
                cout<<"114 statement at last deleting from -> "<<temp->data<<endl;

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

            if(is_debugging_on == 1)
                cout<<"114 Statement ends"<<endl;

        }
        else if(no_of_zeros==56||no_of_ones==56) // Only one possibility of Okay Array
        {
            if(is_debugging_on == 1)
                cout<<"0/1 56 statement starts at  "<<temp->data<<endl;

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
                if(is_debugging_on == 1)
                    cout<<"only okay is not present "<<endl;

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
                if(is_debugging_on == 1)
                    cout<<"Only okay is present go back"<<endl;

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

            if(is_debugging_on == 1)
                cout<<"0/1 56 statement ends"<<endl;

        }
        else if((temp->left != NULL && temp->left->status == 1) || (temp->right != NULL && temp->right->status == 1))
        {   
            if(is_debugging_on == 1)
                cout<<"Normal statement starts "<<endl;

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

            if(is_debugging_on == 1)
                cout<<"Normal statement ends "<<endl;
        }
        else
        {
            if(is_debugging_on == 1)
                cout<<"Normal random statement starts "<<endl;

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

            if(is_debugging_on == 1)
                cout<<"Normal random statement ends "<<endl;
        }

        if(backtrack_needed == 1)
        {
            if(is_debugging_on == 1)
                cout<<"Backtrack starts "<<endl;

            bool rec_flag = 1;

            if(temp->data != 94)
            {
                if(temp->is_this == 0)
                    no_of_zeros--;
                else
                    no_of_ones--;
            }

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

            // Deleting the Backtracked Path (One below current temp position)
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

            if(is_debugging_on == 1)
                cout<<"Backtrack ends"<<endl;
        }

    }

    if(is_debugging_on == 1)
        cout<<"Function ends"<<endl;

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
            //cout<<"Pop Data = "<<temp->data<<endl;
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