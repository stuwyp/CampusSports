// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"
#include "conio.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <direct.h> 
typedef struct changdi{
	long num;
	char name[100][20];
	long time[100];
}changdi;


typedef struct sorteve{
	long id;
	int isFinals;//决赛 0否 1是
	long time;
	int cancle;
}sorteve;

typedef struct SysInfo{
	int peo;//人数
	int eve;//项目数
	int not;//通知数
	char password[20];
	int canjoin;//是否开放报名 1为可以报名，默认1
	int numyuan;//院数
	int numxi;//系数
	char yuan[20][20];//20个院数组
	char xi[20][20];//20个系数组
}SysInfo;//系统信息

typedef struct PerInfo{
	char name[20];
	long id;
	int gender;//性别 0男 1女
	char organization[20];//学院书院
	char faculty[20];//系
	char password[20];//密码
}PerInfo;//运动员个人信息

typedef struct Notice{
	long id;
	char msg[300];
	long time;
}Notice;//通知

typedef struct Participant{
	long id;
	long score;
}Participant;//参赛者信息

typedef struct joinevent{
	char evename[20];//参赛项目
	int joinnum;//参赛人数
	int joinenum;//决赛人数
	int score;//得分
}joinevent;

typedef struct SportsEvent{
	char name[20];
	long id;//编号
	int type;//10男田、11男径、20女田、21女径
	char place[20];//场地
	int timecost;//用时，分钟
	int maxpeople;//上限人数
	int hasFinals;//有无决赛 0无 1有
	int finNum;//有决赛时决赛人数
	long time;//比赛时间戳，秒
	long etime;//决赛时间
	Participant prePerson[100] ;//预赛人数最多100
	Participant finPerson[20] ;//决赛人数最多20
}SportsEvent;//比赛项目信息

//操作
extern void clearScreen(int sleepS);
extern int inputPasswd(char * password);
extern int login(long id, char password[], int type);
extern int signup(PerInfo  msg);

//菜单
extern int adminMenu();
extern int userMenu(long id);
extern int touristMenu();
extern int signupErrorMenu();
extern int toSignupMenu();
extern int toLoginMenu(int type);
extern void welcome();
extern int loginMenu();
extern void mainMenu();

//接口
extern int Char_isok(char * msg);//检查字符非法与否
extern int sys_dec(int type);//减少数目
extern int sys_inc(int type);//增加数目
extern int sys_init();//初始化
extern PerInfo Person_getinfo(long id);			//获取信息（通知）		
extern int Person_create(PerInfo msg);		//创建用户		
extern int Person_delete(long id);		//删除用户			
extern int Person_had(long id);	//是否存在用户		
extern int Person_change(PerInfo msg, int id);	//修改用户信息
extern int Person_getlist(PerInfo * evo);	//获取所有用户
extern int Person_notice(long id, char * msg);		//通知用户	
extern int Person_getnotice(long id, Notice * msn);	//显示最近信息（通知）					
extern void Person_list();		//获取所有成员例子		
extern void Person_mynotice();		//获取成员通知例子						
extern SportsEvent Event_getinfo(long id);	//查询项目（运动员）
extern int Event_create(SportsEvent msg);	//增加项目
extern int Event_delete(long id);	//删除项目
extern int Event_had(long id);	//是否存在项目（按ID查）
extern int Event_hadbyname(char * name);	//是否存在项目（按name查）
extern int Event_change(SportsEvent msg, int type);	//修改项目
extern int Event_getlist(SportsEvent * evo);	//获取所有项目
extern int Event_inc(long eveid, long period, int type);	//增加项目的运动员（项目ID，用户ID，田/径）
extern int Event_dec(long eveid, long period, int type);	// 删除项目的运动员（项目ID，用户ID，田 / 径）
extern int Event_changescore(long eveid, long period, int type, long score);	//修改项目的运动员成绩 （项目ID，用户ID，田/径，成绩）
extern int Event_hasPer(SportsEvent a,long id,int isfinal);	//项目有没这个人
extern void Event_list();
extern long  charToScore(char * msg);//char * msg

extern int sys_incyuan(char* yuan);//增加院
extern int sys_decyuan(char* yuan);
extern int sys_incxi(char* xi);//增加系
extern int sys_decxi(char* xi);
extern int sys_changepsw(char* msg);
extern int sys_changecj(int i);
extern int cmp(const void*a,const void*b);
extern int cmp2(const void*a,const void*b);
extern void  scoreToChar(long score, char * msg);
extern int userSignupedEve(long id, int type);
extern int toTableMenu();
//变量
extern SysInfo sys_info;