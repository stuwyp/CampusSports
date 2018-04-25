#include "stdafx.h"
/*
	char name[20];
	long id;//编号
	int type;//0田、1径
	char place[20];//场地
	int timecost;//用时，分钟
	int maxpeople;//上限人数
	int hasFinals;//有无决赛 0无 1有
	int finNum;//有决赛时决赛人数
	long time;//比赛时间戳，秒
	long etime
	Participant prePerson[100] ;//预赛人数最多100
	Participant finPerson[20] ;//决赛人数最多20
*/
int Event_getlist(SportsEvent* evo){
	FILE * fp= fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return 1;//打开文件失败
    int i=sys_info.eve;
	if(i==0) return 2;//0项目
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
	return 0;
}
SportsEvent Event_getinfo(long id){
	SportsEvent msg;
	msg.id=0;
	FILE *fp = fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return msg;
    int i=sys_info.eve,o;
	if(i==0) return msg;
	SportsEvent* evo;
	evo=(SportsEvent *) malloc(i*sizeof(SportsEvent));
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
    for(o = 0; o < sys_info.eve; o++){
		//printf("%d,%s",o,evo[o].name);
		if(evo[o].id==id){
			msg=evo[o];
			break;
		}
	}
	free(evo);
	return msg;//如果放回的msg.id为0则不存在该项目
}
int Event_had(long id){
	SportsEvent msg=Event_getinfo(id);
	if(msg.id)
			return 1;//存在项目
		else 
			return 0;
}
int Event_hadname(char* ming){
	FILE *fp = fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return 0;
    int i=sys_info.eve,o;
	if(i==0) return 0;
	SportsEvent* evo;
	evo=(SportsEvent *) malloc(i*sizeof(SportsEvent));
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
	long msgid=0;
	char buff[20];
	strcpy(buff,ming);
    for(o = 0; o < sys_info.eve; o++){
		//printf("%s,%s",buff,evo[o].name);
		if(strcmp(evo[o].name,buff)==0){
			msgid=evo[o].id;
			//puts("存在");
			break;
		}
	}
	free(evo);
	if(msgid)
			return msgid;//存在项目
		else 
			return 0;
}
int Event_hasPer(SportsEvent a,long id,int isfinal){
	if(isfinal){
		for(int i=0;i<a.finNum;i++){
			if(a.finPerson[i].id==id)return 1;
		}
		return 0;
	}else{
	for(int i=0;i<a.maxpeople;i++){
		if(a.prePerson[i].id==id)return 1;
	}
	return 0;
	}
}

