#include "stdafx.h"

int Person_getlist(PerInfo* evo){
	FILE * fp;
	fp = fopen("data\\perinfo.txt", "r");
	if(fp==NULL) return 1;//打开文件失败
    int i=sys_info.peo;
	if(i==0) return 2;//0用户
    fread(evo, sizeof(PerInfo), i, fp);
    fclose(fp);
	return 0;
}

int Person_notice(long id,char* msg){
	FILE * fp;
	Notice *notice,msn;
	time_t now;
	time(&now);
	msn.id=id;strcpy(msn.msg,msg);msn.time=now;
	notice=&msn;
	fp = fopen("data\\notinfo.txt", "r+");
	if(fp==NULL) return 1;//打开文件失败
	fseek(fp,0,SEEK_END);
    fwrite(notice, sizeof(Notice), 1, fp);
    fclose(fp);
	sys_inc(2);
	return 0;
}
int Person_getnotice(long id,Notice *msn){//限定显示最近20条信息
	FILE * fp;
	fp = fopen("data\\notinfo.txt", "r");
	if(fp==NULL) return 1;//打开文件失败
    int i=sys_info.not;
	if(i==0) return 2;//0通知
	Notice *allmsn=(Notice *) malloc((sys_info.not)*sizeof(Notice));
    fread(allmsn, sizeof(Notice), i, fp);
    fclose(fp);
	int o=0,p;
	for(p=i-1;p>=0;p--){
		if(allmsn[p].id==id){
			msn[o++]=allmsn[p];
			if(o==20)break;
		}
	}
	return 0;
}

