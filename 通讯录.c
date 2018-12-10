#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


typedef struct NODE
{
	int id;
	char *name;
	char *tel;
	struct NODE *pnext;

}Node;

typedef struct PAGE
{
	int current_page;
	int total_page;
	int total_info;
	int onepage_info;

}page;

int g_menu_key;
char g_key;
void AddNode(Node **pphead,Node **ppend,Node *pNode);

Node *GetNode();

int GetId();

char *GetName();

char *GetTel();

void Initfo(Node **pphead,Node **ppend,int n);

page *GetPage(Node *phead,int n);

void TurnPage(Node *phead,page *Ppage);

void ShowInfo(Node *phead,page *Ppage);

void ShowMenu(page *Ppage);

char GetKey();

void Browse(Node *phead);

Node *GetNodeIn();

char *Getstring();

void Query(Node *phead);

void DeleteNode(Node **pphead,Node **ppend,int id);

void DeleteInfo(Node *phead,Node *pend);

void UpdateInfo(Node *phead);

int main()
{
	Node *phead=NULL;
	Node *pend=NULL;
	char c;

	Initfo(&phead,&pend,100);

	while(1)
	{
		printf("1.查看通讯录\n");
		printf("2.添加信息\n");
		printf("3.查询信息\n");
		printf("4.删除信息\n");
		printf("5.修改信息\n");
		printf("q.退出\n");

		c=GetKey();

		switch (c)
		{
		case '1':
			g_menu_key=1;
			Browse(phead);
			break;

		case '2':
			AddNode(&phead,&pend,GetNodeIn());
			break;

		case '3':
			g_menu_key=3;
			Query(phead);
			break;

		case '4':
			g_menu_key=4;
			DeleteInfo(phead,pend);
			break;

		case '5':
			g_menu_key=5;
			UpdateInfo(phead);
			break;

		case 'q':
			return 0;
			break;
		default:
			break;
		}

	}

	return 0;
}

void AddNode(Node **pphead,Node **ppend,Node *pNode)
{
	if(*pphead==NULL)
	{
		*pphead=pNode;

	}
	else
	{
		(*ppend)->pnext=pNode;
	}
	*ppend=pNode;

	return;
}

Node *GetNode()
{
	Node *pNode=(Node *)malloc(sizeof(Node));
	pNode->id=GetId();
	pNode->name=GetName();
	pNode->tel=GetTel();
	pNode->pnext=NULL;

	return pNode;
		
}

int GetId()
{
	static int n=1;

	return n++;
}

char *GetName()
{
	int i;
	char *pName=(char *)malloc(sizeof(char)*7);
	for(i=0;i<6;i++)
	{
		pName[i]=rand()%26+'a';
	}
	pName[6]='\0';

	return pName;
}

char *GetTel()
{
	int i;
	char *pTel=(char *)malloc(sizeof(char)*12);

	switch(rand()%4)
	{
	case 0:
		strcpy_s(pTel,12,"133");
		break;
	case 1:
		strcpy_s(pTel,12,"155");
		break;
	case 2:
		strcpy_s(pTel,12,"177");
		break;
	case 3:
		strcpy_s(pTel,12,"188");
		break;
	}

	for(i=3;i<11;i++)
	{
		pTel[i]=rand()%10+'0';
	}

	pTel[11]='\0';

	return pTel;
}

void Initfo(Node **pphead,Node **ppend,int n)
{
	int i;
	srand((unsigned int)time(NULL));
    
	for(i=0;i<n;i++)
	{
		AddNode(pphead,ppend,GetNode());
	}

	return;
}

page *GetPage(Node *phead,int n)
{
	page *Ppage=(page *)malloc(sizeof(page));
	Ppage->current_page=0;
	Ppage->onepage_info=n;
	Ppage->total_info=0;

	while(phead!=NULL)
	{
		Ppage->total_info++;
		phead=phead->pnext;
	}

	if(Ppage->total_info % Ppage->onepage_info==0)
	{
		Ppage->total_page=Ppage->total_info / Ppage->onepage_info;
	}
	else
	{
		Ppage->total_page=Ppage->total_info / Ppage->onepage_info+1;
	}

	return Ppage;
}

void TurnPage(Node *phead,page *Ppage)
{
	char c='s';
	while(1)
	{
		switch(c)
		{
		case 's':
			if(Ppage->current_page < Ppage->total_page)
			{
				Ppage->current_page++;
				ShowInfo(phead,Ppage);
				ShowMenu(Ppage);
			}
			else
			{
				printf("当前已经是最后一页啦\n");
			}
			break;
		case 'w':
			if(Ppage->current_page>1)
			{
				Ppage->current_page--;
				ShowInfo(phead,Ppage);
				ShowMenu(Ppage);
			}
			else
			{
				printf("当前已经是第一页啦\n");
			}
			break;
		case 'b':
			return;
			break;
		case 'c':
			return;
			break;
		case 'd':
			return;
			break;
		case 'u':
			return;
			break;
		default:
			printf("按错啦\n");
			break;
		}
		c=GetKey();
		g_key=c;
	}
	return;
}

void ShowInfo(Node *phead,page *Ppage)
{
	int begin =(Ppage->current_page-1)*Ppage->onepage_info+1;
	int end=Ppage->current_page*Ppage->onepage_info;
	int count=0;

	while(phead!=NULL)
	{
		count++;
		if(count<=end && count>=begin)
		{
			printf("%d %s %s\n",phead->id,phead->name,phead->tel);
		}
		phead=phead->pnext;
	}

	return;
}

