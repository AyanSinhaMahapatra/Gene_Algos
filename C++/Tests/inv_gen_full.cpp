#include<iostream>
#include<stdio.h>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

    int num_arr = 10; //Number of Arrays in the Pool
    int overlap_len = 5;

int cost_full(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);  //Returns 1 if okay 0 if not okay
void estimate_binary(Eigen::VectorXd& array_binary);
void correct_binary(Eigen::VectorXd& array_binary);
double give_value_with_tol(double value, int tolerance, char sign);
int check_if_close(double to_check,int check_with,int tolerance);
int is_okay_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,int tolerance);
void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
void random_generate_inv(Eigen::VectorXd& array);
double cost_full_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array);
double cost_analyze(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array);
int random_number_inv();
void random_generate_inv_arrays(Eigen::MatrixXd& arrays);
void genetic_algo_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2);
void create_random_pairs(int *pairs);
void step_genetic(Eigen::MatrixXd& inv_coeff_mat,Eigen::MatrixXd& arrays);
void cost_init_gen(Eigen::MatrixXd& inv_coeff_mat,double *cost_array,Eigen::MatrixXd& arrays);
int random_number(int start,int end,int length,int order);
void random_parents(int *parents_index,int p_length);
void print_cost_all_arrays(Eigen::MatrixXd& inv_coeff_mat,Eigen::MatrixXd& arrays);


int main()
{
    srand(time(0));
    int count = 0;
    Eigen::MatrixXd coeff_mat(116,116);
    Eigen::MatrixXd inv_coeff_mat(116,116);
    assign_coeff_mat(coeff_mat);
    inv_coeff_mat = coeff_mat.inverse();

    Eigen::MatrixXd arrays(num_arr+1,116);
    Eigen::VectorXd array(116);

    random_generate_inv_arrays(arrays);
    print_cost_all_arrays(inv_coeff_mat,arrays);

    cout<<"Starts"<<endl;

    while(1)
    {
        step_genetic(inv_coeff_mat,arrays);
        count++;

        if(count%1==0)
        {
            print_cost_all_arrays(inv_coeff_mat,arrays);    
        }
        //cout<<"Step Main"<<endl;
    }
    
    return 0;
}

void print_cost_all_arrays(Eigen::MatrixXd& inv_coeff_mat,Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);

    for(int i=1;i<=num_arr;i++)
    {
        array = arrays.row(i);
        //cout<<array<<endl<<"-------------------"<<endl;
        cout<<cost_analyze(inv_coeff_mat,array)<<"  ";
    }
    cout<<endl<<endl;

    return;
}

void step_genetic(Eigen::MatrixXd& inv_coeff_mat,Eigen::MatrixXd& arrays)
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

    cost_init_gen(inv_coeff_mat,cost_array,arrays);

    for(int i=0;i<pair_no;i++)
    {
        flag1 = 0;
        flag2 = 0;
        temp = pairs[i]; 
        array = arrays.row(temp);
        array_2 = arrays.row(i+pair_no);

        genetic_algo_inv(inv_coeff_mat,array,array_2,array_off,array_off_2);

        new_cost[0]=cost_analyze(inv_coeff_mat,array_off);
        new_cost[1]=cost_analyze(inv_coeff_mat,array_off_2);
        //cout<<new_cost[0]<<" - "<<new_cost[1]<<endl;

        for(int j=1;j<=num_arr;j++)
        {
            if((cost_array[j]>new_cost[0])&&(flag1==0))
            {
                for(int k=1;k<=115;k++)
                    arrays(j,k)=array_off(k);
                flag1=1;
            }
            else if((cost_array[j]>new_cost[1])&&(flag2==0))
            {
                for(int k=1;k<=115;k++)
                    arrays(j,k)=array_off_2(k);
                flag2=1;
            }
        }
    }

    //print_cost_all_arrays(inv_coeff_mat,arrays);

    return;
}

void genetic_algo_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2)
{
    int parents_index[4];
    int temp=0;

    array_off = array;
    array_off_2 = array_2;
    
    random_parents(parents_index,overlap_len);
    //cout<<parents_index[0]<<"  "<<parents_index[1]<<"  "<<parents_index[2]<<"  "<<parents_index[3]<<"  "<<endl;

    int pointer = parents_index[0];
    while(pointer!=(parents_index[1]+1))
    {
        if(pointer == 116)
            pointer = 1;
        temp = array_off(pointer);
        array_off(pointer) = array_off_2(pointer);
        array_off_2(pointer) = temp;
        pointer++;
    }

    pointer = parents_index[2];
    while(pointer!=(parents_index[3]+1))
    {
        if(pointer == 116)
            pointer = 1;
        temp = array_off(pointer);
        array_off(pointer) = array_off_2(pointer);
        array_off_2(pointer) = temp;
        pointer++;
    }
    return;
}

