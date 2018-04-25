#include "stdafx.h";

//修改密码
//返回0，成功；返回1，失败
int changePwd(long id)
{
	PerInfo msg;
	char nowPwd[20];
	char inputPwd[20];
	char passwd[20];
	char repeatPwd[20];
	msg.id = id;

	strcpy(nowPwd, Person_getinfo(id).password);
	int wrong = 0;
	while (wrong < 3)
	{
		clearScreen(100);
		printf("\n\t请输入您的当前密码\n");
		inputPasswd(inputPwd);
		if (!strcmp(nowPwd, inputPwd))
			break;
		else
		{
			wrong++;
			printf("\n\t密码错误，请重新输入\n");
			Sleep(1000);
			continue;
		}
	}
	if (wrong == 3)
	{
		printf("\n    密码输入错误3次，正在返回上一级......");
		Sleep(1500);
		return -1;
	}
	while (1)
	{
		printf("\n\t请输入您的新密码\n");
		inputPasswd(passwd);
		printf("\n\t请再次输入您的新密码\n");
		inputPasswd(repeatPwd);
		if (!strcmp(passwd, repeatPwd))
		{
			if (!strcmp(passwd, nowPwd))
				printf("\n\t请不要设置相同的密码\n");
			else
			{
				puts("\n\t修改成功，返回主界面......");
				Sleep(500);
				break;
			}

		}
		else
			printf("\n\t您两次输入的密码不一致\n");

	}


	strcpy(msg.password, passwd);
	int i = Person_change(msg, 6);
	return i;
}

//修改个人信息
//返回0，成功；返回1，失败
int changeInfo(long id, int type)
{
	PerInfo msg;
	msg.id = id;
	if (type == 1)
	{
		char name[20];
		puts("\n\t请输入名字：");
		scanf("%s", name);
		strcpy(msg.name, name);
	}
	else if (type == 3)
	{
		int gender;
		puts("\n\t请输入性别(0：男；1：女）：");
		scanf("%d", &gender);
		msg.gender = gender;
	}
	else if (type == 4)
	{
		char org[20];
		puts("\n\t请输入组织：");
		scanf("%s", org);
		strcpy(msg.organization, org);
	}
	else if (type == 5)
	{
		char faculty[20];
		puts("\n\t请输入系：");
		scanf("%s", faculty);
		strcpy(msg.faculty, faculty);
	}
	if (!Person_change(msg, type))
	{
		puts("\n修改成功");
		return 0;
	}
	else
	{
		puts("\n修改失败");
		return 1;
	}
}