PerInfo Person_getinfo(long id){
	FILE * fp;
	PerInfo msg;
	msg.id=0;
	fp = fopen("data\\perinfo.txt", "r");
	//printf("-1-");
	if(fp==NULL) return msg;
    int i=sys_info.peo,o;
	//printf("-2-");
	if(i==0) return msg;
	//printf("-%d-",i*sizeof(PerInfo));
	PerInfo* evo;
	evo=(PerInfo *) malloc(i*sizeof(PerInfo));
	//printf("-4-");
    fread(evo, sizeof(PerInfo), i, fp);
	//printf("-5-");
    fclose(fp);
	//printf("-6-");
    for(o = 0; o < i; o++){
		//printf("%d,%s",o,evo[o].name);
		if(evo[o].id==id){
			msg=evo[o];
			break;
		}
	}
	free(evo);
	//printf("-%ld-",msg.id);
	return msg;//如果放回的msg.id为0则不存在该学生
}
int Person_had(long id){
	PerInfo peo;
	peo=Person_getinfo(id);
	if(peo.id)
			return 1;//存在用户
		else 
			return 0;
}
int Person_delete(long id){
	if(id==0)return 1;//不能删0
	if(!Person_had(id))return 2;//不存在
	FILE *fp = NULL;
	PerInfo *evo,*sen;
	int i,p,o=0;
	fp = fopen("data\\perinfo.txt", "r+");
	if(sys_info.peo){//有1个或以上
		if(sys_info.peo==1){
			fclose(fp);
			fp = fopen("data\\perinfo.txt", "w");
		}else{
			i=sys_info.peo;
			evo=(PerInfo *) malloc((i)*sizeof(PerInfo));//原本 动态声明数组
			sen=(PerInfo *) malloc((i-1)*sizeof(PerInfo));//剩余 删完人-1
			fread(evo, sizeof(PerInfo), i, fp);
			fclose(fp);
			for(p=0;p<i;p++){
				if(evo[p].id!=id) sen[o++]=evo[p];
			}
			fp = fopen("data\\perinfo.txt", "w");
			fwrite(sen, sizeof(PerInfo), o, fp);
			free(evo);
			free(sen);
		}
		fclose(fp);
		sys_dec(0);//减少人数记录
	}else{
		fclose(fp);
		return 3;//成员数0，删除失败
	}
	return 0;
}
int Person_change(PerInfo msg,int type){
	if(Person_had(msg.id)==0)return 7;//不存在用户
	int i,o;
	switch(type){
		case 1:	
			i=Char_isok(msg.name);
			if(i)return 1;
			break;
		case 3:
			if(msg.gender<0 || msg.gender>1) return 3;
			break;
		case 4:
			i=Char_isok(msg.organization);
			if(i)return 4;
			break;
		case 5:
			i=Char_isok(msg.faculty);
			if(i)return 5;
			break;
		case 6:
			i=Char_isok(msg.password);
			if(i)return 6;
			break;
		default:
			return 8;
	}
	FILE * fp;
	fp = fopen("data\\perinfo.txt", "r");
	if(fp==NULL) return 9;
    i=sys_info.peo;
	PerInfo* evo=(PerInfo *) malloc(i*sizeof(PerInfo));
    fread(evo, sizeof(PerInfo), i, fp);
    fclose(fp);
    for(o = 0; o < i; o++){
		if(evo[o].id==msg.id){
			//puts(evo[o].name);
			switch(type){
				case 1:	
					strcpy(evo[o].name,msg.name);
					break;
				case 3:
					evo[o].gender=msg.gender;
					break;
				case 4:
					strcpy(evo[o].organization,msg.organization);
					break;
				case 5:
					strcpy(evo[o].faculty,msg.faculty);
					break;
				case 6:
					strcpy(evo[o].password,msg.password);
					//puts(evo[o].password);
					break;
			}
			break;
		}
	}
	fp = fopen("data\\perinfo.txt", "w");
	fwrite(evo, sizeof(PerInfo), i, fp);
	fclose(fp);
	free(evo);
	return 0;
}
int Person_create(PerInfo msg){//注意注册ID不要超过21亿
	//开始检查录入
	int i,o=1;
	i=Char_isok(msg.name);
	if(i)return 1;//检查名字
	if(msg.id<=10000) return 2;//检查ID
	if(msg.gender<0 || msg.gender>1) return 3;//检查性别
	i=Char_isok(msg.organization);
	if(i)return 4;//学院书院
	for(i=0;i<20;i++){
		if(strcmp(sys_info.yuan[i],msg.organization)==0)
			o=0;
	}
	if(o)return 4;//学院书院
	i=Char_isok(msg.faculty);
	if(i)return 5;//系
	for(i=0;i<20;i++){
		if(strcmp(sys_info.xi[i],msg.faculty)==0)
			o=0;
	}
	if(o)return 5;
	i=Char_isok(msg.password);
	if(i)return 6;//密码
	//检查重复
	if(Person_had(msg.id))return 7;//重复录入
	//检查完毕，进行录入
	FILE *fp;
	PerInfo *evo;
	fp = fopen("data\\perinfo.txt", "r+");
	if(sys_info.peo){//有1个或以上
		i=sys_info.peo;
		evo=(PerInfo *) malloc((i+1)*sizeof(PerInfo));//动态声明数组
		fread(evo, sizeof(PerInfo), i, fp);
		fclose(fp);
		evo[i++]=msg;
		fp = fopen("data\\perinfo.txt", "w");
		fwrite(evo, sizeof(PerInfo), i, fp);
		free(evo);
	}else{
		evo=&msg;
		fwrite(evo, sizeof(PerInfo), 1, fp);
	}
	fclose(fp);
	
	sys_inc(0);//增加人数记录
	return 0;
}


void Person_mynotice(){//获取成员通知例子，请注意有没有满20条
	Notice msn[20];
	Person_getnotice(10000,msn);
	for(int i=0;i<20;i++){
		if(msn[i].time>0){
			time_t  tt = (time_t)msn[i].time;   //假定时间
			char now[64];
			struct tm *ttime;
			ttime = localtime(&tt);
			strftime(now, 64, "%Y%m%d %H:%M:%S", ttime);
			printf("\t\t\t\t公告>%d年%d月%d日  %02d:%02d —— %s\n",ttime->tm_year + 1900, ttime->tm_mon + 1, ttime->tm_mday, ttime->tm_hour, ttime->tm_min,msn[i].msg);
		}
	}
	
}
void Person_list(){//获取所有成员例子
	if(sys_info.peo==0) return;
	PerInfo* evo=(PerInfo *) malloc((sys_info.peo)*sizeof(PerInfo));
	Person_getlist(evo);
    for(int o = 0; o < sys_info.peo; o++){
		printf("> %d_%s_%d_%s_%s_%s\n",evo[o].id,evo[o].name,evo[o].gender,evo[o].organization,evo[o].faculty,evo[o].password);
	}
	free(evo);
}