#include "stdafx.h";
//显示项目属性
int showEventProperty(long id)	
{
	SportsEvent eve;
	eve = Event_getinfo(id);

	clearScreen(400);
	printf("\t项目ID:%d    项目名:%s\t", eve.id, eve.name);
	if (eve.type / 20)
		printf("女子项目 ");
	else
		printf("男子项目 ");
	if (eve.type % 2)
		printf("径赛 ");
	else
		printf("田赛 ");
	printf("%s", eve.place);
	printf("\t(时长:%d分钟)  预赛人数:%d人", eve.timecost, eve.maxpeople);
	return 0;
	//eve.time = 1521421395;
	if (eve.time == 0)
		printf(" \n\t比赛时间未定");
	else
	{
		time_t  tt = (time_t)eve.time;   //假定时间
		char now[64];
		struct tm *ttime;
		ttime = localtime(&tt);
		strftime(now, 64, "%Y%m%d %H:%M:%S", ttime);
		printf(" \n\t预赛时间:%d年%d月%d日  %02d:%02d ", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);
	}
	if (eve.hasFinals)
	{
		printf("\n\t有决赛 (时长:%d分钟)  决赛人数:%d人", eve.timecost, eve.finNum);
		//eve.etime = 1521421395;
		if (eve.etime == 0)
			printf(" 比赛时间未定\n");
		else
		{
			time_t  tt = (time_t)eve.etime;   //假定时间
			char now[64];
			struct tm *ttime;
			ttime = localtime(&tt);
			strftime(now, 64, "%Y%m%d %H:%M:%S", ttime);
			printf("\t决赛时间:%d年%d月%d日  %02d:%02d \n", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);

		}
	}
	else
		printf("无决赛\n");
	return 0;
}
int registerScore(int type)//0初赛 1决赛
{
	long eveid;
	int success = 0;
	SportsEvent* evo = (SportsEvent *)malloc((sys_info.eve)*sizeof(SportsEvent));
	Event_getlist(evo);
	if (type == 0)
	{
		for (int o = 0; o < sys_info.eve; o++)
		{
			printf("\n[项目%d]   ", o);
			printf("ID:%ld %s", evo[o].id,evo[o].name);
			if (evo[o].type / 10 == 1)
				printf("男子 ");
			else
				printf("女子 ");
			if (evo[o].type % 2)
				printf("径赛 ");
			else
				printf("田赛 ");
		}
	}
	else
	{
		for (int o = 0; o < sys_info.eve; o++)
		{
			if (Event_getinfo(evo[o].id).hasFinals)
			{
				printf("\n[项目%d]   ", o);
				printf("ID:%ld %s", evo[o].id,evo[o].name);
				if (evo[o].type / 10 == 1)
					printf("男子 ");
				else
					printf("女子 ");
				if (evo[o].type % 2)
					printf("径赛 ");
				else
					printf("田赛 ");
			}
			
		}
	}
	
	free(evo);
	while (!success)
	{
		success = 0;
		puts("\n  请输入项目ID：");
		if (scanf("%ld", &eveid))
		{
			if (Event_had(eveid))
			{
				if (type == 0)
				{
					clearScreen(500);
					if (Event_getinfo(eveid).maxpeople == 0)
					{
						printf("\n  该项目目前没有参赛者,正在返回上一级...\n");
						Sleep(500);
						break;
					}
					int perid;
					long score;
					char timeString[20];
					showEventProperty(eveid);
					if (Event_getinfo(eveid).type % 2 == 0)
					{
						printf(" (注意单位为厘米 cm ) \n");
						for (int i = 0; i < Event_getinfo(eveid).maxpeople; i++)
						{
							perid = Event_getinfo(eveid).prePerson[i].id;
							while (1)
							{
								printf("\n  %d. 姓名 %s  学号 %ld  成绩 : ", i+1, Person_getinfo(perid).name, perid);
								if (scanf("%ld", &score))
								{
									Event_changescore(eveid,Event_getinfo(eveid).prePerson[i].id,type,score);
								//printf("\n%ld\n", Event_getinfo(eveid).prePerson[i].score);
									fflush(stdin);
									break;
								}
								else
								{
									printf("\n  输入有误，请重新输入该运动员成绩\n");
									fflush(stdin);
									continue;
								}
							}
						}
						success = 1;
					}
					else
					{
						printf("(注意成绩输入格式为 1'12''22 ) \n");
						for (int i = 0; i < Event_getinfo(eveid).maxpeople; i++)
						{
							
								perid = Event_getinfo(eveid).prePerson[i].id;
								while (1)
								{
									printf("\n %d. 姓名 %s  学号 %ld  成绩 : ", i+1, Person_getinfo(perid).name, perid);
									if (scanf("%s", timeString))
									{
								//printf("\n%s\n", timeString);
										score = charToScore(timeString);
								//printf("\n%ld\n", score);
										if (score == -1)
										{
											printf("\n  输入有误，请重新输入该运动员成绩\n");
											fflush(stdin);
											continue;
										}
										Event_changescore(eveid, Event_getinfo(eveid).prePerson[i].id, type, score);
								printf("\n%ld\n", Event_getinfo(eveid).prePerson[i].score);
										fflush(stdin);
										break;
									}
									else
									{
										printf("\n  输入有误，请重新输入该运动员成绩\n");
										fflush(stdin);
										continue;
									}
								}
						}
						success = 1;
					}


				}
				else
				{
					
					if (Event_getinfo(eveid).hasFinals)
					{
						clearScreen(500);
						if (Event_getinfo(eveid).finNum == 0)
						{
							printf("\n  该项目目前没有参赛者,正在返回上一级...\n");
							Sleep(500);
							break;
						}
						int perid;
						long score;
						char timeString[20];
						if (Event_getinfo(eveid).type % 2 == 0)
						{
							printf("(注意单位为厘米 cm ) \n");
							for (int i = 0; i < Event_getinfo(eveid).finNum; i++)
							{
								perid = Event_getinfo(eveid).finPerson[i].id;
								while (1)
								{
									printf("\n %d.   姓名 %s   学号 %ld    成绩 : ", i+1, Person_getinfo(perid).name, perid);
									if (scanf("%ld", &score))
									{
										Event_changescore(eveid, Event_getinfo(eveid).finPerson[i].id, type, score);
									//printf("\n%ld\n", Event_getinfo(eveid).finPerson[i].score);
										fflush(stdin);
										break;
									}
									else
									{
										printf("\n  输入有误，请重新输入该运动员成绩\n");
										fflush(stdin);
										continue;
									}
								}
							}
							success = 1;
						}
						else
						{
							printf("(注意成绩输入格式为 1'12''22 ) \n");
							for (int i = 0; i < Event_getinfo(eveid).finNum; i++)
							{

								perid = Event_getinfo(eveid).finPerson[i].id;
								while (1)
								{
									printf("\n %d.   姓名 %s  学号 %ld    成绩 : ", i+1, Person_getinfo(perid).name, perid);
									if (scanf("%s", timeString))
									{
									//printf("\n%s\n", timeString);
										score = charToScore(timeString);
									//printf("\n%ld\n", score);
										if (score == -1)
										{
											printf("\n  输入有误，请重新输入该运动员成绩\n");
											fflush(stdin);
											continue;
										}
										Event_changescore(eveid, Event_getinfo(eveid).finPerson[i].id, type, score);
									//printf("\n%ld\n", Event_getinfo(eveid).finPerson[i].score);
										fflush(stdin);
										break;
									}
									else
									{
										printf("\n  输入有误，请重新输入该运动员成绩\n");
										fflush(stdin);
										continue;
									}
								}
							}
						}
						success = 1;
					}

					else
					{
						printf("\n  抱歉，该项目没有决赛\n\n  请重新输入");
						continue;
					}
				}
			}
			else
			{
				printf("\n  该ID不存在\n\n  请重新输入\n");
				continue;
			}
		}
		else
		{
			printf("\n  你的输入有误\n\n  正在返回上一级...\n");
			Sleep(1000);
			break;
		}
	}
	if (success)
		printf("\n\n  登记成绩成功\n");
	
	return 0;
}

