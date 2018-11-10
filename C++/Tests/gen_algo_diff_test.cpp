#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

    double prob_swap = 0.2;
    int is_debugging_on = 0;
    int trench_no = 1;

void random_generate(Eigen::VectorXd& array);
void genetic_algo(Eigen::VectorXd& array,Eigen::VectorXd& array_2,
    Eigen::VectorXd& array_off,Eigen::VectorXd& array_off_2);
int check_similar_arrays(Eigen::VectorXd& array,Eigen::VectorXd& array_2);
int is_okay(Eigen::VectorXd& array);
void shake(Eigen::VectorXd& array_main,Eigen::VectorXd& array,int k_neighbourhood);
int check_array(int rand_gen,int *array_random,int size);
void swap_fixed_pos(Eigen::VectorXd& array);


int main()
{
    int count = 0;
    srand(time(NULL));

    Eigen::VectorXd array_1(116);
    Eigen::VectorXd array_off_1(116);
    Eigen::VectorXd array_2(116);
    Eigen::VectorXd array_off_2(116);

    while(1)
    {
        random_generate(array_1);

        for(int j=1;j<=115;j++)
            cout<<array_1(j);
        cout<<endl;

        shake(array_1,array_2,2);
        
        for(int j=1;j<=115;j++)
            cout<<array_2(j);
        cout<<endl;

        break;
    }

    return 0;
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

    if(check_similar_arrays(array,array_off))
        cout<<" PROBLEM ARRAY NOT CHANGING 1 "<<endl;

    if(check_similar_arrays(array_2,array_off_2))
        cout<<" PROBLEM ARRAY NOT CHANGING 2 "<<endl;

    if((is_okay(array_off)==0)||(is_okay(array_off_2)==0))
        cout<< " ERROR ARRAY NOT OKAY GEN_ALGO" <<endl;

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

void swap_fixed_pos(Eigen::VectorXd& array)
{
    array(94)=1-array(94);
    array(115)=1-array(115);
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
        if(int(array(i))==1)
            total_zeros++;
    }
    if(total_ones!=56)
    {
        flag=0;
        if(is_debugging_on == 1)
            cout<<"Problem 3"<<endl;
    }
    if(total_zeros!=56)
    {
        flag=0;
        if(is_debugging_on == 1)
            cout<<"Problem 4"<<endl;
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
}