//该函数显示用户已报名的项目
//返回已报名的项目数    type：0：不打印；1：打印
int userSignupedEve(long id, int type)
{
	if (sys_info.eve == 0)
	{
		printf("\n\t没有比赛项目\n");
		return 0;
	}
	int count = 0;
	SportsEvent* evo = (SportsEvent *)malloc((sys_info.eve)*sizeof(SportsEvent));
	Event_getlist(evo);

	for (int o = 0; o < sys_info.eve; o++)
	{
		for (int p = 0; p < evo[o].maxpeople; p++)
		{
			if (evo[o].prePerson[p].id == id)
			{
				count++;
				if (type)
				{
					printf("\n\n[项目%d] ID:%ld  ", count, evo[o].id);
					if (evo[o].type / 10 == 1)
						printf("男子 ");
					else
						printf("女子 ");
					printf("%s ", evo[o].name);
					if (evo[o].type % 2)
						printf("径赛 ");
					else
						printf("田赛 ");
					printf("%s (时长:%d分钟)  预赛人数:%d人 ", evo[o].place, evo[o].timecost, evo[o].maxpeople);
					//printf("%d", evo[o].time);
					//evo[o].time = 1521421395;
					if (evo[o].time == 0)
						printf(" 比赛时间未定");
					else
					{
						time_t  tt = (time_t)evo[o].time;   //假定时间
						char now[64];
						struct tm *ttime;
						ttime = localtime(&tt);
						strftime(now, 64, "%Y%m%d %H:%M:%S", ttime);
						printf(" %d年%d月%d日  %02d:%02d ", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);
					}

					if (evo[o].type % 2) //径赛
					{

						char sc[20] = "";
						//printf(" [分数:%d] ", evo[o].prePerson[p].score);
						scoreToChar(evo[o].prePerson[p].score, sc);
						printf(" 成绩:%s ", sc);
					}
					else
					{
						if (evo[o].prePerson[p].score > 1000)
							printf(" 成绩:%dm ", evo[o].prePerson[p].score / 1000);

						printf(" 成绩:%dcm ", evo[o].prePerson[p].score % 1000);
					}
					qsort(evo[o].prePerson, 100, sizeof(Participant), cmp);
					int rank = 0;
					if (evo[o].type % 2)   //径赛
					{

						for (int i = 0; i < 100; i++)
						{
							if (evo[o].prePerson[i].score == 0)
								continue;
							else
							{
								rank++;
								if (evo[o].prePerson[i].id == id)
									break;
							}
						}
						if (rank == 0)
							printf(" 暂无排名");
						else
							printf(" 第%d名\n", rank);
					}
					else
					{

						for (int i = 100; i > 0; i--)
						{
							if (evo[o].prePerson[i].score == 0)
								continue;
							else
							{

								rank++;
								if (evo[o].prePerson[i].id == id)
									break;
							}
						}
						if (rank == 0)
							printf(" 暂无排名");
						else
							printf(" 第%d名\n", rank);
					}


					if (evo[o].hasFinals)
					{

						printf("\n\t有决赛 (时长:%d分钟) 决赛人数:%d人", evo[o].timecost, evo[o].finNum);

						for (int p = 0; p < evo[o].finNum; p++)
						{
							if (evo[o].finPerson[p].id == id)
							{
								if (type)
								{
									//evo[o].etime = 1521421395;
									if (evo[o].etime == 0)
										printf(" 比赛时间未定");
									else
									{
										time_t  tt = (time_t)evo[o].etime;   //假定时间
										char now[64];
										struct tm *ttime;
										ttime = localtime(&tt);
										strftime(now, 64, "%Y%m%d %H:%M:%S", ttime);
										printf(" %d年%d月%d日  %02d:%02d ", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);

									}
									if (evo[o].type % 2) //径赛
									{
										char sc[20] = "";
										scoreToChar(evo[o].finPerson[p].score, sc);
										printf(" 成绩:%s ", sc);
									}
									else
									{
										if (evo[o].finPerson[p].score > 1000)
											printf(" 成绩:%dm ", evo[o].finPerson[p].score / 1000);

										printf(" 成绩:%dcm ", evo[o].finPerson[p].score % 1000);
									}
									qsort(evo[o].finPerson, 20, sizeof(Participant), cmp);
									int finRank = 0;
									if (evo[o].type % 2)   //径赛
									{

										for (int i = 0; i < 20; i++)
										{
											if (evo[o].finPerson[i].score == 0)
												continue;
											else
											{
												finRank++;
												if (evo[o].finPerson[i].id == id)
													break;
											}
										}
										if (finRank == 0)
											printf("暂无排名 ");
										else
											printf("第%d名\n", finRank);
									}
									else
									{
										for (int i = 20, rank = 0; i > 0; i--)
										{
											if (evo[o].finPerson[i].score == 0)
												continue;
											else
											{
												rank++;
												if (evo[o].finPerson[i].id == id)
													break;
											}
										}
										if (finRank == 0)
											printf("暂无排名 ");
										else
											printf("第%d名\n", finRank);
									}
								}
							}
						}

					}
					else
						printf(" 无决赛\n");
				}
				break;
			}
			else
				continue;

		}
	}

	free(evo);

	fflush(stdin);
	if (type)
	{
		if (count == 0)
			printf("\n  暂无已报名的运动项目\n");
		else
		{
			int ch;
			printf("\n  是否取消报名项目\n\n  1. 是 2. 否(输入除数字1以外的键)  ");
			scanf("%d", &ch);
			if (ch == 1)
			{
				int eveid;
				printf("\n 请输入要取消报名的项目ID  ");
				if (scanf("%ld", &eveid))
				{
					if (Event_dec(eveid, id, 0) == 0)
					{
						printf("\n  取消报名成功\n");
						Sleep(1000);
						return 1;
					}
					else
					{
						printf("\n  取消报名失败，可能你的输入错误");
						Sleep(1000);
					}
				}
				else
				{
					printf("\n  输入错误");
					Sleep(1000);
				}


			}

		}
		fflush(stdin);
		printf("\n  按回车键返回上一级");
		getchar();
	}

	return count;
}