long timeTransferToTime_t(char strtime[50])
{
	int ppp;
	char* p;
	const char delim[2] = "-";//分隔符字符串
	struct tm p_stm;
	p = strtok(strtime, delim);//第一次调用strtok
	char pp[5][20];
	ppp = 0;
	char str[20];
	int year;
	int mon;
	int day;
	int hour;
	int min;
	if (p != NULL)strcpy(pp[ppp++], p);
	while (p != NULL)
	{//当返回值不为NULL时，继续循环
	 //printf("%s\n", p);//输出分解的字符串
		p = strtok(NULL, delim);//继续调用strtok，分解剩下的字符串
		if (p != NULL) strcpy(pp[ppp++], p);
		if (ppp >= 5)break;
	}
	if (ppp == 5)
	{
		strcpy(str, pp[0]);
		year = atoi(str);
		strcpy(str, pp[1]);
		mon = atoi(str);
		strcpy(str, pp[2]);
		day = atoi(str);
		strcpy(str, pp[3]);
		hour = atoi(str);
		strcpy(str, pp[4]);
		min = atoi(str);

		p_stm.tm_year = year - 1900;
		p_stm.tm_mon = mon - 1;
		p_stm.tm_mday = day;
		p_stm.tm_hour = hour;
		p_stm.tm_min = min;
		p_stm.tm_sec = 0;
		p_stm.tm_isdst = -1;

		long time = (long)mktime(&p_stm);

		return time;
	}
	else
	{
		printf("输入错误");
	}
	return 0;
}

//修改项目属性
int changeEventProperty(long eve_id)
{
	SportsEvent eve;
	int choice = 0;
	char name[20];
	int type;//10男田、11男径、20女田、21女径
	char place[20];//场地
	int timecost;//用时，分钟
	int maxpeople;//上限人数
	int hasFinals;//有无决赛 0无 1有
	int finNum;//有决赛时决赛人数
	long time;//比赛时间戳，秒
	long etime;//决赛时间
	eve.id = eve_id;
	do
	{
		while (1)
		{
			clearScreen(400);
			showEventProperty(eve_id);
			printf("\n\t\t\t\t\t修改项目属性界面\t\t    \n");
			printf("\n\t\t\t 1. 修改名字              \t 2. 修改项目类型    \n");
			printf("\n\t\t\t 3. 修改场地              \t 4. 修改用时  \n");
			printf("\n\t\t\t 5. 有无决赛              \t 6.  修改预赛时间  \n");
			printf("\n\t\t\t 7. 返回上一级  \n");
			scanf("%d", &choice);
			if (choice <= 7 && choice >= 1)
				break;
			else
				printf("\t 您的输入有误，请重新输入\n");
		}
		int state;
		switch (choice)
		{
		case 1:								//
			printf("\t请修改此项目的名称\n");
			scanf("%s", &name);
			strcpy(eve.name, name);
			state = Event_change(eve, 1);
			if (state == 1)
				printf("非法输入");
			else if (state == 11)
				printf("不存在此项目");
			else printf("修改成功");
			break;
		case 2:
			printf("\t请输入此项目类别（男子田赛输入10，径赛输入11，女子田赛输入20，径赛输入21）\n");
			scanf("%d", &type);
			eve.type = type;
			state = Event_change(eve, 3);
			printf("state值为%d", state);
			if (state == 3)
				printf("非法输入");
			else if (state == 11)
				printf("不存在此项目");
			else
				printf("修改成功");
			break;
		case 3:
			printf("\t请修改此项目的场地\n");
			scanf("%s", &place);
			strcpy(eve.place, place);
			state = Event_change(eve, 4);
			printf("state值为%d", state);
			if (state == 4)
				printf("非法输入");
			else if (state == 11)
				printf("不存在此项目");
			else
				printf("修改成功");
			break;
		case 4:
			printf("\t请修改此项目的用时\n");
			scanf("%d", &timecost);
			eve.timecost = timecost;
			state = Event_change(eve, 5);
			if (state == 5)
				printf("非法输入");
			else if (state == 11)
				printf("不存在此项目");
			else
				printf("修改成功");
			break;
		case 6:
			char strtime[50];
			printf("请修改预赛的时间（2010-01-01-01-01）");
			scanf("%s", &strtime);
			state = timeTransferToTime_t(strtime);
			if (state != 0)
			{
				time = state;
				eve.time = time;//2018-03-20-1-45
				state = Event_change(eve, 9);
				if (state == 9)
					printf("非法输入");
				else if (state == 11)
					printf("不存在此项目");
				else
				{
					printf("修改成功");
				}
			}
			else
			{
				printf("非法输入");
			}
			break;
		case 5:
			printf("\t1.有决赛 2.无决赛\n");
			scanf("%d", &hasFinals);
			eve.hasFinals = hasFinals;
			state = Event_change(eve, 7);
			if (state == 7)
				printf("非法输入");
			else if (state == 11)
				printf("不存在此项目");
			else if (hasFinals == 0)
				printf("修改成功");
			else
			{
				int finchoice = 3;
				do
				{
					while (1)
					{
						clearScreen(400);
						//printf("\n\t 1.  修改决赛人数 \n");
						printf("\n\t 1.  修改决赛时间 \n");
						printf("\n\t 2.  返回上一级\n");
						scanf("%d", &finchoice);
						if (finchoice >= 1 && finchoice <= 2)
							break;
						else
							printf("输入错误，请重新输入");
					}
					/*if (finchoice == 1)
					{
					printf("\t请修改此项目的决赛人数（小于20）");
					scanf("%d", &finNum);
					eve.finNum = finNum;
					int state = Event_change(eve, 8);
					if (state == 8)
					printf("非法输入");
					else
					printf("修改成功");
					}
					else */if (finchoice == 1)
					{
						char strtime[50];
						printf("请修改决赛的时间（2010-01-01-01-01）");
						scanf("%s", &strtime);
						state = timeTransferToTime_t(strtime);
						if (state != 0)
						{
							etime = state;
							eve.etime = etime;//2018-03-20-1-45
							state = Event_change(eve, 10);
							if (state == 9)
								printf("非法输入");
							else if (state == 11)
								printf("不存在此项目");
							else
							{
								printf("修改成功");
							}
						}
						else
						{
							printf("非法输入");
						}
						break;
					}
				} while (finchoice != 2);
			}
			break;
		}
	} while (choice != 7);
	return 0;
}



//显示项目部分信息
int showAllEventID()		//显示目前所有项目ID
{
	int i;
	SportsEvent *evo = (SportsEvent*)malloc(sys_info.eve * sizeof(SportsEvent));
	Event_getlist(evo);
	printf("\n\t\t\t目前所有项目\t\t    \n");
	for (i = 0; i < sys_info.eve; i++)
	{
		printf("\n\t\t\t%ld  %s  ", evo[i].id, evo[i].name);
		if (evo[i].type / 10 == 1)
			printf("男子 ");
		else
			printf("女子 ");
		printf("预赛人数:%d\n", evo[i].maxpeople);
	}
	free(evo);
	return 0;

}

