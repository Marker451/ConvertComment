#include<iostream>
#include"StdAfx.h"
using namespace std;

extern "C" int ConvertComment(FILE *inputfile, FILE *outputfile);

typedef enum
{
	NO_COMMENT_STATE = 1,
	C_COMMENT_STATE,
	CPP_COMMENT_STATE,
	STR_STATE,
	END_STATE
}STATE_ENUM;

typedef struct STATE_MACHINE
{
	FILE *inputfile;
	FILE *outputfile;
	UL ulstate;
};

STATE_MACHINE g_state = {0};

//////////////////////////////////////////////
void EventPro(char ch);
void EventProAtNo(char ch);
void EventProAtC(char ch);
void EventProAtCpp(char ch);
void EventProAtStr(char ch);
//////////////////////////////////////////////

int ConvertComment(FILE *inputfile, FILE *outputfile)
{
	if(inputfile==NULL || outputfile==NULL)
	{
		printf("argument is Error!\n");
		return -1;
	}

	g_state.inputfile = inputfile;
	g_state.outputfile = outputfile;
	g_state.ulstate = NO_COMMENT_STATE;

	char ch;
	char nextch;
	while(g_state.ulstate != END_STATE)
	{
		ch = fgetc(g_state.inputfile);
		EventPro(ch);
	}
	return 0;
}

void EventPro(char ch)
{
	switch(g_state.ulstate)
	{
	case NO_COMMENT_STATE:
		EventProAtNo(ch);
		break;
	case C_COMMENT_STATE:
		EventProAtC(ch);
		break;
	case CPP_COMMENT_STATE:
		EventProAtCpp(ch);
		break;
	case STR_STATE:
		EventProAtStr(ch);
		break;
	case END_STATE:
		break;
	default:
		break;
	}
}
void EventProAtNo(char ch)
{
	char nextch;
	switch(ch)
	{
	case '/':
		nextch = fgetc(g_state.inputfile);
		if(nextch == '/') //CPP  // ->/*
		{
			//
			fputc(ch,g_state.outputfile); // /
			fputc('*',g_state.outputfile);// *
			g_state.ulstate = CPP_COMMENT_STATE;
		}
		else if(nextch == '*') //C
		{
			fputc(ch,g_state.outputfile);
			fputc(nextch,g_state.outputfile);
			g_state.ulstate=C_COMMENT_STATE;
		}
		break;
	case EOF:
		g_state.ulstate=END_STATE;
		break;
	case '"':
		fputc(ch,g_state.outputfile);
		g_state.ulstate=STR_STATE;
		break;
	default:
		fputc(ch,g_state.outputfile);
	}

}
void EventProAtC(char ch)
{
	char nextch;
	switch(ch)
	{
	case '*':
		nextch=fgetc(g_state.inputfile);
		if(nextch== '/')
		{
			fputc(ch,g_state.outputfile);
			fputc(nextch,g_state.outputfile);
			g_state.ulstate=NO_COMMENT_STATE;
		}/*kkkk/**/
		else
		{
			fputc(ch,g_state.outputfile);  
			fseek(g_state.inputfile,-1,SEEK_CUR);
		}
		break;
	case '/':
		nextch=fgetc(g_state.inputfile);
		if (nextch=='/')
		{
			fputc(' ',g_state.outputfile);
			fputc(' ',g_state.outputfile);
		}
		break;
	default :
		fputc(ch,g_state.outputfile);
	}
}
void EventProAtCpp(char ch)
{
	char nextch;
	switch(ch)
	{
	case EOF:
		fputc('*',g_state.outputfile);
		fputc('/',g_state.outputfile);
		g_state.ulstate = END_STATE;
		break;
	case '\n':
		fputc('*',g_state.outputfile);
		fputc('/',g_state.outputfile);
		fputc(ch,g_state.outputfile);
		g_state.ulstate=NO_COMMENT_STATE;
		break;
	case '/':
		nextch=fgetc(g_state.inputfile);
		if (nextch=='/')
		{
			fputc(' ',g_state.outputfile);
			fputc(' ',g_state.outputfile);
		}
		else if (nextch=='*')
		{
			fputc(' ',g_state.outputfile);
			fputc(' ',g_state.outputfile);
		}
		else
		{
			fputc(ch,g_state.outputfile);
			fseek(g_state.inputfile,-1,SEEK_CUR);
		}
		break;
	case '*':
		nextch=fgetc(g_state.inputfile);
		if (nextch=='/')
		{
		    fputc(' ',g_state.outputfile);
			fputc(' ',g_state.outputfile);
		}
		else
		{
			fputc(ch,g_state.outputfile);
			fseek(g_state.inputfile,-1,SEEK_CUR);
		}
		break;

	default:
		fputc(ch,g_state.outputfile);
		break;
	}
}
void EventProAtStr(char ch)
{
	switch (ch)
	{
	case'"':
		fputc(ch,g_state.outputfile);
		g_state.ulstate=NO_COMMENT_STATE;
	default:
		fputc(ch,g_state.outputfile);
	}
}