//该函数用于报名项目（预赛）
//返回0，成功；返回1，失败
int signupSports(long  peoid)
{
	if (sys_info.eve == 0)
	{
		printf("\n\t没有比赛项目，报名失败\n");
		Sleep(1000);
		return 1;
	}
	if (sys_info.canjoin == 0)
	{
		printf("\n\t目前不是报名时间，报名失败\n");
		Sleep(1000);
		return 1;
	}
	int count = 0;
	SportsEvent* evo = (SportsEvent *)malloc((sys_info.eve)*sizeof(SportsEvent));
	Event_getlist(evo);
	for (int o = 0; o < sys_info.eve; o++)
	{
		if ((evo[o].type / 10 -1) == Person_getinfo(peoid).gender)
		{


			printf("\n[项目%d]   ", o);
			printf("ID:%ld %s", evo[o].id, evo[o].name);
			if (evo[o].type / 10 == 1)
				printf("男子 ");
			else
				printf("女子 ");
			if (evo[o].type % 2)
				printf("径赛 ");
			else
				printf("田赛 ");
			printf("%s (时长:%d分钟) 预赛人数:%d人 -", evo[o].place, evo[o].timecost, evo[o].maxpeople);
			//evo[o].time = 1521421395;
			if (evo[o].time == 0)
				printf(" 比赛时间未定");
			else
			{
				time_t  tt = (time_t)evo[o].time;   //假定时间
				char now[64];
				struct tm *ttime;
				ttime = localtime(&tt);
				strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
				printf(" %d年%d月%d日  %02d:%02d", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);
			}

			if (evo[o].hasFinals)
				printf(" 有决赛\n");
			else
				printf(" 无决赛\n");
		}
	}
	free(evo);

	long eveid;
	count = userSignupedEve(peoid, 0);

	if (count >= 3)
	{
		printf("\n  您报名的项目已满（最多三个）\n");
		Sleep(1000);
		return 1;
	}

	printf("\n  请输入您想报名的项目的ID : ");
	scanf("%ld", &eveid);
	if (Event_had(eveid))
	{
		if (Event_getinfo(eveid).type / 20 != Person_getinfo(peoid).gender)
		{
			printf("\n  您报名的项目有误（性别错误）\n");
			Sleep(1000);
			printf("\n  正在跳转，返回上一级.......");
			return 1;
		}
		/*printf("报名返回值为:%d", Event_inc(eveid, peoid, 0));*/
		if (Event_inc(eveid, peoid, 0) == 3)
		{
			printf("\n  不要重复报名该项目\n");
			Sleep(1000);
			return 1;
		}

		printf("\n 报名参赛成功\n");
		fflush(stdin);
		printf("\n  按回车键返回上一级");
		getchar();
		return 0;
	}
	else
		printf("\n  不存在此项目\n");

	fflush(stdin);
	printf("\n  按回车键返回上一级");
	getchar();
	return 1;

}