//增加项目
void creatEvent() 
{		
	clearScreen(400);
	char name[20];
	int type;//10男田、11男径、20女田、21女径
	char place[20];//场地
	int timecost;//用时，分钟
	int hasFinals = 0;//有无决赛 0无 1有
	printf("\t请输入新项目的名称\n");
	scanf("%s", &name);
	printf("\t请输入此项目类别（男子田赛输入10，径赛输入11，女子田赛输入20，径赛输入21）\n");
	scanf("%d", &type);
	printf("\t请输入此项目的场地\n");
	scanf("%s", &place);
	printf("\t请输入此项目的用时\n");
	scanf("%d", &timecost);
	printf("\t有无决赛（0.无 1.有）\n");
	scanf("%d", &hasFinals);
	SportsEvent msg;
	strcpy(msg.name, name);
	msg.type = type;
	strcpy(msg.place, place);
	msg.timecost = timecost;
	msg.hasFinals = hasFinals;
	int state = Event_create(msg);
	if (state == 1)
		printf("\n\t名字输入错误\n");
	else if (state == 3)
		printf("\n\t田径类别输入错误\n");
	else if (state == 4)
		printf("\n\t场地输入错误\n");
	else if (state == 5)
		printf("\n\t用时输入错误\n");
	else if (state == 7)
		printf("\n\t有无决赛输入错误\n");
	else if (state == 10)
		printf("\n\t太多项目\n");
	else printf("\n\t创建成功\n");
} //

//删除项目
void delectEvent()
{
	long id = -1;
	int state;
	printf("\n\t请输入需要删除的项目的ID\n");
	scanf("%ld", &id);
	state = Event_delete(id);
	if (state == 1)
		printf("\n\t此项目不能删除\n");
	else if (state == 2)
		printf("\n此项目不存在\t\n");
	else if (state == 3)
		printf("\n\t删除失败\n");
	Sleep(1000);
}

//锁定报名
void lockjoin()		
{
	sys_changecj(0);
	printf("\n\t报名已锁定\n");
	Sleep(1000);
}

//开放报名
void unlockjoin()	//开放报名
{
	sys_changecj(1);
	printf("\n\t报名已开放\n");
	Sleep(1000);
}

//修改预赛成绩
void changeonegrrade(long eveid)	
{
	SportsEvent	spEve;
	long peoid;
	int rank;
	fflush(stdin);
	printf("\n\t请输入运动员ID：");
	scanf("%ld", &peoid);
	if (Person_getinfo(peoid).id == 0)
		puts("\n  抱歉，系统暂无该用户\n");
	else
	{
		clearScreen(500);
		spEve = Event_getinfo(eveid);
		int isExisted = 1;
		for (int p = 0; p < spEve.maxpeople; p++)
		{
			if (spEve.prePerson[p].id == peoid)
			{
				isExisted = 1;
				break;
			}
			else
				isExisted = 0;
		}
		if (!isExisted)
		{
			printf("\n该项目不存在该id");
			//break;
		}
		printf("\n[项目]   项目ID:%d    项目名:%s", spEve.id, spEve.name);
		if (spEve.type % 2)
			printf("径赛 ");
		else
			printf("田赛 ");

		long score;
		printf("\n\n  请输入该名运动员的预赛成绩(注意该项目是");
		if (spEve.type % 2)
			printf("径赛)\n");
		else
			printf("田赛)\n");
		scanf("%ld", &score);
		Event_changescore(eveid, peoid, 0, score);
		puts("\n  登记成绩成功");
	}
}

//增加预赛运动员
void addOnePlayer(long eveid)
{
	int ret;
	long peoid;
	fflush(stdin);
	printf("\n\t请输入运动员ID：");
	scanf("%ld", &peoid);
	if (Person_getinfo(peoid).id == 0)
		puts("\n  抱歉，暂无该用户\n");
	else
	{
		ret = Event_inc(eveid, peoid, 0);
		if (ret == 0)
			puts("\n  该运动员报名预赛已经成功\n");
		else if (ret == 3)
			puts("\n  该运动员之前已经成功报名预赛啦\n");
		else
			puts("\n  该运动员报名预赛失败\n");
	}
	Sleep(500);
}

//删除预赛运动员
void delectOnePlayer(long eveid)
{
	long peoid;int i=0,p=0;
	PerInfo peo;
	SportsEvent	spEve;
	spEve=Event_getinfo(eveid);
	printf("%s预赛：\n",spEve.name);
	for(i=0;i<100;i++){
		if(spEve.prePerson[i].id){
			printf("%d",spEve.prePerson[i].id);
			peo=Person_getinfo(spEve.prePerson[i].id);
			printf("\t%s(%s)\n",peo.name,peo.faculty);
			p++;
		}
		if(p>=spEve.maxpeople)break;
	}

	fflush(stdin);
	printf("\n\t请输入运动员ID：");
	scanf("%ld", &peoid);
	if (Person_getinfo(peoid).id == 0)
		puts("\n  抱歉，暂无该用户\n");
	else
	{
		spEve = Event_getinfo(eveid);
		for (int p = 0; p < 100; p++)
		{
			if (spEve.prePerson[p].id == peoid)

				spEve.prePerson[p].score = 0;
		}
		if (Event_dec(eveid, peoid, 0) == 0)
			puts("\n  删除运动员成功\n");
		else
			puts("\n  删除运动员失败\n");
	}
	Sleep(500);
}

//增加决赛运动员
void addTwoPlayer(long eveid)
{
	long peoid;int i=0,p=0;
	PerInfo peo;
	SportsEvent	spEve;
	spEve=Event_getinfo(eveid);
	printf("%s决赛：\n",spEve.name);
	for(i=0;i<20;i++){
		if(spEve.finPerson[i].id){
			printf("%d",spEve.finPerson[i].id);
			peo=Person_getinfo(spEve.finPerson[i].id);
			printf("\t%s(%s)\n",peo.name,peo.faculty);
			p++;
		}
		if(p>=spEve.finNum)break;
	}

	fflush(stdin);
	printf("\n\t请输入运动员ID：");
	scanf("%ld", &peoid);
	if (Person_getinfo(peoid).id == 0)
		puts("\n  抱歉，暂无该用户\n");
	else if (Event_inc(eveid, peoid, 1) == 0)
		puts("\n  该运动员已经成功进入决赛\n");
	else
		puts("\n  操作失败\n");
	Sleep(500);
}

//查询预赛情况
void getoneInfo(long eveid)
{
	SportsEvent	spEve;
	int rank;
	fflush(stdin);
	rank = 0;
	spEve = Event_getinfo(eveid);
	qsort(spEve.prePerson, 100, sizeof(Participant), cmp);

	if (spEve.type % 2) //径赛
	{

		for (int p = 0; p < 100; p++)
		{
			if (spEve.prePerson[p].score == 0)
				continue;
			rank++;

			printf("\n第%d名 : ", rank);
			printf("%s   ", Person_getinfo(spEve.prePerson[p].id).name);

			char s[20] = "";
			scoreToChar(spEve.prePerson[p].score, s);
			printf("  成绩:%s ", s);

		}

	}
	else     //田赛
	{
		for (int p = spEve.maxpeople; p > 0; p--)
		{
			if (spEve.prePerson[p].score == 0)
				continue;
			rank++;
			printf("\n第%d名 :", rank);
			printf("%s", Person_getinfo(spEve.prePerson[p].id).name);
			if (spEve.prePerson[p].score > 1000)
				printf("  成绩:%dm", spEve.prePerson[p].score / 1000);

			printf("  成绩:%dcm", spEve.prePerson[p].score % 1000);

		}

	}
	if (rank == 0)
		printf("\n  暂无预赛成绩\n");

	fflush(stdin);
	puts("\n\n按回车键返回上一步");
	getchar();
}

