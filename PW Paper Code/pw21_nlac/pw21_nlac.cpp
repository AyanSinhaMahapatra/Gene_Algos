// This program computes the nonlinearity and absolute indicator values of the 21-variable PW type Boolean 
// functions obtained from the 4 solutions given in Table 1 (also given below). The truth tables of these 
// functions (each is of length 2097152 (=2^{21})) are read from the file "PWfunctions.txt".
// The code is compiled using Microsoft Visual C++ 2010 Express.
//
// Solution #1: "0111000110111110011111101010010010111001110100001001100010100100101110001000001000100110100101110111011001111100000"
// Solution #2: "0110010100111001010110000100001100111110011100111000000100010100000111111001110000111111001001011111100101001111000"
// Solution #3: "0111010010100001111110010011001010111111000111001101001011001100101100110010000111110111011010000000000000101111001"
// Solution #4: "0011000111100101111000100011011010100001101010101011011111111010001000001010111010100101000101100110010101110010100"


#include "stdafx.h"
#include "stdlib.h"

#define Zn 21
#define Zm 2097152 // (=2^{21})

FILE *outx=fopen("PWfunctions.txt", "r");  // This file contains the aforementioned truth tables of the 
										   // 21-variable PW type Boolean functions.

int _tmain(int argc, _TCHAR* argv[])
{
	void fastwh(int *FW, int *TTs); // Computes the Walsh-Hadamard spectrum "FW" of a 21 variable function (given its polar form "TTs").
	void fastwhld(long double *FW, long double *TTs); // Used by the function "acor" to obtain the autocorrelation spectrum.
	int acor(int *FW); // Computes the absolute indicator of a 21 variable function (given its Walsh-Hadamard spectrum "FW").
	int findmax(int *FW); //Finds the maximum absolute value in the Walsh-Hadamard spectrum.

	int *TTs,*TT,*FW,*ACA,*ADK;

	TT=(int *)malloc(Zm*sizeof(int));  // "TT"  will be used to represent the truth table of a 21 variable Boolean function.
	FW=(int *)malloc(Zm*sizeof(int));  // "FW"  will be used to represent the Walsh-Hadamard transform of "TT".
	TTs=(int *)malloc(Zm*sizeof(int)); // "TTs" will be used to represent the polar form of "TT".

	int x,i,j,k,NL,acx;
	i=0,j=0;
	while (!feof(outx))
	{
		fscanf(outx,"%d ",&x);

		TT[i]=x;
		i=i+1;
		if (i==Zm)
		{
			i=0;
			j=j+1;
			for (k=0;k<Zm;k++)
				TTs[k]=1-2*TT[k];
			fastwh(FW,TTs); // Finds the Walsh-Hadamard spectrum "FW" of the Boolean function "TT" (using its polar form "TTs").
			NL=Zm/2-findmax(FW)/2;
			acx=acor(FW); // Finds the absolute indicator "acx" using the Walsh-Hadamard spectrum "FW".
			// The PW functions are in the same order as the solutions given above.
			printf("\nFor the PW function #%d, nonlinearity=%d and absolute indicator=%d",j,NL,acx);
		}
	}
	fclose(outx);

	return 0;
}

// The function "fastwh" implements the fast Walsh-Hadamard transform.
void fastwh(int *FW, int *TTs)
{	
	int i,j,i1,i2,i3,k1=2,k2=Zm/2,k3=1,L1,temp1,temp2;
	for (i=0;i<Zm;i++)
		FW[i]=TTs[i];
	for (i1=0;i1<Zn;i1++)  
	{
	   L1=1;
	   for (i2=0;i2<k2;i2++)
	   {
		  for (i3=0;i3<k3;i3++)
		  {
			 i=i3+L1-1; j=i+k3; 
		     temp1= FW[i]; temp2 = FW[j]; 
			 FW[i]=temp1+temp2;
		     FW[j]=temp1-temp2;
		  }
	      L1=L1+k1; 
	   }
	   k1=k1*2; k2=k2/2; k3=k3*2;
	}
}

// The following function "fastwhld" is used by the subsequent function "acor".
// The only difference between the functions "fastwh" and "fastwhld" is the types 
// of the arrays "FW" and "TTs". This is required since we take the Walsh-Hadamard 
// transform of the squared Walsh-Hadamard spectrum to obtain the autocorrelation 
// spectrum, which results in dealing with values greater than 2^{32}. 
void fastwhld(long double *FW, long double *TTs)
{	
	int i,j,i1,i2,i3,k1=2,k2=Zm/2,k3=1,L1;
	long double temp1,temp2;
	for (i=0;i<Zm;i++)
		FW[i]=TTs[i];
	for (i1=0;i1<Zn;i1++)  
	{
	   L1=1;
	   for (i2=0;i2<k2;i2++)
	   {
		  for (i3=0;i3<k3;i3++)
		  {
			 i=i3+L1-1; j=i+k3; 
		     temp1= FW[i]; temp2 = FW[j]; 
			 FW[i]=temp1+temp2;
		     FW[j]=temp1-temp2;
		  }
	      L1=L1+k1; 
	   }
	   k1=k1*2; k2=k2/2; k3=k3*2;
	}
}

// The function "acor" returns the absolute indicator "Maxi" given the Walsh-Hadamard spectrum "FW".
// The autocorrelation spectrum is computed using the squared Walsh-Hadamard spectrum.
int acor(int *FW)
{
	int i;
	long double *AC,*QC,*ACt,*QCt;
	QC=(long double *)malloc(Zm*sizeof(long double)); AC=QC;
	QCt=(long double *)malloc(Zm*sizeof(long double)); ACt=QCt;
	for (i=0;i<Zm;i++)
		ACt[i]=((long double) FW[i])*((long double) FW[i]);
	fastwhld(AC,ACt);

	long double D,Maxi=-1;
	for (i=1;i<Zm;i++)
	{
		D=AC[i];
		if (AC[i]<0)
			D=-AC[i];
		if (D>Maxi)
			Maxi=D;
	}
	free(AC);
	free(ACt);
	Maxi=Maxi/Zm;
	return ((int) Maxi);
}

// The function "findmax" is used to find the maximum absolute value in the Walsh-Hadamard spectrum.
int findmax(int *FW)
{
	int i;
	int D,Maxi=-1;
	for (i=0;i<Zm;i++)
	{
		D=FW[i];
		if (FW[i]<0)
			D=-FW[i];
		if (D>Maxi)
			Maxi=D;
	}
	return Maxi;
}
