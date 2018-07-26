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

// ToDo Update status open/closed 
struct treenode
{
    int data;
    struct treenode* left;
    struct treenode* right;
};

    struct treenode *his_loc;
    long long int pop_counter = 0;

// ToDo check if this can be anything 
    int num_arr = 10; //Number of Arrays in the Pool

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
void check_archive(Eigen::VectorXd& array);

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void random_generate(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
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

    struct treenode history;
	his_loc = &history;
	his_loc = newNode(0);

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

void genetic_algo(Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2)
{

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
    cout<<"No of Diff == "<<place_1<<endl;

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
    for(int i=1;i<=2;i++)
    {
        
        int rand_p = int((rand() / (double)RAND_MAX)*place_1);
        temp_swap = array_diff_1at1[rand_p];
        array_diff_1at1[rand_p] = array_diff_1at2[rand_p];
        array_diff_1at2[rand_p] = temp_swap;
        
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

    // Mutation

    while(1)
    {
        double p = rand() / (double)RAND_MAX;
        if(p<mutation_probablity)
        {
            int place_1 = rand()%113 + 2;
            int place_2 = rand()%113 + 2;
            if((place_1!=94)&&(place_2!=94))
            {
                temp_swap = array_off[place_1];
                array_off[place_1] = array_off[place_2];;
                array_off[place_2] = temp_swap;

                temp_swap = array_off_2[place_1];
                array_off_2[place_1] = array_off_2[place_2];;
                array_off_2[place_2] = temp_swap;
            }
            
        }
        break;
    }

    if(check_similar_arrays(array,array_off))
        cout<<" PROBLEM ARRAY NOT CHANGING 1 "<<endl;

    if(check_similar_arrays(array_2,array_off_2))
        cout<<" PROBLEM ARRAY NOT CHANGING 2 "<<endl;

    if((is_okay(array_off)==0)||(is_okay(array_off_2)==0))
        cout<< " ERROR ARRAY NOT OKAY GEN_ALGO" <<endl;

    check_archive(array_off);
    check_archive(array_off_2);

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
        	check_archive(array);

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

    check_archive(array);
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
// ToDo update functions with status open/close
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

// 1 - present , 0 - not present
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
            //cout<<"Break at "<<i<<endl;
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

void check_archive(Eigen::VectorXd& array)
{
	if(is_present(his_loc,array))
    {
    	Eigen::VectorXd array_arch(116);
    	int count_arch = 1;
    	int neigh_arch = 5;
    	int flag_arch = 1;

    	while(is_present(his_loc,array)==1)
    	{
    		shake(array,array_arch,neigh_arch);
    		if(check_similar_arrays(array,array_arch)==1)
    			cout<<"Same Array No Shake"<<endl;
    		array = array_arch;
    		count_arch++;
    		if(count_arch%8==0)
    			neigh_arch++;
    		if(count_arch%100==0)
    		{	
    			flag_arch = 0;
    			break;
    		}
    		//cout<<"Count Arch == "<<count_arch<<endl;
    	}

    	if(flag_arch==1)	
    		insert_array(his_loc,array);
    }
    else
    	insert_array(his_loc,array);

    if(is_okay(array)==0)
        cout<< " ERROR ARRAY NOT OKAY CHECK_ARCHIVE" <<endl;
}
