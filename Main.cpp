#include<iostream>
using namespace std;

extern "C" int ConvertComment(FILE *inputfile, FILE *outputfile);

/**********************************
 function: 
 describe: 
 input   : 
 output  : 
 retutn  : 
 when    what   why    who
 
 *********************************/
int main()
{
	FILE *fpIn = NULL;
	FILE *fpOut = NULL;
	fpIn = fopen("input.c","r");
	//if(fpIn == NULL)
	if(NULL == fpIn)
	{
		printf("Open input.c file fail!\n");
		return -1;
	}

	fpOut = fopen("output.c","w");
	if(NULL == fpOut)
	{
		printf("Open output.c file fail!\n");
		return -1;
	}

	//
	ConvertComment(fpIn,fpOut);//

	fclose(fpIn);
	fclose(fpOut);
	return 0;
}