//修改决赛成绩
void changetwograde(long eveid)		
{
	SportsEvent	spEve;
	long peoid;
	int rank;
	fflush(stdin);
	printf("\n\t请输入运动员ID：");
	scanf("%ld", &peoid);
	if (Person_getinfo(peoid).id == 0)
		puts("\n  抱歉，系统暂无该用户\n");
	else
	{
		clearScreen(500);
		spEve = Event_getinfo(eveid);
		int isExisted = 1;
		for (int p = 0; p < spEve.finNum; p++)
		{
			if (spEve.finPerson[p].id == peoid)
			{
				isExisted = 1;
				break;
			}
			else
				isExisted = 0;
		}
		if (!isExisted)
		{
			printf("\n该项目不存在该id");
			//break;
		}

		printf("[项目]   项目ID:%d    项目名:%s", spEve.id, spEve.name);
		if (spEve.type % 2)
			printf("径赛 ");
		else
			printf("田赛 ");
		long score;
		printf("\n  请输入该名运动员的决赛成绩(注意该项目是");
		if (spEve.type % 2)
			printf("径赛)\n");
		else
			printf("田赛)\n");
		scanf("%ld", &score);
		if (Event_changescore(eveid, peoid, 1, score) == 0)
			puts("\n  登记成绩成功");
		else
			puts("\n  登记成绩失败");
	}

}

//查询决赛情况
void getTwoInfo(long eveid)		
{
	SportsEvent	spEve;
	int rank;
	fflush(stdin);
	spEve = Event_getinfo(eveid);
	qsort(spEve.finPerson, 20, sizeof(Participant), cmp);

	rank = 0;
	if (spEve.type % 2) //径赛
	{

		for (int p = 0; p < 20; p++)
		{
			spEve.prePerson[p].score = 0;
			if (spEve.finPerson[p].score == 0)
				continue;
			rank++;

			printf("\n第%d名 : ", rank);
			printf("%s", Person_getinfo(spEve.finPerson[p].id).name);

			char s[20] = "";
			scoreToChar(spEve.finPerson[p].score, s);
			printf("  成绩:%s ", s);

		}
	}
	else     //田赛
	{
		for (int p = spEve.finNum; p > 0; p--)
		{
			if (spEve.finPerson[p].score == 0)
				continue;
			rank++;
			printf("\n第%d名 : ", rank);
			printf("%s", Person_getinfo(spEve.finPerson[p].id).name);
			if (spEve.finPerson[p].score > 1000)
				printf("  成绩:%dm", spEve.finPerson[p].score / 1000);

			printf("  成绩:%dcm", spEve.finPerson[p].score % 1000);

		}

	}
	//printf("%d", rank);
	if (rank == 0)
		printf("\n  暂无决赛成绩\n");
	fflush(stdin);
	puts("\n\n按回车键返回上一步");
	getchar();
}

//筛选决赛运动员
void selcetTwoPlayer(long eveid) 
{
	SportsEvent	spEve;
	int rank;
	long peoid;
	int ret;
	fflush(stdin);
	spEve = Event_getinfo(eveid);
	qsort(spEve.finPerson, 20, sizeof(Participant), cmp);
	rank = 0;
	if (spEve.type % 2) //径赛
	{
		int finNum;
		printf("\n当前预赛运动员总数:%d", spEve.maxpeople);
		while (1)
		{
			printf("\n  请输入能进决赛的人数(请合理安排)\n");
			scanf("%d", &finNum);
			if (finNum < 2 || finNum >= spEve.maxpeople)
				printf("\n  错误，请重输\n");
			else
				break;
		}
		for (int p = 0; p < finNum; p++)
		{
			if (spEve.prePerson[p].score == 0)
				continue;
			rank++;

			printf("\n第%d名 : ", rank);
			printf("%s", Person_getinfo(spEve.prePerson[p].id).name);
			peoid = spEve.prePerson[p].id;
			char s[20] = "";
			scoreToChar(spEve.prePerson[p].score, s);
			printf("  成绩:%s ", s);
			ret = Event_inc(eveid, peoid, 1);
			//printf("返回值%d",ret );
			if (ret == 0)
				printf("  该名运动员成功进入决赛");
			else if (ret == 3)
				printf("  已经进入决赛，不要重复操作");
			else
				printf("  输入有误，操作失败");

		}

	}
	else     //田赛
	{
		int finNum;
		printf("\n当前预赛运动员总数:%d", spEve.maxpeople);
		while (1)
		{
			printf("\n  请输入能进决赛的人数(请合理安排)\n");
			scanf("%d", &finNum);
			if (finNum >= spEve.maxpeople)
				printf("\n  错误，请重输\n");
			else
				break;
		}

		for (int p = finNum; p > 0; p--)
		{
			if (spEve.prePerson[p].score == 0)
				continue;
			rank++;
			printf("\n第%d名 :", rank);
			printf("%s", Person_getinfo(spEve.finPerson[p].id).name);
			if (spEve.finPerson[p].score > 1000)
				printf("  成绩:%dm", spEve.prePerson[p].score / 1000);

			printf("  成绩:%dcm", spEve.prePerson[p].score % 1000);
			peoid = spEve.prePerson[p].id;
			if (Event_inc(eveid, peoid, 1) == 0)
				printf("该名运动员成功进入决赛");
			else
				printf("操作失败");
		}

	}
	if (rank == 0)
		printf("\n  暂无预赛成绩");
	fflush(stdin);
	puts("\n\n按回车键返回上一步");
	getchar();
}

//删除决赛运动员
void delectTwoPlayer(long eveid)
{
	SportsEvent	spEve;
	long peoid;
	fflush(stdin);
	printf("\n\t请输入运动员ID：");
	scanf("%ld", &peoid);
	if (Person_getinfo(peoid).id == 0)
		puts("\n  抱歉，暂无该用户\n");

	else
	{
		spEve = Event_getinfo(eveid);

		for (int p = 0; p < 100; p++)
		{
			if (spEve.finPerson[p].id == peoid)
				spEve.finPerson[p].score = 0;
		}
		if (Event_dec(eveid, peoid, 1) == 0)
			puts("\n  删除运动员成功\n");
		else
			puts("\n  删除运动员失败\n");
	}
	Sleep(100);
}

//显示比赛成绩
void showEventScore(long eveid) 
{
	clearScreen(400);
	int i;
	SportsEvent eve = Event_getinfo(eveid);
	printf("目前预赛人数 ：%d\n", eve.maxpeople);
	for (i = 0; i < eve.maxpeople; i++)
	{
		printf("%d\t  ", eve.prePerson[i].id);
		if (eve.type % 2) //径赛
		{

			char sc[20] = "";
			//printf(" [分数:%d] ", evo[o].prePerson[p].score);
			scoreToChar(eve.prePerson[i].score, sc);
			printf(" 成绩:%s  \n", sc);
		}
		else
		{
			if (eve.prePerson[i].score > 1000)
				printf(" 成绩:%d m\n ", eve.prePerson[i].score / 1000);

			printf(" 成绩:%d cm\n", eve.prePerson[i].score % 1000);
		}
	}

	if (eve.hasFinals)
	{

		printf("目前决赛人数 ：%d\n", eve.finNum);

		for (i = 0; i < eve.finNum; i++)
		{
			printf("%d\t  ", eve.finPerson[i].id);
			if (eve.type % 2) //径赛
			{

				char sc[20] = "";
				//printf(" [分数:%d] ", evo[o].prePerson[p].score);
				scoreToChar(eve.finPerson[i].score, sc);
				printf(" 成绩:%s \n", sc);
			}
			else
			{
				if (eve.finPerson[i].score > 1000)
					printf(" 成绩:%d m \n", eve.finPerson[i].score / 1000);

				printf(" 成绩:%d cm \n", eve.finPerson[i].score % 1000);
			}
		}
	}
}

