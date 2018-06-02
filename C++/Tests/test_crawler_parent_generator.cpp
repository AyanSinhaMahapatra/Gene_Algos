#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

/*
int random_number(int start,int end,int length,int order)
{
	//srand(time(NULL));
	int rand_no =0;
	int rand_range =0;

	if(start>end)
		rand_range = (115-start) + (end-2);
	else
		rand_range = end-start;
	if(order==2)
		rand_range = rand_range - length ;

	rand_no = rand() % rand_range;
	rand_no = rand_no + start;	

	if(rand_no==94)
		rand_no++;
	if(rand_no>114)
		rand_no = 1 + rand_no % 114;

	return rand_no;
}
void random_parents(int *parents_index,int p_length)
{
	//Parent 1
	int p1_start = random_number(2,114,p_length,1);
	if(p1_start==94)
		p1_start++;
	int p1_end = p1_start + p_length - 1;
	if(p1_end>=94&&p1_start<94)
		p1_end++;
	if(p1_end>114)
		p1_end = p1_end % 114+1;
	//Parent 2
	int p2_start = random_number(p1_end+1,p1_start-1,p_length,2);
	if(p2_start==94)
		p2_start++;
	int p2_end = p2_start + p_length - 1;
	if(p2_end>=94&&p2_start<94)
		p2_end++;
	if(p2_end>114)
		p2_end = p2_end % 114 + 1;

	parents_index[0] = p1_start;
	parents_index[1] = p1_end;
	parents_index[2] = p2_start;
	parents_index[3] = p2_end;
}

int main()
{
	srand(time(NULL));
	int i=0,count115=0,count94=0;
	for(i=0;i<=10000;i++)
	{
		int flag=0,crawler,count;
		int p1_parents_index[4];
		int p1_length=10;
		random_parents(p1_parents_index,p1_length);

		crawler = p1_parents_index[0];
		count=1;
		while(count<p1_length)
		{
			if(crawler==115)
				crawler=2;
			if(crawler==94)
				crawler++;
			count++;
			crawler++;
		}
		if(crawler==115)
			{
				crawler=2;
				count115++;
			}
			if(crawler==94)	
			{
				crawler++;
				count94++;
			}
		if(crawler!=p1_parents_index[1])
		{
			flag=1;
			cout<<crawler<<"-"<<p1_parents_index[1]<<endl;
		}

		crawler = p1_parents_index[2];
		count=1;
		while(count<p1_length)
		{
			if(crawler==115)
			{
				crawler=2;
				count115++;
			}
			if(crawler==94)	
			{
				crawler++;
				count94++;
			}
			count++;
			crawler++;
		}
		if(crawler==115)
			{
				crawler=2;
				count115++;
			}
			if(crawler==94)	
			{
				crawler++;
				count94++;
			}
		if(crawler!=p1_parents_index[3])
		{
			flag=1;
			cout<<crawler<<"-"<<p1_parents_index[3]<<endl;
		}

		if(flag)
		{
			cout<<"First Parent"<<endl;
			cout<<p1_parents_index[0]<<" - "<<p1_parents_index[1]<<endl;
			cout<<"Second Parent"<<endl;
			cout<<p1_parents_index[2]<<" - "<<p1_parents_index[3]<<endl<<endl; 
			flag=0;
		}
	}
	cout<<i<<endl<<"94 - "<<count94<<endl<<"115 - "<<count115<<endl;
	return 0;
}

*/

int random_number(int start,int end,int length,int order)
{
    //srand(time(NULL));
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
}

int main()
{
	srand(time(NULL));
	int parents_index[4];
	int i=0,count115=0,count94=0;
	for(i=0;i<=10000000;i++)
	{
		random_parents(parents_index,10);

		for(int j=0;j<4;j++)
			if((parents_index[j]>115)||(parents_index[j]<1))
				cout<<"Outside Scope"<<endl;

		parents_index[0] = (parents_index[0] + 9)%115;
		parents_index[2] = (parents_index[2] + 9)%115;

		if(parents_index[0]==0)
			parents_index[0]=115;
		else if(parents_index[2]==0)
			parents_index[2]=115;

		if((parents_index[2]!=parents_index[3])||(parents_index[0]!=parents_index[1]))
			cout<<"Not Macthing "<<parents_index[0]<<" "<<parents_index[1]<<" "<<parents_index[2]<<" "<<parents_index[3]<<" "<<endl;

	}
	return 0;
}