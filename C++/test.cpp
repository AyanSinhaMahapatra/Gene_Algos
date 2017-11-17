#include<iostream>
#include<Eigen/Dense>
using namespace std;
using namespace Eigen;
int main()
{
	Eigen::VectorXd array(116);
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
    cout<<array;
    return 0;
}