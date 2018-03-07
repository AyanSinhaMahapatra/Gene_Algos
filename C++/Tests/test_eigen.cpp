#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
#include<Eigen/Dense> //Linear Algebra Library
using namespace std;
using namespace Eigen; 
void change_array(Eigen::VectorXd& array,int length)
{
	for(int i=0;i<length;i++)
    {
    	array(i)=1;
    }
    cout<<"Chnaged"<<endl;
}
int main()
{
    Eigen::VectorXd array(6);
    for(int i=0;i<6;i++)
    {
    	array(i)=0;
    }
    cout<<array<<endl;
    change_array(array,6);
    cout<<array<<endl;
}