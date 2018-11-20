#include<iostream>
#include<stdio.h>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

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
void genetic_algo_inv(int length,Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array);
int main()
{
    double lowest_cost = 100;
    double cost_hold = 0;
    double cost_hold_check = 0;
    int count = 0;
    Eigen::VectorXd array(116);
    
    Eigen::MatrixXd coeff_mat(116,116);
    Eigen::MatrixXd inv_coeff_mat(116,116);
    assign_coeff_mat(coeff_mat);
    inv_coeff_mat = coeff_mat.inverse();
    //cout<<"Random Generated Array == "<<is_okay_inv(inv_coeff_mat,array,4)<<endl;

    while(1)
    {
        random_generate_inv(array);
        //cout<<"------------------------------------------------------------------------"<<endl;
        while(1)
        {
            count++;
            genetic_algo_inv(10,inv_coeff_mat,array);
            //genetic_algo_inv(20,inv_coeff_mat,array);
            if(count%100000==0)
            {
                //cout<<count<<endl;
                cost_hold_check = cost_hold;
                cost_hold = cost_analyze(inv_coeff_mat,array); 
                //cout<<"Analyze cost "<<cost_hold<<endl;
                //cout<<"Is_okay? -- "<<is_okay_inv(inv_coeff_mat,array,4)<<endl;
                if(cost_hold==cost_hold_check)
                {
                    
                    if(cost_full_inv(inv_coeff_mat,array)==0)
                        return 0;

                    //cout<<"Is_okay? -- "<<is_okay_inv(inv_coeff_mat,array,4)<<endl;
                    if(cost_hold<lowest_cost)
                    {
                        lowest_cost=cost_hold;
                        cout<<" Cost Holder    "<<lowest_cost<<"-----"<<cost_hold<<endl;
                    }
                    break;
                }
            }
        }
    }

    return 0;
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
}
void random_generate_inv(Eigen::VectorXd& array) //Generates A Random Array which obeys all the specifications
{
    srand(time(NULL));
    array.fill(0);
    int rand_gen=0;
    for(int i=1;i<=115;i++)
    {
        rand_gen = rand() % 8;
        rand_gen +=60;
        array(i)=rand_gen;
    }
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
void genetic_algo_inv(int length,Eigen::MatrixXd& inv_coeff_mat,Eigen::VectorXd& array)
{
    double cost_prev=0;
    double cost_after=0;
    int temp=0;
    int index[2];
    int rand_val_max = length/2;
    Eigen::VectorXd array_test(116);

    array_test = array;
    cost_prev = cost_analyze(inv_coeff_mat,array_test);
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
    cost_after = cost_analyze(inv_coeff_mat,array_test);

    if(cost_after<cost_prev)
    {    
        array=array_test;
        //cout<<"Cost == "<<cost_after<<endl;
    }
    return;
}