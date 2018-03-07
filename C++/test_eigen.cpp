#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 
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
    double temp_okay=0;
    array_temp = inv_coeff_mat * array;
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
            cout<<"i == "<<i<<endl;
            cout<<"Error 1"<<endl;
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
            cout<<"Error 2"<<endl;
            return 1;
        }

    if((count[1]!=56)||(count[0]!=56))
    {   
        cout<<"Error 3  ->  "<<count[0]<<" -- "<<count[1]<<endl;
        return 1;
    }

    // If Here Than Both Arrays are Okay 
    return 0;
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
void random_generate_inv(Eigen::VectorXd& array) //Generates A Random Array which obeys all the specifications
{
    srand(time(NULL));
    array.fill(0);
    int rand_gen=0;
    for(int i=1;i<=115;i++)
    {
        rand_gen = rand() % 16;
        rand_gen +=57;
        array(i)=rand_gen;
    }
}
double cost_full_inv(Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array)
{
    Eigen::VectorXd array_temp(116);
    double temp_cost=0;
    array_temp = inv_coeff_mat * array;
    //cout<<array_temp<<endl;
    
    for(int i=1;i<=115;i++)
    {
        temp_cost=array_temp(i);
        //cout<<temp_cost<<" - ";
        if((temp_cost>=0)&&(temp_cost<=1))
        {
            if(temp_cost>=0.5)
                array_temp(i) = (1 - temp_cost)*(1 - temp_cost);
            else if(temp_cost<0.5)
                array_temp(i) = temp_cost*temp_cost;
        }
        if(temp_cost<0)
            array_temp(i) = ((-100)*temp_cost) * ((-100)*temp_cost) * ((-100)*temp_cost);
        if(temp_cost>1)
            array_temp(i) = ((temp_cost - 1)*100) * ((temp_cost - 1)*100) * ((temp_cost - 1)*100);
        temp_cost=array_temp(i);
        //cout<<temp_cost<<endl;
    }
    array_temp(0)=0;
    return array_temp.sum();
}
int random_number_inv()
{
    int rand_no = 0;
    rand_no = rand() % 115;
    rand_no +=1;
    return rand_no;
}
void genetic_algo_inv(int length,Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array)
{
    double cost_prev=0;
    double cost_after=0;
    int temp=0;
    int index[2];
    int rand_val_max = length/2;
    Eigen::VectorXd array_test(116);

    array_test = array;
    cost_prev = cost_full_inv(inv_coeff_mat,array_test);
    index[0]=0;index[1]=0;
    index[0]=random_number_inv();
    index[1]=index[0]+length-1;
    if(index[1]>115)
        index[1]=index[1]-115;
    int trav = index[0];
    while(trav!=(index[1]+1))
    {
        if(trav>115)
            trav=1;
        temp = array_test(trav);
        int rand_val = rand() % rand_val_max + 1;
        int rand_dir = rand() % 2;
        if(rand_dir==0)
            rand_dir--;
        int rand_gate = rand() % 2  ;
        
        temp = temp + rand_gate * rand_dir * rand_val;
        if(temp<57)
            temp = 57;
        else if(temp>72)
            temp = 72;
        array_test(trav)=temp;
        trav++;
    }
    cost_after = cost_full_inv(inv_coeff_mat,array_test);

    if(cost_after<cost_prev)
    {    
        array=array_test;
        cout<<"Cost == "<<cost_after<<endl;
    }
    return;
}

int main()
{
    Eigen::VectorXd array(116);
    random_generate_inv(array);
    
    Eigen::MatrixXd coeff_mat(116,116);
    Eigen::MatrixXd inv_coeff_mat(116,116);
    assign_coeff_mat(coeff_mat);
    inv_coeff_mat = coeff_mat.inverse();
    //cout<<"Random Generated Array == "<<is_okay_inv(inv_coeff_mat,array,4)<<endl<<array<<endl;

    while(1)
        genetic_algo_inv(10,inv_coeff_mat,array);

    return 0;
}