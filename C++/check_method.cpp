// Load any of the Already found results and then Deviate from them and check how things are converging 
// Try to improve based on that, hold only one approach and improve that only

#include<iostream>
#include <stdio.h>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;
int main()
{
	srand(time(NULL));
	int min = 100;
	int max = 0;
	int place_1;

	for(int i=1;i<=100;i++)
	{
		place_1 = rand()%113 + 2;
		if(place_1<min)
			min = place_1;
		if(place_1>max)
			max = place_1;
	}

	cout<<max<<" "<<min<<endl;
}