//修改项目中的运动员管理
int EveSporterManage(long eveid)
{
	int choice;SportsEvent eve;
	int ret;
	int i;
	eve=Event_getinfo(eveid);
	while (1)
	{

		while (1)
		{
			printf("\n\t管理运动员界面 %d\n",eve.hasFinals);
			printf("\n\t\t 1. 修改项目属性      \t 2. 增加预赛运动员 \n");
			printf("\n\t\t 3. 删除预赛运动员    \t 4. 修改预赛成绩 \n");
			if(eve.hasFinals){
				printf("\n\t\t 5. 筛选出决赛运动员  \t 6. 增加决赛运动员   \n");
				printf("\n\t\t 7. 删除决赛运动员   \t 8. 修改决赛成绩决赛   \n");
				printf("\n\t\t 9. 返回上一级");
			}else{
				printf("\n\t\t 8. 修改决赛成绩决赛  \t 9. 返回上一级   \n");
			}
			
			if (scanf("%d", &choice) == 1 && (choice <= 10 && choice >= 1))
				break;
			else
				printf("\t\t\t 您的输入有误，请重新输入");
		}
		SportsEvent	spEve;
		long peoid;
		int rank;
		switch (choice)
		{
		case 1:
			changeEventProperty(eveid);
			break;
		case 2:
			addOnePlayer(eveid);
			break;
		case 3:
			delectOnePlayer(eveid);
			break;
		case 4:
			changeonegrrade(eveid);
			break;
		case 5:
			if(eve.hasFinals) selcetTwoPlayer(eveid);
			break;
		case 6:
			if(eve.hasFinals) addTwoPlayer(eveid);
			break;
		case 7:
			if(eve.hasFinals) delectTwoPlayer(eveid);
			break;
		case 8:
			changetwograde(eveid);
			break;
		default:
			return 0;
		}
	}
}

//修改项目
int changeEvent(long eveid)
{
	int choice;SportsEvent eve;
	int ret;
	int i;
	eve=Event_getinfo(eveid);
	while (1)
	{

		while (1)
		{
			clearScreen(500);
			printf("\n\t管理运动员界面\n");
			printf("\n\t\t 1. 修改项目属性      \t 2. 增加预赛运动员 \n");
			printf("\n\t\t 3. 删除预赛运动员    \t 4. 修改预赛成绩 \n");
			if(eve.hasFinals){
				printf("\n\t\t 5. 增加决赛运动员   \t 6. 删除决赛运动员   \n");
				printf("\n\t\t 7. 修改决赛成绩决赛   \t 8. 返回上一级   \n");
			}else{
				printf("\n\t\t 8. 返回上一级\n");
			}
			if (scanf("%d", &choice) == 1 && (choice <= 8 && choice >= 1))
				break;
			else
				printf("\t\t\t 您的输入有误，请重新输入");
		}
		SportsEvent	spEve;
		long peoid;
		int rank;
		switch (choice)
		{
		case 1:
			changeEventProperty(eveid);
			break;
		case 2:
			addOnePlayer(eveid);
			break;
		case 3:
			delectOnePlayer(eveid);
			break;
		case 4:
			fflush(stdin);
			printf("\n\t请输入运动员ID：");
			scanf("%ld", &peoid);
			if (Person_getinfo(peoid).id == 0)
				puts("\n  抱歉，系统暂无该用户\n");
			else
			{
				clearScreen(500);
				spEve = Event_getinfo(eveid);
				int isExisted = 1;
				for (int p = 0; p < spEve.maxpeople; p++)
				{
					if (spEve.prePerson[p].id == peoid)
					{
						isExisted = 1;
						break;
					}
					else
						isExisted = 0;
				}
				if (!isExisted)
				{
					printf("\n该项目不存在该id");
					//break;
				}
				printf("\n[项目]   项目ID:%d    项目名:%s", spEve.id, spEve.name);
				if (spEve.type % 2)
					printf("径赛 ");
				else
					printf("田赛 ");

				long score;
				printf("\n\n  请输入该名运动员的预赛成绩(注意该项目是");
				if (spEve.type % 2)
					printf("径赛)\n");
				else
					printf("田赛)\n");
				scanf("%ld", &score);
				Event_changescore(eveid, peoid, 0, score);
				puts("\n  登记成绩成功");
			}
			break;
		/*case 5:
			selcetTwoPlayer(eveid);
			break;*/
		case 5:
			if(eve.hasFinals) addTwoPlayer(eveid);
			break;
		case 6:
			if(eve.hasFinals) delectTwoPlayer(eveid);
			break;
		case 7:
			if(eve.hasFinals){
			fflush(stdin);
			printf("\n\t请输入运动员ID：");
			scanf("%ld", &peoid);
			if (Person_getinfo(peoid).id == 0)
				puts("\n  抱歉，系统暂无该用户\n");
			else
			{
				clearScreen(500);
				spEve = Event_getinfo(eveid);
				int isExisted = 1;
				for (int p = 0; p < spEve.finNum; p++)
				{
					if (spEve.finPerson[p].id == peoid)
					{
						isExisted = 1;
						break;
					}
					else
						isExisted = 0;
				}
				if (!isExisted)
				{
					printf("\n该项目不存在该id");
					break;
				}

				printf("[项目]   项目ID:%d    项目名:%s", spEve.id, spEve.name);
				if (spEve.type % 2)
					printf("径赛 ");
				else
					printf("田赛 ");
				long score;
				printf("\n  请输入该名运动员的决赛成绩(注意该项目是");
				if (spEve.type % 2)
					printf("径赛)\n");
				else
					printf("田赛)\n");
				scanf("%ld", &score);
				if (Event_changescore(eveid, peoid, 1, score) == 0)
					puts("\n  登记成绩成功");
				else
					puts("\n  登记成绩失败");
			}
			}
			break;
		default:
			return 0;
		}
	}
}

//旧修改项目
int changeEventInfo()
{
	clearScreen(400);
	showAllEventID();
	long id;
	printf("\n\n\t\t请输入要修改项目的ID:\n");
	scanf("%ld", &id);
	int state = Event_had(id);
	if (state == 1)
	{
		int type;
		do
		{
			SportsEvent eve;
			eve = Event_getinfo(id);

			clearScreen(400);

			while (1)
			{
				printf("\n\t 1.  运动员管理 \n");
				printf("\n\t 2.  修改项目属性 \n");
				printf("\n\t 3.  返回上一级 \n");
				if (scanf("%d", &type) == 1 && (type <= 3 && type >= 1))
					break;
				else
					printf("\n\t 您的输入有误，请重新输入\n");
			}
			if (type == 1)
				EveSporterManage(id);
			else if (type == 2)
				changeEventProperty(id);
		} while (type != 3);


	}
	else
	{
		printf("不存在此项目");
	}
	return 0;
}