void ShowMenu(page *Ppage)
{
	switch(g_menu_key)
	{

	case 1:
		printf("共有%d条    共有%d页    当前是第%d页      s下一页   w上一页  b返回\n",Ppage->total_info,Ppage->total_page,Ppage->current_page);
		break;
	case 3:
		printf("共有%d条    共有%d页    当前是第%d页      s下一页   w上一页  c重新查询   b返回\n",Ppage->total_info,Ppage->total_page,Ppage->current_page);
		break;
	case 4:
		printf("共有%d条    共有%d页    当前是第%d页      s下一页   w上一页  d删除信息   b返回\n",Ppage->total_info,Ppage->total_page,Ppage->current_page);
		break;
	case 5:
		printf("共有%d条    共有%d页    当前是第%d页      s下一页   w上一页  u修改信息   b返回\n",Ppage->total_info,Ppage->total_page,Ppage->current_page);
		break;
	}
	return;
}

char GetKey()
{
	char c;
	char z;
	int flag=1;

	while((c=getchar())!='\n' ||flag==1)
	{
		z=c;
		flag=0;
	}

	return z;
}

void Browse(Node *phead)
{
	page *Ppage=GetPage(phead,10);
	TurnPage(phead,Ppage);
	free(Ppage);
	Ppage=NULL;

	return;
}

Node *GetNodeIn()
{
	Node *pNode=(Node *)malloc(sizeof(Node));
	pNode->id=GetId();
	printf("请输入要添加的姓名：\n");
	pNode->name=Getstring();
	printf("请输入要添加的电话：\n");
	pNode->tel=Getstring();
	pNode->pnext=NULL;

	return pNode;

}

char *Getstring()
{
	char c;
	int size=5;
	int count=0;
	char *str=(char *)malloc(size);
	char *pmark=str;
	char *newstr=NULL;

	while((c=getchar())!='\n')
	{
		count++;
		*str=c;
		str++;
		if(count+1==size)
		{
			*str='\0';
			size+=5;
			newstr=(char *)malloc(size);
			strcpy_s(newstr,size,pmark);
			free(pmark);
			pmark=newstr;
			str=newstr+count;
		}


	}
	*str='\0';

	return pmark;
}

void Query(Node *phead)
{
	char c;
	char *str=NULL;
	Node *newphead=NULL;
	Node *newpend=NULL;
    Node *pmark=phead;
	Node *del=NULL;

	while(1)
	{
		while(1)
		{
			printf("请输入关键字:\n");
			str=Getstring();
			printf("是否确认？  a确认  其它键返回\n");
			if(GetKey()!='a')
			{
				free(str);
				str=NULL;

			}
			else
			{
				break;
			}
		}

		phead=pmark;
		while(phead!=NULL)
		{
			if(strncmp(str,phead->name,strlen(str))==0||strncmp(str,phead->tel,strlen(str))==0)
			{
				Node *pNode=(Node *)malloc(sizeof(Node));
				pNode->id=phead->id;
				pNode->name=phead->name;
				pNode->tel=phead->tel;
				pNode->pnext=NULL;

				AddNode(&newphead,&newpend,pNode);
			}
			phead=phead->pnext;
		}

		Browse(newphead);

		while(newphead!=NULL)
		{
			del=newphead;
			newphead=newphead->pnext;
			free(del);
			del=NULL;
		}
		newpend=NULL;

		if(g_key=='b'||g_key=='d'||g_key=='u')
		{
			break;
		}
	}
	return;
}

void DeleteNode(Node **pphead,Node **ppend,int id)
{
	Node *pmark=*pphead;
	Node *del=NULL;
	if(id==(*pphead)->id)
	{
		del=*pphead;
		*pphead=(*pphead)->pnext;
		free(del);
		del=NULL;
	}

	while(pmark->pnext!=NULL)
	{
		if(pmark->pnext->id==id)
		{
			del=pmark->pnext;
			pmark->pnext=pmark->pnext->pnext;
			free(del);
			del=NULL;

			if(pmark->pnext==NULL)
			{
				*ppend=pmark;
			}
		}
		pmark=pmark->pnext;
	}

	return;

}

void DeleteInfo(Node *phead,Node *pend)
{

	char *pid;
	int id;

	while(1)
	{
		Query(phead);

		if(g_key=='b')
		{
			break;
		}

		printf("请输入要删除的编号:\n");
		pid=Getstring();
		id=atoi(pid);
		free(pid);
		pid=NULL;

		DeleteNode(&phead,&pend,id);
		
		printf("是否继续删除？  y继续删除  其它键返回\n");
		if(GetKey()!='y')
		{
			break;
		}
	}

	return;
}

void UpdateInfo(Node *phead)
{
	char *pid;
	int id;

	Query(phead);

	printf("请输入要修改的编号:\n");
	pid=Getstring();
	id=atoi(pid);
	free(pid);
	pid=NULL;

	while(phead!=NULL)
	{
		
		if(id==phead->id)
		{
			printf("请输入要修改的姓名:\n");
			free(phead->name);
			phead->name=Getstring();
			printf("请输入要修改的电话:\n");
			free(phead->tel);
			phead->tel=Getstring();
		}
		phead=phead->pnext;
	}

	return;
}