#include "stdafx.h";

void clearScreen(int sleepS)
{
	fflush(stdin);
	Sleep(sleepS);
	system("cls");
}

int inputPasswd(char * password)
{
	char ch;
	int length = 0;
	while ((ch = _getch()) != '\r')
	{
		if (ch != 8)//不是回撤就录入
		{
			password[length] = ch;
			putchar('*');//并且输出*号
			length++;
		}
		else
		{
			if(!length)continue;
			putchar('\b');//这里是删除一个，我们通过输出回撤符 /b，回撤一格，
			putchar(' ');//再显示空格符把刚才的*给盖住，
			putchar('\b');//然后再 回撤一格等待录入。
			length--;
		}
	}
	password[length] = '\0';
	return length;
}

int login(long id, char* psw, int type)//0错误 1管理 2用户
{
	if(id<10000)return 0;
	if(id==10000){
		if(strcmp(sys_info.password ,psw)==0){
			return 1;
		}else{
			return 0;
		}
	}else{
		PerInfo peo;
		peo=Person_getinfo(id);
		if(peo.id==0)return 0;
		if(strcmp(peo.password ,psw)==0){
			return 2;
		}else{
			return 0;
		}
	}
	return 0; //表示登录成功
	//return 0; //表示账户或密码错误，登录失败
}

int signup(PerInfo  msg)
{
	int retValue = Person_create(msg);
	if (retValue == 0)
	{
		printf("\n\t注册成功");
		Sleep(500);
		return 0;
	}
	else
	{
		puts("");
		if (retValue == 1)
		{
			printf("\n\t名字：非法字符串\n");
		}
		else if (retValue == 2)
		{
			printf("\n\tID：非法ID\n");
		}
		else if (retValue == 3)
		{
			printf("\n\t性别：非法数字\n");
		}
		else if (retValue == 4)
		{
			printf("\n\t院：非法字符串\n");
		}
		else if (retValue == 5)
		{
			printf("\n\t系：非法字符串\n");
		}
		else if (retValue == 6)
		{
			printf("\n\t密码：非法字符串\n");
		}
		else if (retValue == 7)
		{
			printf("\n\t错误：该账号已存在\n");
		}
		else
		{
			printf("\n\t返回值错误\n");
		}
		printf("\n\t注册失败\n");
		Sleep(500);
		return -1;
	}
}