int Event_inc(long eveid,long peoid,int type){
	if(Event_had(eveid)==0)return 11;//不存在项目
	FILE * fp= fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return 1;//打开文件失败
    int i=sys_info.eve;//项目数
	if(i==0) return 2;//0项目
	SportsEvent* evo,msg;//所有项目，单个项目
	evo=(SportsEvent *) malloc(i*sizeof(SportsEvent));
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
	int o;Participant one;//新增参赛者
	one.id=peoid;one.score=0;
	for(int p=0;p<i;p++){
		if(evo[p].id==eveid){
			if(type==0){//第一轮
				if(evo[p].maxpeople==100)return 4;
				for(o = 0; o < 100; o++){//检查有没有参加
					if(evo[p].prePerson[o].id==peoid){
						free(evo);
						return 3;//已加入
					}
				}
				for(o = 0; o < 100; o++){//参加
					if(evo[p].prePerson[o].id<10001){//少于1w就不是用户
						evo[p].prePerson[o]=one;
						break;//已加入
					}
				}
				evo[p].maxpeople++;
			}else{//决赛加入
				if(evo[p].finNum==20)return 4;
				for(o = 0; o < 20; o++){
					if(evo[p].finPerson[o].id==peoid){
						free(evo);
						return 3;//已加入
					}
				}
				for(o = 0; o < 20; o++){//参加
					if(evo[p].finPerson[o].id<10001){
						evo[p].finPerson[o]=one;
						break;//已加入
					}
				}
				evo[p].finNum++;
			}
		}
	}
	fp = fopen("data\\eveinfo.txt", "w");
	fwrite(evo, sizeof(SportsEvent), i, fp);
	fclose(fp);
	free(evo);
	return 0;
}
int Event_dec(long eveid,long peoid,int type){
	if(Event_had(eveid)==0)return 11;//不存在项目
	int i=sys_info.eve;//项目数
	if(i==0) return 2;//0项目
	FILE * fp= fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return 1;//打开文件失败
	SportsEvent* evo,msg;//所有项目，单个项目
	evo=(SportsEvent *) malloc(i*sizeof(SportsEvent));
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
	int o;
	for(int p=0;p<i;p++){
		if(evo[p].id==eveid){
			if(type==0){//第一轮
				if(evo[p].maxpeople==0){free(evo);return 3;}
				for(o = 0; o < 100; o++){//检查有没有参加
					if(evo[p].prePerson[o].id==peoid){
						evo[p].prePerson[o].id=evo[p].prePerson[o].score=0;//删除参赛
						evo[p].maxpeople--;
						fp = fopen("data\\eveinfo.txt", "w");
						
						fwrite(evo, sizeof(SportsEvent), i, fp);
						fclose(fp);
						free(evo);
						return 0;//已删除
					}
				}
			}else{//决赛
				if(evo[p].finNum==0){free(evo);return 3;}
				for(o = 0; o < 20; o++){
					if(evo[p].finPerson[o].id==peoid){
						evo[p].finPerson[o].id=evo[p].finPerson[o].score=0;
						evo[p].finNum--;
						
						fp = fopen("data\\eveinfo.txt", "w");
						fwrite(evo, sizeof(SportsEvent), i, fp);
						//printf("\n—%d—%ld—%d—\n",evo[p].maxpeople,evo[p].finPerson[o].id,evo[p].finNum);
						fclose(fp);
						free(evo);
						return 0;//已删除
					}
				}
			}
		}
	}
	free(evo);
	return 3;//没有这个参赛
}
int Event_changescore(long eveid,long peoid,int type,long score){
	if(Event_had(eveid)==0)return 11;//不存在项目
	FILE * fp= fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return 1;//打开文件失败
    int i=sys_info.eve;//项目数
	if(i==0) return 2;//0项目
	SportsEvent* evo,msg;//所有项目，单个项目
	evo=(SportsEvent *) malloc(i*sizeof(SportsEvent));
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
	int o;Participant one;//新增参赛者
	one.id=peoid;one.score=score;
	for(int p=0;p<i;p++){
		if(evo[p].id==eveid){
			if(type==0){//第一轮
				if(evo[p].maxpeople==0)return 3;
				for(o = 0; o < 100; o++){//检查有没有参加
					if(evo[p].prePerson[o].id==peoid){
						evo[p].prePerson[o].score=score;//改成绩
						fp = fopen("data\\eveinfo.txt", "w");
						fwrite(evo, sizeof(SportsEvent), i, fp);
						fclose(fp);
						free(evo);
						return 0;//已删除
					}
				}
			}else{//决赛
				if(evo[p].finNum==0)return 3;
				for(o = 0; o < 20; o++){
					if(evo[p].finPerson[o].id==peoid){
						evo[p].finPerson[o].score=score;
						fp = fopen("data\\eveinfo.txt", "w");
						fwrite(evo, sizeof(SportsEvent), i, fp);
						fclose(fp);
						free(evo);
						return 0;//已删除
					}
				}
			}
		}
	}
	free(evo);
	return 3;//没有这个参赛
}
int Event_change(SportsEvent msg,int type){
	if(Event_had(msg.id)==0)return 11;//不存在项目
	int i,o;
	switch(type){
		case 1:	
			i=Char_isok(msg.name);
			if(i)return 1;
			i=Event_hadname(msg.name);
			if(i)return 1;
			break;
		case 3:
			if(msg.type!=10 && msg.type!=11 && msg.type!=20 && msg.type!=21) return 3;
			break;
		case 4:
			i=Char_isok(msg.place);
			if(i)return 4;
			break;
		case 5:
			if(msg.timecost<0 )return 5;
			break;
		case 6:
			if(msg.maxpeople<0 || msg.maxpeople>100)return 6;
			break;
		case 7:
			if(msg.hasFinals<0 || msg.hasFinals>1) return 7;
			break;
		case 8:
			if(msg.finNum<0 || msg.finNum>20)return 8;
			break;
		case 9:
			if(msg.time<0 )return 9;
			break;
		case 10:
			if(msg.etime<0 )return 9;
			break;
		default:
			return 10;
	}
	FILE * fp;
	fp = fopen("data\\eveinfo.txt", "r");
	if(fp==NULL) return 9;
    i=sys_info.eve;
	SportsEvent* evo=(SportsEvent *) malloc(i*sizeof(SportsEvent));
    fread(evo, sizeof(SportsEvent), i, fp);
    fclose(fp);
    for(o = 0; o < i; o++){
		if(evo[o].id==msg.id){
			switch(type){
				case 1:	
					strcpy(evo[o].name,msg.name);
					break;
				case 3:
					evo[o].type=msg.type;
					break;
				case 4:
					strcpy(evo[o].place,msg.place);
					break;
				case 5:
					evo[o].timecost=msg.timecost;
					break;
				case 6:
					evo[o].maxpeople=msg.maxpeople;
					break;
				case 7:
					evo[o].hasFinals=msg.hasFinals;
					break;
				case 8:
					evo[o].finNum=msg.finNum;
					break;
				case 9:
					evo[o].time=msg.time;
					break;
				case 10:
					evo[o].etime=msg.etime;
					break;
				}
			break;
		}
	}
	fp = fopen("data\\eveinfo.txt", "w");
	fwrite(evo, sizeof(SportsEvent), i, fp);
	fclose(fp);
	free(evo);
	return 0;
}
int Event_delete(long id){
	if(id==0)return 1;//不能删0
	if(!Event_had(id))return 2;//不存在
	FILE *fp;
	SportsEvent *evo,*sen;
	int i,p,o=0;
	fp = fopen("data\\eveinfo.txt", "r+");
	if(sys_info.eve){//有1个或以上
		if(sys_info.eve==1){
			fclose(fp);
			fp = fopen("data\\eveinfo.txt", "w");
		}else{
			i=sys_info.eve;
			evo=(SportsEvent *) malloc((i)*sizeof(SportsEvent));//原本 动态声明数组
			sen=(SportsEvent *) malloc((i-1)*sizeof(SportsEvent));//剩余 删完项目-1
			fread(evo, sizeof(SportsEvent), i, fp);
			fclose(fp);
			for(p=0;p<i;p++){
				if(evo[p].id!=id) sen[o++]=evo[p];
			}
			fp = fopen("data\\eveinfo.txt", "w");
			fwrite(sen, sizeof(SportsEvent), o, fp);
			free(evo);
			free(sen);
		}
		fclose(fp);
		sys_dec(1);//减少项目记录
	}else{
		fclose(fp);
		return 3;//项目数0，删除失败
	}
	return 0;
}
int Event_create(SportsEvent msg){
	int i;
	i=Char_isok(msg.name);
	if(i)return 1;//检查名字
	i=Event_hadname(msg.name);
	if(i)return 1;
	if(msg.type!=10 && msg.type!=11 && msg.type!=20 && msg.type!=21) return 3;//检查田径
	i=Char_isok(msg.place);
	if(i)return 4;//检查场地
	if(msg.timecost<0 ) return 5;//检查耗时
	if(msg.hasFinals<0 || msg.hasFinals>1) return 7;//检查决赛
	if(sys_info.eve>8888)return 10;//太多项目
	i=rand()%(9999-1000+1)+1000;
	while(Event_had(i))	i=rand()%(9999-1000+1)+1000;
	msg.time=msg.etime=msg.maxpeople=msg.finNum=0;
	//检查完毕，进行录入
	msg.id=i;
	for(i=0;i<100;i++)msg.prePerson[i].id=msg.prePerson[i].score=0;
	for(i=0;i<20;i++)msg.finPerson[i].id=msg.finPerson[i].score=0;
	FILE *fp;
	SportsEvent *evo;
	fp = fopen("data\\eveinfo.txt", "r+");
	if(sys_info.eve){//有1个或以上
		i=sys_info.eve;
		evo=(SportsEvent *) malloc((i+1)*sizeof(SportsEvent));//动态声明数组
		fread(evo, sizeof(SportsEvent), i, fp);
		fclose(fp);
		evo[i++]=msg;
		fp = fopen("data\\eveinfo.txt", "w");
		fwrite(evo, sizeof(SportsEvent), i, fp);
		free(evo);
	}else{
		evo=&msg;
		fwrite(evo, sizeof(SportsEvent), 1, fp);
	}
	fclose(fp);

	sys_inc(1);//增加项目记录
	return msg.id;
}


void Event_list(){//获取所有成员例子
	if(sys_info.eve==0) return;
	SportsEvent* evo=(SportsEvent *) malloc((sys_info.eve)*sizeof(SportsEvent));
	Event_getlist(evo);
    for(int o = 0; o < sys_info.eve; o++){
		printf("> %d_%s_%d_%s_%d_%d_%d_%d_%d\n",evo[o].id,evo[o].name,evo[o].type,evo[o].place,evo[o].timecost,evo[o].maxpeople,evo[o].hasFinals,evo[o].finNum,evo[o].time);
	}
	free(evo);
}

