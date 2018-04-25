#include "stdafx.h";
SysInfo sys_info;
void datain(){

	int i=0;SportsEvent eve;PerInfo peo;
	/*peo.id=2010101010;
	strcpy(peo.password ,"123");
	Person_change(peo,6);
	//peo=Person_getinfo(2010101010);
	
	
	sys_incyuan("思源");
	sys_incyuan("知行");
	sys_incxi("工");
	sys_incxi("商");
	

	
	peo.gender=0;
	strcpy(peo.password,"123456");
	strcpy(peo.name ,"小鸣");
	peo.id=2010101010;
	strcpy(peo.faculty,"工");
	strcpy(peo.organization,"知行");
	Person_create(peo);
	strcpy(peo.name ,"小刚");
	peo.id=2010101011;
	strcpy(peo.faculty,"商");
	strcpy(peo.organization,"思源");
	Person_create(peo);
	strcpy(peo.name ,"小乐");
	peo.id=2010101012;
	strcpy(peo.faculty,"工");
	strcpy(peo.organization,"思源");
	Person_create(peo);
	strcpy(peo.name ,"小a");
	peo.id=2010101013;
	strcpy(peo.faculty,"工");
	strcpy(peo.organization,"思源");
	Person_create(peo);
	strcpy(peo.name ,"小b");
	peo.id=2010101014;
	strcpy(peo.faculty,"工");
	strcpy(peo.organization,"思源");
	Person_create(peo);
	strcpy(peo.name ,"小c");
	peo.id=2010101015;
	strcpy(peo.faculty,"工");
	strcpy(peo.organization,"知行");
	Person_create(peo);
	strcpy(peo.name ,"小d");
	peo.id=2010101016;
	strcpy(peo.faculty,"工");
	strcpy(peo.organization,"知行");
	Person_create(peo);
	strcpy(peo.name ,"小e");
	peo.id=2010101017;
	strcpy(peo.faculty,"商");
	strcpy(peo.organization,"知行");
	Person_create(peo);
	

	
	strcpy(eve.name,"跑步");
	eve.type=11;
	strcpy(eve.place,"操场");
	eve.timecost=60;eve.time=1231231;eve.etime=1239120;

	eve.hasFinals=1;
	eve.id=Event_create(eve);
	Event_change(eve,9);Event_change(eve,10);
	printf("%d\n",i);
	strcpy(eve.name,"跳远");
	eve.type=10;
	strcpy(eve.place,"操场");
	eve.timecost=60;eve.time=1239;eve.etime=0;
	eve.hasFinals=0;
	eve.id=Event_create(eve);
	i=Event_change(eve,9);Event_change(eve,10);
	printf("-%d\n",i);
	
	
	Event_inc(1041,2010101010,0);
	Event_inc(1041,2010101011,0);
	Event_inc(1041,2010101012,0);
	Event_inc(1041,2010101013,0);
	Event_inc(1041,2010101014,0);
	Event_inc(1041,2010101015,0);
	Event_inc(1041,2010101016,0);
	Event_inc(1041,2010101017,0);

	Event_changescore(1041,2010101010,0,1);
	Event_changescore(1041,2010101011,0,2);
	Event_changescore(1041,2010101012,0,3);
	Event_changescore(1041,2010101013,0,4);
	Event_changescore(1041,2010101014,0,5);
	Event_changescore(1041,2010101015,0,6);
	Event_changescore(1041,2010101016,0,7);
	Event_changescore(1041,2010101017,0,8);

	Event_inc(1041,2010101010,1);
	Event_inc(1041,2010101011,1);
	Event_inc(1041,2010101012,1);
	Event_inc(1041,2010101013,1);
	Event_inc(1041,2010101014,1);
	Event_inc(1041,2010101015,1);

	Event_changescore(1041,2010101010,1,1);
	Event_changescore(1041,2010101011,1,2);
	Event_changescore(1041,2010101012,1,3);
	Event_changescore(1041,2010101013,1,4);
	Event_changescore(1041,2010101014,1,5);
	Event_changescore(1041,2010101015,1,6);

	Event_inc(1467,2010101010,0);
	Event_inc(1467,2010101011,0);
	Event_inc(1467,2010101012,0);
	Event_inc(1467,2010101013,0);*/
	Person_list();
	Event_list();
	eve=Event_getinfo(9500);
	printf("%d\n",eve.finNum);
	printf("%d\n预赛：\n",eve.maxpeople);
	for(i=0;i<eve.maxpeople;i++)
		printf("%d\t%d\n",eve.prePerson[i].id,eve.prePerson[i].score);
	puts("决赛：\n");
	for(i=0;i<eve.finNum;i++)
		printf("%d\t%d\n",eve.finPerson[i].id,eve.finPerson[i].score);
	printf("%d\n",i);

}
extern int creatTableMenu();
extern int registerScore(int);
int main()
{
	sys_init();
	//Person_notice(10000,"这是一条测试");
	//Person_notice(10000,"这是第二条测试");
	//Event_dec(1041, 2010101010, 1);
	//datain();
	//getchar();
	//toSituationMenu();
	mainMenu();
	//registerScore(1);
	//creatTableMenu();
	//changeEventInfo();
	return 0;
}