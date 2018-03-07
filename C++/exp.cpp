#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 

    int lowest_differance = 115;
    double total_diff = 0;

void assign_coeff_mat(Eigen::MatrixXd& coeff_mat);
void random_generate(Eigen::VectorXd& array);
int cost_full(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array);
void assign_result_arrays(Eigen::VectorXd& array);
int is_okay(Eigen::VectorXd& array);
void swap_fixed_pos(Eigen::VectorXd& array);
void try_swap_fixed(Eigen::VectorXd& array,Eigen::MatrixXd& coeff_mat);
void print_result_new(Eigen::VectorXd& array);
void assign_results_new(Eigen::MatrixXd& coeff_mat);
void assign_results_old(Eigen::MatrixXd& coeff_mat);
void print_matrix(Eigen::MatrixXd& mat);
int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test);
void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results);
int cost_exp(Eigen::MatrixXd& coeff_mat,Eigen::VectorXd& array)
{
    Eigen::VectorXd array_temp(116);
    int temp_cost=0;
    array_temp = coeff_mat * array;
    int first = 0;
    int second= 0; 
    for(int i=1;i<=115;i++)
    {
        temp_cost=array_temp(i);
        //cout<<temp_cost<<endl;
        if((temp_cost>=57)&&(temp_cost<=64))
        {
            first++;
        }
        if((temp_cost>=65)&&(temp_cost<=72))
        {
            second++;
        }
    }
    cout<<"First Half = "<<first<<endl;
    cout<<"Second Half = "<<second<<endl;
    return array_temp.sum();
}

int main()
{
    int temp;
    int number = 0;
    int numbers[12];
	srand(time(NULL));
	//Load Co-efficient Matrix, Initialize Arrays and Variables
	Eigen::MatrixXd coeff_mat(200,116);
    assign_coeff_mat(coeff_mat);

    Eigen::MatrixXd results_new(300,116);     // New Results
    assign_results_new(results_new);

    // Already Found Results
    Eigen::MatrixXd results_old(200,116); 
    assign_results_old(results_old);
	
    Eigen::VectorXd array(116);
    Eigen::VectorXd array2(116);

    for (int i = 1; i < 5; i++)
    {
        array=results_old.row(i);
        temp=cost_exp(coeff_mat,array);
        cout<<"Cost = "<<temp<<endl;
    }


/*
    for(int i=0;i<=11;i++)
        numbers[i]=0;

	for(int i=1;i<=194;i++)
    {
        for(int j=1;j<=194;j++)
        {
            if(i==j)
                j++;
            array = results.row(i);
            array2 = results.row(j);
            temp = change_in_array(array,array2); 
            //cout<<temp<<" ";
            if(temp<lowest_differance)
                lowest_differance = temp;
            total_diff+=temp;
            if(temp<=10)   
                numbers[0]++;
            if(temp<=20)   
                numbers[1]++;
            if(temp<=30)   
                numbers[2]++;
            if(temp<=40)   
                numbers[3]++;
            if(temp<=50)   
                numbers[4]++;
            if(temp<=60)   
                numbers[5]++;
            if(temp<=70)   
                numbers[6]++;
            if(temp<=80)   
                numbers[7]++;
            if(temp<=90)   
                numbers[8]++;
            if(temp<=100)   
                numbers[9]++;
            if(temp<=115)   
                numbers[10]++;
        }
        //cout<<endl;
            
    }
    cout<<"Lowest Difference == "<<lowest_differance<<endl;
    cout<<"Average Difference == "<<(total_diff)/(194*194-194)<<endl;
    cout<<"Changes == "<<endl;
    for(int i=0;i<=11;i++)
        {
            cout<<numbers[i]<<" ";
            numbers[i]=0;
        }
    cout<<endl;

    lowest_differance = 115;
    total_diff = 0;

    for(int i=1;i<=194;i++)
    {
        array = results.row(i);
        for(int j=1;j<=4;j++)
        {
            array2 = results_old.row(j);
            temp = change_in_array(array,array2); 
            if(temp<lowest_differance)
                lowest_differance = temp;
            total_diff+=temp;
            if(temp<=10)   
                numbers[0]++;
            if(temp<=20)   
                numbers[1]++;
            if(temp<=30)   
                numbers[2]++;
            if(temp<=40)   
                numbers[3]++;
            if(temp<=50)   
                numbers[4]++;
            if(temp<=60)   
                numbers[5]++;
            if(temp<=70)   
                numbers[6]++;
            if(temp<=80)   
                numbers[7]++;
            if(temp<=90)   
                numbers[8]++;
            if(temp<=100)   
                numbers[9]++;
            if(temp<=115)   
                numbers[10]++;
        }
    }
    cout<<"Lowest Difference == "<<lowest_differance<<endl;
    cout<<"Average Difference == "<<(total_diff)/(194*4)<<endl;
    cout<<"Changes == "<<endl;
    for(int i=0;i<=11;i++)
        cout<<numbers[i]<<" ";
*/
	return 0;
}

