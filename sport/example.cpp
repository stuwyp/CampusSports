#include "stdafx.h"
int fenge()//分隔例子
{
	char str[80] = "This is - www.w3cschool.cc - website";
	const char s[2] = "-";
	char *token;
	token = strtok(str, s);
	while( token != NULL ) 
	{
		printf( " %s\n", token );
		token = strtok(NULL, s);
	}
	return 0;
}
int xieru()//录入例子
{
	char names[21];
	PerInfo people;
	printf( "Enter ur name :");
	fgets(names,sizeof(names),stdin);
	printf( "Ur name is %d:",names[0]);
	strcpy(people.name,names);
	puts(people.name);
	//Person_create(people);
	return 0;
}