#include "stdafx.h";

int touristMenu()
{
	int choice;
	while (1)
	{

		while (1)
		{
			clearScreen(500);
			printf("\t\t\t *************************************************** \n");
			printf("\n\t\t\t\t\t汕头大学校运会管理系统\t\t    \n");
			printf("\n\t\t\t *************************************************** \n\n");

			printf("\n\t\t\t 1. 查看运动会          \t 2. 查看院系得分\n");
			printf("\n\t\t\t 3. 查看通知栏          \t 4. 返回登录界面\n");
			printf("\n\t\t\t 5. 退出系统 \n\n");

			if (scanf("%d", &choice) == 1 && (choice <= 5 && choice >= 1))
				break;
			else
				printf("\t\t         您的输入有误，请重新输入");
		}
		switch (choice)
		{

			case 1:
			case 2:
			case 3:
				break;
			case 4:
				return -1;
			default:
				return 0;
		}
	}
}