//该函数显示用户准备注册过程
//返回0：成功；1：失败
int toSignupMenu()
{
	int choice;
	int success = 0;
	PerInfo perInfo;

	printf("\n 请稍候，页面正在跳转中 ......");
	while (1)
	{

		clearScreen(500);
		printf("\n\t注册界面 \n");
		while (1)
		{
			printf("\n\t请输入学号（ID）：");
			if (scanf("%ld", &perInfo.id) != 1)
			{
				fflush(stdin);
				printf("\n    你输入的ID有误，请重新输入\n");
				continue;
			}
			else
			{
				fflush(stdin);
				break;
			}
		}
		while (1)
		{
			printf("\n\t请输入姓名（Name）：");
			if (scanf("%s", perInfo.name) != 1 && Char_isok(perInfo.name))
			{
				fflush(stdin);
				printf("\n    你输入的姓名有误，请重新输入\n");
				continue;
			}
			else
			{
				fflush(stdin);
				break;
			}
		}
		while (1)
		{
			printf("\n\t请输入性别（Gender：0 -- 男；1 -- 女）：");
			if (scanf("%d", &perInfo.gender) != 1 || (perInfo.gender != 0  && perInfo.gender != 1))
			{
				fflush(stdin);
				printf("\n    你输入的性别数字有误，请重新输入\n");
				continue;
			}
			else
			{
				fflush(stdin);
				break;
			}
		}

		printf("\n\t目前所有书院\n");
		for (int i = 0; i < sys_info.numyuan; i++)
		{
			if (i % 5 == 0)
				printf("\n");
			printf("%s    ", sys_info.yuan[i]);
		}
		puts("");
		do
		{
			success = 0;
			printf("\n\t请输入所在书院（Organization）：");

			if (scanf("%s", perInfo.organization) == 1)
			{
				for (int i = 0; i < sys_info.numyuan; i++)
				{
					if (strcmp(sys_info.yuan[i], perInfo.organization)==0)
					{
						success = 1;
						break;
					}
					else
					{
						success = 0;
						continue;
					}
				}
				if(!success)
					printf("\n\t输入书院名字不存在，请重新输入\n");
			}
			else
			{
				fflush(stdin);
				printf("\n    你输入的书院有误，请重新输入\n");
				continue;
			}
			
		} while (!success);

		printf("\n\t目前所有的系\n");
		for (int i = 0; i < sys_info.numxi; i++)
		{
			if (i % 5 == 0)
				printf("\n");
			printf("%s    ", sys_info.xi[i]);
		}
		puts("");
		do
		{
			success = 0;
			printf("\n\t请输入所在系（Faculty）：");

			if (scanf("%s", perInfo.faculty) == 1)
			{
				for (int i = 0; i < sys_info.numxi; i++)
				{
					if (strcmp(sys_info.xi[i], perInfo.faculty) == 0)
					{
						success = 1;
						break;
					}
					else
					{
						success = 0;
						continue;
					}
				}
				if (!success)
				printf("\n\t输入系的名字不存在，请重新输入\n");
			}
			else
			{
				fflush(stdin);
				printf("\n    你输入的系有误，请重新输入\n");
				continue;
			}

		} while (!success);


		char passwd[20];
		char repeatPwd[20];
		while (1)
		{
			printf("\n\t请输入您的新密码\n");
			inputPasswd(passwd);
			printf("\n\t请再次输入您的新密码\n");
			inputPasswd(repeatPwd);
			if (!strcmp(passwd, repeatPwd))
			{
				strcpy(perInfo.password, passwd);
				break;
			}
			else
			{
				printf("\n\t您两次输入的密码不一致，请重新输入\n");
				fflush(stdin);
			}
		}
		if (!signup(perInfo)) break;
		else
		{

			while (1)
			{
				printf("\n\t1. 重新输入\n");
				printf("\n\t2. 退出注册\n");
				if (scanf("%d", &choice) == 1 && (choice <= 2 && choice >= 1))
					break;
				else
					printf("\n\t\t 您的输入有误，请重新输入");
			}
			if (choice == 1)
				continue;
			else
			{
				printf("\n\t注册失败");
				Sleep(500);
				return 1;
			}

		}

	}
	return 0;
}
//该函数显示用户登录后的菜单
int userMenu(long id)
{
	int choice;
	while (1)
	{

		while (1)
		{
			clearScreen(500);
			Notice * not = (Notice *)malloc((sys_info.not)*sizeof(Notice));
			int i = 0;
			if (!Person_getnotice(id, not))
			{
				if (!(not[i].time > 0)) printf("[暂无个人通知]\n");
				else printf("[个人通知]\n\n");
				while (not[i].time > 0)
				{

					printf("[通知%d] ", i + 1);
					time_t  tt = (time_t)not[i].time;   //假定时间
					char now[64];
					struct tm *ttime;
					ttime = localtime(&tt);
					strftime(now, 64, "%Y-%m-%d %H:%M:%S", ttime);
					printf(" 通知时间：%d年%d月%d日  %02d:%02d  ", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);

					printf(" 通知信息：%s\n", not[i].msg);
					puts("");
					i++;
				}
			}


			free(not);
			printf("\n\t\t\t *************************************************** \n");
			printf("\n\t\t\t\t\t汕头大学校运会管理系统\t\t    \n");
			printf("\n\t\t\t *************************************************** \n\n");
			printf("\n\t\t\t 1. 修改密码		\t 2. 修改信息 \n");
			printf("\n\t\t\t 3. 管理已报名项目	\t 4. 报名运动会 \n");
			printf("\n\t\t\t 5. 退出登录		\t 6. 退出系统  \n\n");


			if (scanf("%d", &choice) == 1 && (choice <= 6 && choice >= 1))
				break;
			else
				printf("\t\t\t 您的输入有误，请重新输入");
		}
		switch (choice)
		{
			case 1:
				changePwd(id);
				break;
			case 2:
				while (1)
				{
					clearScreen(100);
					PerInfo per = Person_getinfo(id);
					printf("\n\t\t\t  姓名：%s", per.name);
					if (per.gender) printf(" 性别：女");
					else printf(" 性别：男");
					printf(" 书院：%s   系：%s\n", per.organization, per.faculty);
					printf("\n\t\t\t *************************************************** \n");
					printf("\n\t\t\t\t\t汕头大学校运会管理系统\t\t    \n");
					printf("\n\t\t\t *************************************************** \n\n");
					printf("\n\t\t\t 1. 修改姓名		\t 2. 修改性别 \n");
					printf("\n\t\t\t 3. 修改书院		\t 4. 修改系 \n");
					printf("\n\t\t\t 5. 返回上一级\n\n");


					if (scanf("%d", &choice) == 1 && (choice <= 5 && choice >= 1))
						break;
					else
						printf("\t\t\t 您的输入有误，请重新输入");
				}
				switch (choice)
				{
					case 1:
						changeInfo(id, 1);
						break;
					case 2:
						changeInfo(id, 3);
						break;
					case 3:
						changeInfo(id, 4);
						break;
					case 4:
						changeInfo(id, 5);
					default:
						continue;
				}
				fflush(stdin);
				puts("\n\n按回车键返回上一步");
				getchar();
				break;
			case 3:
				userSignupedEve(id, 1);
				break;
			case 4:
				signupSports(id);
				break;
			case 5:
				return -1;
			default:
				return 0;
		}
	}
	return 0;
}