int random_number(int start,int end,int length,int order)
{
    int rand_no =0;
    int rand_range =0;

    if(start>end)
        rand_range = (116-start) + (end-1);
    else
        rand_range = end-start;
    if(order==2)
        rand_range = rand_range - length ;

    rand_no = rand() % rand_range;
    rand_no = rand_no + start;  

    if(rand_no>115)
        rand_no = 1 + rand_no % 115;

    return rand_no;
}

void random_parents(int *parents_index,int p_length)
{
    //Parent 1
    int p1_start = random_number(1,115,p_length,1);
    
    int p1_end = p1_start + p_length - 1;

    if(p1_end>115)
        p1_end = p1_end % 115;
    //Parent 2
    int p2_start = random_number(p1_end+1,p1_start-1,p_length,2);

    int p2_end = p2_start + p_length - 1;

    if(p2_end>115)
        p2_end = p2_end % 115;

    parents_index[0] = p1_start;
    parents_index[1] = p1_end;
    parents_index[2] = p2_start;
    parents_index[3] = p2_end;

    return;
}

void cost_init_gen(Eigen::MatrixXd& inv_coeff_mat,double *cost_array,Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);

    for(int i=1;i<=num_arr;i++)
    {
        array = arrays.row(i);
        cost_array[i] = cost_analyze(inv_coeff_mat,array);
    }

    return;
}

void random_generate_inv_arrays(Eigen::MatrixXd& arrays)
{
    Eigen::VectorXd array(116);

    for(int i=1;i<=num_arr;i++)
    {
        random_generate_inv(array);
        arrays(i,0)=0;
        for(int j=1;j<=115;j++)
        {
            arrays(i,j)=array(j);
        }
    }

    return;
}

void random_generate_inv(Eigen::VectorXd& array) //Generates A Random Array which obeys all the specifications
{
    array.fill(0);
    int rand_gen=0;
    for(int i=1;i<=115;i++)
    {
        rand_gen = rand() % 8;
        rand_gen +=60;
        array(i)=rand_gen;
    }

    return;
}

void create_random_pairs(int *pairs)
{
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

    return;
}

int cost_full(Eigen::VectorXd& array)
{
    array(0)=0;
    Eigen::MatrixXd coeff_mat(116,116);
    coeff_mat.fill(0);
    assign_coeff_mat(coeff_mat);
    Eigen::VectorXd array_temp(116);
    array_temp.fill(0);
    int temp_cost=0;
    array_temp = coeff_mat * array;
    for(int i=1;i<=115;i++)
    {
        temp_cost=array_temp(i);
        
        if((temp_cost>=57)&&(temp_cost<=72))
            array_temp(i)=0;
        if(temp_cost<57)
            array_temp(i)=(57-temp_cost)*(57-temp_cost);
        if(temp_cost>72)
            array_temp(i)=(temp_cost-72)*(temp_cost-72);

        //cout<<array_temp(i)<<endl;
    }
    return array_temp.sum();
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
void correct_binary(Eigen::VectorXd& array_binary)
{
    cout<<"Error Error Error ==================================================================="<<endl;
    cout<<"Corrected"<<endl;
    return;
}
void estimate_binary(Eigen::VectorXd& array_binary)
{
    double temp = 0;
    for(int i=1;i<=115;i++)
    {
        temp = array_binary(i);
        if(temp>=0.5)
            array_binary(i)=1;
        else if(temp<0.5)
            array_binary(i)=0;
    }

    if(is_okay(array_binary))
        return;
    else
    {
        cout<<"Correcting Array :: "<<endl;
        correct_binary(array_binary);
    }
    return;
}
double give_value_with_tol(double value, int tolerance, char sign)
{
    if(sign=='p')
        value = value + pow(0.1,tolerance);
    else if(sign=='n')
        value = value - pow(0.1,tolerance);
    return value;
}
int check_if_close(double to_check,int check_with,int tolerance)
{
    if((to_check<give_value_with_tol(check_with,tolerance,'p'))&&(to_check>give_value_with_tol(check_with,tolerance,'n')))
        return 1;
    else 
        return 0;
}
// 2 - Problem In Array. 1 - Problem in Binary Array. 0 - Okay.
int is_okay_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array,int tolerance)
{
    Eigen::VectorXd array_temp(116);
    //cout<<inv_coeff_mat<<endl;
    double temp_okay=0;
    array_temp = inv_coeff_mat * array;
    //cout<<array_temp<<endl;
    //swap_array_to_whatever(array_temp,94,115);
    //change_array_to_whatever(array_temp,115,0.00001);

    // Checking Array (That has Numbers between 57 - 72) 
    for(int i=1;i<=115;i++)
    {
        temp_okay = array(i);
        if((temp_okay>72)||(temp_okay<57))
        {
            cout<<i<<endl;
            cout<<temp_okay<<endl;
            return 2;
        }
    }

    // Checking Binary Array
    int count[2];
    count[0]=0;
    count[1]=0;
    double tol1 = 1 + pow(0.1,tolerance);
    double tol0 = 0 - pow(0.1,tolerance);
    for(int i=1;i<=115;i++)
    {
        temp_okay = array_temp(i);
        if((temp_okay>tol1)||(temp_okay<tol0))
        {   
            //cout<<"i == "<<i<<endl;
            //cout<<"Value == "<<temp_okay<<endl;
            //cout<<"Error 1"<<endl;
            return 1;
        }
        if((i!=1)&&(i!=94)&&(i!=115))
        {
            if(temp_okay>=0.5)
                count[1]++;
            if(temp_okay<0.5)
                count[0]++;
        }
    }
    if(((check_if_close(array_temp(94),1,3))&&(check_if_close(array_temp(115),0,3)))||((check_if_close(array_temp(115),1,3))&&(check_if_close(array_temp(94),0,3))))
        temp_okay = 0;
    else 
        {
            //cout<<"Error 2"<<endl;
            //cout<<"94 == "<<array_temp(94)<<endl;
            //cout<<"115 == "<<array_temp(115)<<endl;
            //cout<<"1 == "<<array_temp(1)<<endl;
            //cout<<"Counts are Following "<<count[0]<<" -- "<<count[1]<<endl;
            return 1;
        }

    if((count[1]!=56)||(count[0]!=56))
    {   
        //cout<<"Error 3  ->  "<<count[0]<<" -- "<<count[1]<<endl;
        return 1;
    }

    // If Here Than Both Arrays are Okay 
    return 0;
}
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
    return;
}