void try_difference(Eigen::VectorXd& array,Eigen::MatrixXd& results) 
{
    FILE* fp_diff;
    fp_diff = fopen("diff_log.txt","a"); 
    Eigen::VectorXd result(116);
    int diff = 0;
    for(int i=1;i<=4;i++)
    {
       result = results.row(i); 
       diff = change_in_array(result,array); 
       total_diff+= diff;
       if(diff<lowest_differance)
            {
                lowest_differance=diff;
                for(int j=1;j<=115;j++)
                    fprintf(fp_diff, "%d",array(j));
                fprintf(fp_diff,"\n  Difference == %d  \n",lowest_differance);
            }
    }
    fclose(fp_diff);
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
        if(ch==EOF||rows==300)
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
void print_matrix(Eigen::MatrixXd& mat)
{
    for(int i=1;i<=115;i++)
    {
        for(int j=1;j<=115;j++)
        {
            cout<<mat(i,j);
        }
        cout<<endl;
    }
}

void swap_fixed_pos(Eigen::VectorXd& array)
{
    array(94)=1-array(94);
    array(115)=1-array(115);
}

void try_swap_fixed(Eigen::VectorXd& array,Eigen::MatrixXd& coeff_mat)
{
    int swap_cost_1,swap_cost_2;
    swap_cost_1 = cost_full(coeff_mat,array);
    swap_fixed_pos(array);
    swap_cost_2 = cost_full(coeff_mat,array);
    if(swap_cost_1<swap_cost_2)
        swap_fixed_pos(array);
    else 
        {
            cout<<"Swapped"<<endl;
            cout<<swap_cost_1<<endl;
            cout<<swap_cost_2<<endl;
        }
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
		{
            array_temp(i)=(57-temp_cost)*(57-temp_cost);
           //cout<<"Heer"<<endl;
        }
		if(temp_cost>72)
        {
			array_temp(i)=(temp_cost-72)*(temp_cost-72);
           // cout<<"Heer"<<endl;
        }
        
	}
	return array_temp.sum();
}

void assign_result_arrays(Eigen::VectorXd& array)
{
	array.fill(0);
	FILE* fp;
	fp = fopen("already_Found_results.txt","r");
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

void print_result_new(Eigen::VectorXd& array)
{
	FILE* fp_rslt;
	fp_rslt=fopen("results_found_by_me.txt","a");
	int temp;
	cout<<endl;
	for(int i=1;i<116;i++)
	{
		temp=array(i);
		cout<<temp;
		fprintf(fp_rslt,"%d",temp);
	}
	cout<<endl;
	fprintf(fp_rslt,"\n");
	fclose(fp_rslt);
}

int change_in_array(Eigen::VectorXd& array,Eigen::VectorXd& array_test)
{
    int changes=0;
    for(int i=2;i<116;i++)
    {
        if(array(i)!=array_test(i))
            changes++;
    }
    return changes;
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
}