int ifusechangdi(SportsEvent *evo, char *changdi, long time)
{//是否占用场地  组，场地，开始时间
	for (int i = 0; i < sys_info.eve; i++)
	{
		if (strcmp(evo[i].place, changdi))
		{
			if (time >= evo[i].time && time <= evo[i].time + evo[i].timecost)return 1;
			if (time >= evo[i].etime && time <= evo[i].etime + evo[i].timecost)return 1;
		}
	}
	return 0;
}
void timetochar(long time)
{
	time_t  tt = (time_t)time;   //假定时间
	char now[64];
	struct tm *ttime;
	ttime = localtime(&tt);
	strftime(now, 64, "%Y%m%d %H:%M:%S", ttime);
	printf("%d年%d月%d日  %02d:%02d ", ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min);
}
int creatTableMenu()
{
	clearScreen(1000);
	int i, dmorn, mnum, dafte, anum, setnum = 0, numplace = 0, congtu, p, c, x; char strtime[50] = ""; long state = 0; changdi cd; cd.num = 0;
	SportsEvent *evo = (SportsEvent*)malloc(sys_info.eve * sizeof(SportsEvent));
	Event_getlist(evo);
	printf("\t目前所有项目\t\t    \n");
	int ii = sys_info.eve;//预赛加决赛数
	for (i = 0; i < sys_info.eve; i++)
	{
		printf("\n\t%ld,%s, 报名人数:%d\n", evo[i].id, evo[i].name, evo[i].maxpeople);
		evo[i].time = evo[i].etime = 0;
		if (evo[i].hasFinals)ii++;
		strcpy(cd.name[cd.num], evo[i].place);
		cd.time[cd.num++] = 0;
	}

	while (!state)
	{
		printf("\n\n请输入比赛日期（如2010-01-01）：");
		scanf("%s", &strtime);
		strcat(strtime, "-0-0");
		state = timeTransferToTime_t(strtime);
		if (!state)printf("，请重新输入");
	}

	while (setnum < ii)
	{//设置好的少于所有
		//printf("\n%d",ii);
		for (i = 0; i < cd.num; i++)cd.time[i] = state + 8 * 60 * 60;//初始化场地时间
		//8:00-11:00 14:00-17:00
		//mnum=anum=0;//早上个数，下午个数
		for (i = 0; i < sys_info.eve; i += 2)
		{//早上
			if (!evo[i].time)
			{//判断预赛头
				for (p = 0; p<cd.num; p++)
				{//轮循场地
					//printf("\n%s\t%s",evo[i].place,cd.name[p]);
					if (!strcmp(evo[i].place, cd.name[p]))
					{
						if (cd.time[p]>(state + 11 * 60 * 60))break;
						//冲突判断
						congtu = 0;
						for (c = 0; c < sys_info.eve; c++)
						{//预赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].time)continue;//还没安排
							for (x = 0; x < evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (evo[c].maxpeople<4)continue;//比赛被取消
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 0))
								{//项目有同一个人
									if (cd.time[p]>evo[c].time && cd.time[p] < (evo[c].time + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
						}
						if (!congtu)//还不冲突就检查决赛
						for (c = 0; c < sys_info.eve; c++)
						{//决赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].hasFinals)continue;//没有决赛
							if (!evo[c].etime)continue;//还没安排
							for (x = 0; x<evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 1))
								{//项目有同一个人
									if (cd.time[p]>evo[c].etime && cd.time[p] < (evo[c].etime + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
						}
						if (congtu)continue;
						//
						evo[i].time = cd.time[p];
						cd.time[p] += evo[i].timecost * 60;
						printf("\n[第一轮]%s\t%s\t用时：%d分钟\t", evo[i].name, evo[i].place, evo[i].timecost);
						timetochar(evo[i].time);
						printf("\n");
						setnum++;
						break;
					}
				}
			}
			//判断预赛尾
		}
		for (i = 0; i < sys_info.eve; i++)
		{//早上
			if (!evo[i].etime && evo[i].hasFinals)
			{//判断决赛头
				for (p = 0; p<cd.num; p++)
				{
					//printf("\n%s\t%s",evo[i].place,cd.name[p]);
					if (!strcmp(evo[i].place, cd.name[p]))
					{
						//if(!strcmp(evo[i].name,"女子100米"))printf("  %d\t%d",cd.time[p],evo[i].time+3*60*60);
						if (!evo[i].time)break;
						if (cd.time[p] <= (evo[i].time + 3 * 60 * 60))break;
						if (cd.time[p]>(state + 11 * 60 * 60))break;
						//冲突判断
						congtu = 0;
						for (c = 0; c < sys_info.eve; c++)
						{//预赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].time)continue;//还没安排
							for (x = 0; x < evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (evo[c].maxpeople<4)continue;//比赛被取消
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 0))
								{//项目有同一个人
									if (cd.time[p]>evo[c].time && cd.time[p] < (evo[c].time + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
						}
						if (!congtu)//还不冲突就检查决赛
						for (c = 0; c < sys_info.eve; c++)
						{//决赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].hasFinals)continue;//没有决赛
							if (!evo[c].etime)continue;//还没安排
							for (x = 0; x<evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 1))
								{//项目有同一个人
									if (cd.time[p]>evo[c].etime && cd.time[p] < (evo[c].etime + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
						}
						if (congtu)continue;
						//
						evo[i].etime = cd.time[p];
						cd.time[p] += evo[i].timecost * 60;
						printf("\n[决赛]%s\t%s\t用时：%d分钟\t", evo[i].name, evo[i].place, evo[i].timecost);
						timetochar(evo[i].etime);
						printf("\n");
						setnum++;
						break;
					}
				}
			}
			//判断决赛尾
		}
		for (i = 0; i < cd.num; i++)cd.time[i] = state + 14 * 60 * 60;//初始化场地时间
		for (i = 1; i < sys_info.eve; i += 2)
		{//下午
			if (!evo[i].time)
			{//判断预赛头
				for (p = 0; p<cd.num; p++)
				{
					if (!strcmp(evo[i].place, cd.name[p]))
					{
						if (cd.time[p]>(state + 17 * 60 * 60))break;
						//冲突判断
						congtu = 0;
						for (c = 0; c < sys_info.eve; c++)
						{//预赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].time)continue;//还没安排
							for (x = 0; x < evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (evo[c].maxpeople<4)continue;//比赛被取消
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 0))
								{//项目有同一个人
									if (cd.time[p]>evo[c].time && cd.time[p] < (evo[c].time + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
						}
						if (!congtu)//还不冲突就检查决赛
						for (c = 0; c < sys_info.eve; c++)
						{//决赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].hasFinals)continue;//没有决赛
							if (!evo[c].etime)continue;//还没安排
							for (x = 0; x<evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 1))
								{//项目有同一个人
									if (cd.time[p]>evo[c].etime && cd.time[p] < (evo[c].etime + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
						}
						if (congtu)continue;
						//
						evo[i].time = cd.time[p];
						cd.time[p] += evo[i].timecost * 60;
						printf("\n[第一轮]%s\t%s\t用时：%d分钟\t", evo[i].name, evo[i].place, evo[i].timecost);
						timetochar(evo[i].time);
						printf("\n");
						setnum++;
						break;
					}
				}
			}
			//判断预赛尾
		}
		for (i = 0; i < sys_info.eve; i++)
		{//下午
			if (!evo[i].etime && evo[i].hasFinals)
			{//判断决赛头
				for (p = 0; p<cd.num; p++)
				{
					if (!strcmp(evo[i].place, cd.name[p]))
					{
						//timetochar(cd.time[p]);timetochar(evo[i].time);
						if (!evo[i].time)break;
						if (cd.time[p] <= (evo[i].time + 3 * 60 * 60))break;
						if (cd.time[p]>(state + 17 * 60 * 60))break;
						//冲突判断
						congtu = 0;
						for (c = 0; c < sys_info.eve; c++)
						{//预赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].time)continue;//还没安排
							for (x = 0; x < evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (evo[c].maxpeople<4)continue;//比赛被取消
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 0))
								{//项目有同一个人
									if (cd.time[p]>evo[c].time && cd.time[p] < (evo[c].time + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
							if (!evo[c].etime)continue;
						}
						if (!congtu)//还不冲突就检查决赛
						for (c = 0; c < sys_info.eve; c++)
						{//决赛
							if (evo[c].id == evo[i].id)continue;//相同跳过
							if (!evo[c].hasFinals)continue;//没有决赛
							if (!evo[c].etime)continue;//还没安排
							for (x = 0; x<evo[c].maxpeople; x++)
							{
								if (!evo[c].prePerson[x].id)continue;
								if (Event_hasPer(evo[i], evo[c].prePerson[x].id, 1))
								{//项目有同一个人
									if (cd.time[p]>evo[c].etime && cd.time[p] < (evo[c].etime + evo[c].timecost))
									{
										//时间冲突
										congtu = 1;
										break;
									}
								}
							}
							if (!evo[c].etime)continue;
						}
						if (congtu)continue;
						//
						evo[i].etime = cd.time[p];
						cd.time[p] += evo[i].timecost * 60;
						printf("\n[决赛]%s\t%s\t用时：%d分钟\t", evo[i].name, evo[i].place, evo[i].timecost);
						timetochar(evo[i].etime);
						printf("\n");
						setnum++;
						break;
					}
				}
			}
			//判断决赛尾
		}
		state += 24 * 60 * 60;//下一天
	}

	//printf("\n%ld\n",state);
	FILE *fp;
	fp = fopen("data\\eveinfo.txt", "w");
	fwrite(evo, sizeof(SportsEvent), sys_info.eve, fp);
	fclose(fp);
	free(evo);
	fflush(stdin);
	getchar();
	toTableMenu();
	return 0;
}

//显示所有有决赛的项目
int showFinalEvent()
{
	long eveid;
	SportsEvent* evo = (SportsEvent *)malloc((sys_info.eve)*sizeof(SportsEvent));
	Event_getlist(evo);
	for (int o = 0; o < sys_info.eve; o++)
	{
		if (Event_getinfo(evo[o].id).hasFinals)
		{
			printf("\n[项目%d]   ", o);
			printf("ID:%ld ", evo[o].id);
			if (evo[o].type / 10 == 1)
				printf("男子 ");
			else
				printf("女子 ");
			if (evo[o].type % 2)
				printf("径赛 ");
			else
				printf("田赛 ");
		}
	}
	return 0;
}

//主菜单
int adminMenu()
{
	int choice;
	long type;
	int state;
	long id;
	while (1)
	{

		while (1)
		{
			clearScreen(500);
			printf("\t\t\t *************************************************** \n");
			printf("\n\t\t\t\t\t汕头大学校运会管理系统\t\t    \n");
			printf("\n\t\t\t *************************************************** \n\n");
			printf("\n\t\t\t 1. 账号管理          \t 2. 通知栏管理    \n");
			printf("\n\t\t\t 3. 项目管理          \t 4. 组织管理  \n");
			printf("\n\t\t\t 5. 生成秩序册        \t 6. 开放/锁定报名  \n");
			printf("\n\t\t\t 7. 登记预赛成绩      \t 8. 登记决赛成绩  \n");
			printf("\n\t\t\t 9. 查询预赛情况      \t 10. 查询决赛情况  \n");
			printf("\n\t\t\t 11.筛选进决赛运动员   \t 12. 退出登录\n");
			if (scanf("%d", &choice) == 1 && (choice <= 12 && choice >= 1))
				break;
			else
				printf("\t\t\t 您的输入有误，请重新输入");
		}
		switch (choice)
		{
		case 1:                                //此case为账号管理模块
			fflush(stdin);
			do
			{
				while (1)
				{
					clearScreen(400);
					printf("\n\t\t\t\t\t\t账号管理界面\t\t    \n");
					printf("\n\t\t\t 1. 查询已注册用户      \t 2. 重置用户密码    \n");
					printf("\n\t\t\t 3. 删除用户账号        \t 4. 重置管理员密码  \n");
					printf("\n\t\t\t 5. 返回上一级\n");

					if (scanf("%d", &type) == 1 && (type <= 5 && type >= 1))
						break;
					else
						printf("\t 您的输入有误，请重新输入");
				}
				if (type == 1)			//查询已注册用户
				{
					Person_list();
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}
				else if (type == 2)		//重置用户密码
				{
					Person_list();
					printf("\n\t 请输入需要修改的用户ID \n");
					long id;
					PerInfo user;
					scanf("%ld", &id);
					user.id = id;
					printf("\n\t 请重置密码 \n");
					char password[20];
					inputPasswd(password);
					strcpy(user.password, password);
					int state = Person_change(user, 6);
					if (state == 0) printf("\n\t修改成功！\n");
					else if (state == 7) printf("\n\t不存在此账号\n");
					else if (state == 9) printf("\n\t打开文件失败\n");
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}
				else if (type == 3)			//删除用户账号
				{
					printf("\n\t 请输入需要删除的用户id \n");
					long id;
					PerInfo user;
					scanf("%ld", &id);
					int state = Person_delete(id);
					if (state == 0) printf("\n\t删除成功！\n");
					else if (state == 1) printf("\n\t不能删除id为0的用户\n");
					else if (state == 2) printf("\n\t此用户不存在\n");
					else if (state == 3) printf("\n\t成员为0，删除失败！\n");
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}
				else if (type == 4)			//重置管理员密码
				{
					PerInfo admin;
					char oldpassword[20];
					int islogin = 0;
					int	count = 0;
					int reinput = 0;
					while (islogin == 0)
					{
						printf("\n\t 请输入原始密码 \n");
						inputPasswd(oldpassword);
						if (strcmp(oldpassword, sys_info.password) == 0)
						{
							islogin = 1;
							printf("\n\t 验证成功，请重置密码 \n");
							char password1[20];
							inputPasswd(password1);
							printf("\n\t 请再次输入密码 \n");
							char password2[20];
							inputPasswd(password2);
							if (strcmp(password1, password2) == 0)
							{
								if (strcmp(password1, oldpassword) != 0)
								{
									int state = sys_changepsw(password1);
									if (state == 0) printf("\n\t修改成功！\n");
									else if (state == 1) printf("\n\t非法字符\n");
								}
								else
								{
									printf("\n\t与旧密码相同，修改失败\n\t");
								}
							}
							else
							{
								printf("\n\t前后密码不一致\n");
								count++;
							}
						}
						else {
							count++;
							printf("\n\t原始密码错误,需要重新输入吗? 1.是 2.否\n");
							if (reinput == 2)
								islogin = 1;
							if (count == 3)
							{
								printf("\n\t你已经输入错误三次了\n\t");
								break;
							}
						}
					}
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}
			} while (type != 5);
			break;
		case 2:					//此case为通知栏管理模块
			fflush(stdin);
			do
			{
				while (1)
				{
					clearScreen(400);
					printf("\n\t\t\t\t\t通知栏管理界面\t\t    \n");
					printf("\n\t\t\t 1. 发布个人通知               \t 2. 发布项目通知    \n");
					printf("\n\t\t\t 3. 发布公告            \t 4. 返回上一级  \n");
					if (scanf("%d", &type) == 1 && (type <= 4 && type >= 1))
						break;
					else
						printf("\n\t 您的输入有误，请重新输入\n");
				}
				if (type == 1)		//发布个人通知
				{
					long id;
					printf("\n\t请输入被通知用户的id(必须大于10000)\n");
					scanf("%ld", &id);
					if (id > 10000)
					{
						printf("\n\t请输入通知\n");
						char msg[300];
						scanf("%s", &msg);
						int state = Person_notice(id, msg);
						if (state == 0)printf("\n\t通知成功\n");
						else
						{
							printf("\n\t打开文件失败\n");
						}
					}
					else
					{
						printf("\n\t输入id非法\n");
					}
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}
				else if (type == 2)			//发布项目通知
				{
					long id;
					showAllEventID();
					printf("\n\t请输入项目的id(1000-9999)\n");
					scanf("%ld", &id);
					if (id >= 1000 && id <= 9999)
					{
						if (Event_had(id) == 0) printf("\n\t不存在此项目\n");
						else
						{
							int match = 0, i = 0;
							printf("\n\t请选择此时的比赛阶段 1.预赛 2.决赛\n");
							scanf("%d", &match);
							SportsEvent eve;
							eve = Event_getinfo(id);
							if (match == 1)
							{
								long prePersonid[100];
								for (int i = 0; i < 100; i++)
									prePersonid[i] = eve.prePerson[i].id;
								printf("\n\t请输入通知\n");
								char msg[300];
								scanf("%s", &msg);
								int state;
								for (int i = 0; i < 100; i++)
								{
									state = Person_notice(prePersonid[i], msg);
								}
								if (state == 0)printf("\n\t通知成功\n");
								else
								{
									printf("\n\t打开文件失败\n");
								}
							}
							else
							{
								if (match == 2)
								{
									long finPersonid[20];
									for (int i = 0; i < 20; i++)
										finPersonid[i] = eve.finPerson[i].id;
									printf("\n\t请输入通知\n");
									char* msg;
									scanf("%s", &msg);
									int state;
									for (int i = 0; i < 20; i++)
									{
										state = Person_notice(finPersonid[i], msg);
									}
									if (state == 0)printf("\n\t通知成功\n");
									else
									{
										printf("\n\t打开文件失败\t");
									}
								}
								else printf("\n\t输入错误\n");
							}
						}
					}
					else
					{
						printf("\n\t输入id非法\n");
					}
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}
				else if (type == 3)		//发布公告
				{
					printf("\n\t请输入通知\t");
					char msg[300];
					scanf("%s", &msg);
					int state = Person_notice(10000, msg);
					if (state == 0)printf("\n\t通知成功\n");
					else
					{
						printf("\n\t打开文件失败\n");
					}
					fflush(stdin);
					printf("\n  按回车键返回上一级");
					getchar();
				}

			} while (type != 4);
			break;
		case 3:					//此case为项目管理模块
			fflush(stdin);
			do
			{
				while (1)
				{
					clearScreen(400);
					showAllEventID();
					printf("\n\t\t\t\t\t项目管理界面\t\t    \n");
					printf("\n\t\t\t 1. 增加项目            \t 2. 减少项目  \n");
					printf("\n\t\t\t 3. 返回上一级\n");
					printf("\n\t\t\t输入[1-3]操作或输入项目ID进行修改：\n");
					scanf("%d", &type);
					if ((type <= 3 && type >= 1)||(Event_had(type)))
						break;
					else
						printf("\t 您的输入有误，请重新输入\n");
				}
				if (type == 1)			//增加项目
				{
					creatEvent();
				}
				else if (type == 2)			//减少项目
				{
					delectEvent();
				}
				else if(type!=3)
				{
					changeEvent(type);
				}
			} while (type != 3);
			break;
		case 4:					//此case为组织管理模块
			fflush(stdin);
			do
			{
				while (1)
				{
					clearScreen(400);
					printf("\n\t\t\t\t\t目前院\t\t    \n");
					for (int i = 0; i < sys_info.numyuan; i++)
					{
						printf("\n\t\t\t%s\n", sys_info.yuan[i]);
					}
					printf("\n\t\t\t\t\t目前系\n");
					for (int i = 0; i < sys_info.numxi; i++)
					{
						printf("\n\t\t\t%s\n", sys_info.xi[i]);
					}
					printf("\n\t\t\t\t\t组织管理界面\t\t    \n");
					printf("\n\t\t\t 1. 增加院            \t 2. 减少院    \n");
					printf("\n\t\t\t 3. 增加系            \t 4. 减少系  \n");
					printf("\n\t\t\t 5. 返回上一级");
					scanf("%d", &type);
					if (type <= 5 && type >= 1)
						break;
					else
						printf("\t 您的输入有误，请重新输入\n");
				}
				if (type == 1)			//增加院
				{
					printf("\n\t请输入该院的名称\n");
					char yuan[20];
					scanf("%s", &yuan);
					int state = sys_incyuan(yuan);
					if (state == 0)
						printf("\n\t增加成功\n");
					else if (state == 1)
						printf("\n\t已存在\n");
					else if (state == 2)
						printf("\n\t已满员\n");
					else if (state == 3)
						printf("\n\t输入错误\n");
				}
				else if (type == 2)	//减少院
				{
					printf("\n\t请输入该院的名称\n");
					char yuan[20];
					scanf("%s", &yuan);
					int state = sys_decyuan(yuan);
					if (state == 0)
						printf("\n\t减少成功\n");
					else if (state == 1)
						printf("\n\t学院数目为0\n");
					else if (state == 2)
						printf("\n\t不存在该学院\n");
					else if (state == 3)
						printf("\n\t输入错误\n");
				}
				else if (type == 3)	//增加系
				{
					printf("\n\t请输入该系的名称\n");
					char xi[20];
					scanf("%s", &xi);
					int state = sys_incxi(xi);
					if (state == 0)
						printf("\n\t增加成功\n");
					else if (state == 1)
						printf("\n\t已存在该系\n");
					else if (state == 2)
						printf("\n\t已满员\n");
					else if (state == 3)
						printf("\n\t输入错误\n");
				}
				else if (type == 4)	//减少系
				{
					printf("\n\t请输入该系的名称\n");
					char xi[20];
					scanf("%s", &xi);
					int state = sys_decxi(xi);
					if (state == 0)
						printf("\n\t减少成功\n");
					else if (state == 1)
						printf("\n\t不存在该系\n");
					else if (state == 2)
						printf("\n\t不存在该系\n");
					else if (state == 3)
						printf("\n\t输入错误\n");
				}

			} while (type != 5);
			break;
		case 5:					//此case为秩序册生成模块
			creatTableMenu();
			fflush(stdin);
			printf("\n  按回车键返回上一级");
			getchar();
			break;
		case 6:
			fflush(stdin);
			do
			{
				while (1)
				{
					clearScreen(400);
					printf("\n\t1. 开放报名 \n");
					printf("\n\t2. 锁定报名\n");
					printf("\n\t3. 返回上一级\n");
					scanf("%d", &state);
					if (state <= 3 && state >= 1)
						break;
					else
						printf("\t 您的输入有误，请重新输入\n");
				}
				if (state == 1) unlockjoin();
				else if (state == 2) lockjoin();
			} while (state != 3);
			break;
		case 7:	//登记预赛成绩
			registerScore(0);
			break;
		case 8:	//登记决赛成绩
			registerScore(1);
			break;
		case 9:	//查询预赛情况
			clearScreen(400);
			showAllEventID();
			printf("\n\n\t\t请输入要修改项目的ID:\n");
			scanf("%ld", &id);
			state = Event_had(id);
			if (state == 1)
				getoneInfo(id);
			else
			{
				printf("\n  不存在此项目\n");
				printf("\n  正在返回上一级......");
				Sleep(1000);
			}
			break;
		case 10:
			clearScreen(400);
			showFinalEvent();
			printf("\n  请输入要查询项目的ID:\n");
			scanf("%ld", &id);
			state = Event_had(id);
			if (state == 1)
				getTwoInfo(id);
			else
			{
				printf("\n  不存在此项目\n");
				printf("\n  正在返回上一级......");
				Sleep(1000);
			}
			break;
		case 11:
			while (1)
			{
				clearScreen(400);
				showAllEventID();
				printf("\n\n  请输入项目ID ");
				long eveid;
				if (scanf("%ld", &eveid))
				{
					if (Event_had(eveid))
					{
						selcetTwoPlayer(eveid);
						Sleep(100);
						break;
					}
					else
					{
						printf("\n  不存在此项目ID\n");
						continue;
					}
				}
			}
			Sleep(500);
			break;
		case 12:
			return -1;
		default:
			return 0;
		}
	}
}