double cost_full_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array)
{
    Eigen::VectorXd array_temp(116);
    int cost_haha = 0;
    double temp_cost=0;
    array_temp = inv_coeff_mat * array;
    estimate_binary(array_temp);
    cout<<"Samples after Estimation -  "<<array_temp(1)<<" - "<<array_temp(94)<<" - "<<array_temp(115)<<endl;

    if(is_okay(array_temp))
    {
        cost_haha = cost_full(array_temp);
        if(cost_haha==0)
        {
            cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
            cout<<array_temp<<endl;
        }
        else if(cost_haha<20)
        {
            cout<<"Cost ==== "<<cost_haha<<endl;
            for(int i=1;i<=115;i++)
                cout<<array_temp(i);
            cout<<endl;
        }
        else
            cout<<"Cost ==== "<<cost_haha<<endl;
    }
    else
    {
        cout<<"Not Okay"<<endl;
    }

    return cost_haha;
}
double cost_analyze(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array)
{
    double cost_extra = 0;
    Eigen::VectorXd array_temp_1(116);
    Eigen::VectorXd array_temp_2(116);
    Eigen::VectorXd array_temp(116);
    double temp_cost=0;
    array_temp_1.fill(0);
    array_temp_2.fill(0);
    array_temp = inv_coeff_mat * array;
    //cout<<array_temp<<endl;
    
    int count[2];
    count[0]=0;
    count[1]=0;
    for(int i=1;i<=115;i++)
    {
        temp_cost=array_temp(i);

        if((i!=1)&&(i!=94)&&(i!=115))
        {
            if(temp_cost>=0.5)
                count[1]++;
            if(temp_cost<0.5)
                count[0]++;
        }
        //cout<<temp_cost<<" - ";
        if((temp_cost>=0)&&(temp_cost<=1))
        {
            if(temp_cost>=0.5)
                array_temp_1(i) = (1 - temp_cost)*(1 - temp_cost);
            else if(temp_cost<0.5)
                array_temp_1(i) = temp_cost*temp_cost;
        }
        if(temp_cost<0)
            array_temp_2(i) = ((-100)*temp_cost) * ((-100)*temp_cost) * ((-100)*temp_cost);
        if(temp_cost>1)
            array_temp_2(i) = ((temp_cost - 1)*100) * ((temp_cost - 1)*100) * ((temp_cost - 1)*100);
        //temp_cost=array_temp(i);
        //cout<<temp_cost<<endl;
        
    }

    double temp_1 = array_temp(1);
    double temp_94 = array_temp(94);
    double temp_115 = array_temp(115);

    if(((temp_94>=0.9)&&(temp_115<0.1))||((temp_115>=0.9)&&(temp_94<0.1)))
        cost_extra += 0;
    else{
        cost_extra+= 2000; 
    }

    if(temp_1>0.1)
        cost_extra+= 1000; 

    if(count[0]<count[1])
        cost_extra += 1000*(count[1]-count[0]);
    else if(count[0]>count[1])
        cost_extra += 1000*(count[0]-count[1]);

    array_temp(0)=0;
    array_temp_1(0)=0;
    array_temp_2(0)=0;
    //cout<<"Cost Divisions are like == "<<array_temp_1.sum()<<" --- "<<array_temp_2.sum()<<endl;
    return (array_temp_1.sum()+array_temp_2.sum()+cost_extra);
}
int random_number_inv()
{
    int rand_no = 0;
    rand_no = rand() % 115;
    rand_no +=1;
    return rand_no;
}
