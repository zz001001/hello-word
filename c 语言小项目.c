#include<windows.h>
#include<stdio.h>
#include<time.h>
#include"resource.h"

typedef struct NODE
{
	int x;
	int y;
	struct NODE *pnext;
	struct NODE *plast;

}snack,Apple;

enum DRECTION {UP,DOWN,LEFT,RIGHT};

enum DRECTION drection=UP;

snack *pHead;
snack *pEnd;
Apple apple={5,5,NULL,NULL};
BOOL bflag=TRUE;

//创建位图
HBITMAP h_Hbitmap_background;
HBITMAP h_Hbitmap_Apple;
HBITMAP h_Hbitmap_snackbody;
HBITMAP h_Hbitmap_snackhead_right;
HBITMAP h_Hbitmap_snackhead_lift;
HBITMAP h_Hbitmap_snackhead_up;
HBITMAP h_Hbitmap_snackhead_down;

void Showbackground(HDC hdc);

void AddNode(int x,int y);

void ShowSnack(HDC hdc);

void Move();

void ShowApple(HDC hdc);

BOOL IsEatApple();

void NewApple();

BOOL IsBumpWall();

BOOL IsEatItself();

LRESULT CALLBACK MyWNDPROC (HWND hwin, UINT massage, WPARAM wparam, LPARAM lparam); //处理消息函数
                        //    窗口句柄     无符号整型(消息类型  每个数字对应一个消息)    64位无符号整型（一般是来源的ID号消息）    64位长整型（是通过消息传输的数据）

int CALLBACK WinMain(HINSTANCE hinstance, //当前应用程序实例的句柄 ，句柄能够储存数据，并且找到这块储存空间（应用程序实例），但不能修改它
					 HINSTANCE hprevinstance, //应用程序的前一个实例句柄(win32程序不使用，设为NULL)
					 LPSTR lpcmpline, //命令行参数
					 int ncmpshow     //这个窗体显示的方式
                     )
{
	WNDCLASSEX ex;                  //包含窗口信息的结构体
	HWND hwin;                      //窗口句柄
	MSG msg;                        //储存消息的结构体
	

	//1.设计窗口
	ex.style=CS_HREDRAW|CS_VREDRAW; //水平刷新或垂直刷新
	ex.cbSize=sizeof(ex);           //当前结构体的大小
	ex.cbClsExtra=0;                //为结构体开辟附加内存空间,或附加信息，通常为0
	ex.cbWndExtra=0;                //在注册窗口时申请多大的内存空间，这段空间用来存放此窗口特有的数据，通常为0
	ex.hInstance=hinstance;         //应用程序句柄，此结构体在那个应用程序中使用
	ex.hIcon=NULL;                  //指定窗口类的图标句柄
	ex.hCursor=NULL;                //加载鼠标
	//ex.hbrBackground=CreateSolidBrush(RGB(255,0,128));   //指定背景画刷句柄
	ex.hbrBackground= (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	ex.hIconSm=NULL;                 //窗体图标,最左上角的小图标
	ex.lpfnWndProc=&MyWNDPROC;       //消息处理函数指针，窗口从消息函数中接收消息
	ex.lpszMenuName=NULL;            //加载菜单名
	ex.lpszClassName="aaa";          //指定图纸的名字
	 
	//2.注册窗口
	RegisterClassEx(&ex);           //参数为设计窗口结构体的地址

	//3.创建窗口
	hwin=CreateWindow(ex.lpszClassName,"贪吃蛇",WS_OVERLAPPEDWINDOW,50,50,615,638,NULL,NULL,hinstance,NULL);
	                //   图纸名           窗口名      窗体的样式        左上x  左上y 宽  高  父窗体的句柄 菜单句柄 窗口所属应用程序句柄  默认为NULL多文档时赋值
	                //返回值是一个窗口句柄
	//4.显示窗口
	ShowWindow(hwin,SW_SHOW);
	        //  窗口句柄  指定窗口显示状态
	//将图片加载到内存中

	h_Hbitmap_background=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP1));
	//  返回值是位图句柄                         程序实例句柄    加载位图名称字符串
	                                           //将整数性转换成包含在资源文件中的字符串  参数为路径的宏定义
	h_Hbitmap_Apple=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP2));
	h_Hbitmap_snackbody=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP3));
	h_Hbitmap_snackhead_right=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP4));
	h_Hbitmap_snackhead_lift=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP6));
	h_Hbitmap_snackhead_up=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP5));
	h_Hbitmap_snackhead_down=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP7));

	//初始化蛇身
	AddNode(3,3);
	AddNode(3,4);
	AddNode(3,5);

	srand((unsigned int )time(NULL));

	//定时器 创建在窗口创建之后  消息循环之前

	SetTimer(hwin,1,300,NULL); //定时器函数，将消息函数中的WM_TIMER消息发送给窗口
	//      窗口句柄  定时器ID  向窗口发送消息的时间间隔（以ms为单位） 为NULL则发送消息
	
	//消息循环
	while(GetMessage(&msg,NULL,0,0))  //GetMessage返回值为0时，则退出消息获取
	{
		//指向一个MSG的指针  指定接收哪个窗口的消息(如果为空则接收所有的窗口)  指定要获取消息的最小值,通常为0   指定要获取消息的最大值，如果最大和最小都是0就接收所有

		TranslateMessage(&msg); //翻译消息
		DispatchMessage(&msg);  //分配消息
	}
	return 0;
}

LRESULT CALLBACK MyWNDPROC (HWND hwin, UINT massage, WPARAM wparam, LPARAM lparam)
{
	
	HDC hdc;         //设备环境句柄
	HDC hdcman;
	PAINTSTRUCT ps;  //绘图所需的结构体
	HBRUSH hbrush;   //刷子句柄

	switch(massage)
	{
	case WM_CLOSE:  //关闭消息
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:   //按键消息
		if(bflag)
		{
			if(wparam==VK_UP && drection!=DOWN)
			{
				drection=UP;
			}
			if(wparam==VK_DOWN && drection!=UP)
			{
				drection=DOWN;
			}
			if(wparam==VK_LEFT && drection!=RIGHT)
			{
				drection=LEFT;
			}
			if(wparam==VK_RIGHT && drection!=LEFT)
			{
				drection=RIGHT;
			}

			bflag=FALSE;

			hdc=GetDC(hwin);
			Showbackground(hdc);
			ShowSnack(hdc);
			//显示苹果
			ShowApple(hdc);
			ReleaseDC(hwin,hdc);
		}

		break;
	
	case WM_PAINT:    //重绘消息（窗口出现时就有，所以重绘消息显示的图形不会消失）  只有重绘消息用beginpaint获得设备环境句柄
		hdc=BeginPaint(hwin,&ps);
		//显示背景
		Showbackground(hdc);
		//显示蛇身
		ShowSnack(hdc);
		//显示苹果
		ShowApple(hdc);

		EndPaint(hwin,&ps);

		break;

	case WM_TIMER:
		hdc=GetDC(hwin);
		//蛇移动
		Move();

		if(IsBumpWall()||IsEatItself())
		{
			KillTimer(hwin,1);
			MessageBox(hwin,"GOVE OVER","提示",MB_OK);
		}

		if(IsEatApple())
		{
			//长个
			AddNode(-10,-10); //添加到显示区域外面   随着蛇的移动它自动添加到上一个节点

			NewApple();
		}



		//显示背景
		Showbackground(hdc);
		//显示蛇身
		ShowSnack(hdc);
		//显示苹果
		ShowApple(hdc);
		ReleaseDC(hwin,hdc);

		bflag=TRUE;

		break;

		
		
	}

	return DefWindowProc(hwin,massage,wparam,lparam); //要将消息返回
}

void Showbackground(HDC hdc)
{
	HDC hdcman;
	//1.创建兼容性HDC
	hdcman=CreateCompatibleDC(hdc);
	//2.给兼容性HDC选择一张图片
	SelectObject(hdcman,h_Hbitmap_background);
	//3.传输  从兼容性HDC向窗口HDC传输
	BitBlt(hdc,0,0,600,600,hdcman,0,0,SRCCOPY);
	//  窗口句柄 传输的初始坐标  传输的终点坐标（图片的大小）  兼容性句柄  从兼容性HDC的传输起始点  传输的形式
	//4.删除兼容性HDC
	DeleteDC(hdcman);

	return;
}

void AddNode(int x,int y)
{
	snack *pNode=(snack *)malloc(sizeof(snack));
	pNode->x=x;
	pNode->y=y;
	pNode->pnext=NULL;
	pNode->plast=NULL;

	if(pHead==NULL)
	{
		pHead=pNode;
	}
	else
	{
		pEnd->pnext=pNode;
		pNode->plast=pEnd;
	}
	pEnd=pNode;

	return;
}

void ShowSnack(HDC hdc)
{
	snack *pmark=pHead->pnext;
	HDC hdcmen=CreateCompatibleDC(hdc);
	//1.显示蛇头
	switch(drection)
	{

	case UP:
		SelectObject(hdcmen,h_Hbitmap_snackhead_up);
		break;
	case DOWN:
		SelectObject(hdcmen,h_Hbitmap_snackhead_down);
		break;
	case LEFT:
		SelectObject(hdcmen,h_Hbitmap_snackhead_lift);
		break;
	case RIGHT:
		SelectObject(hdcmen,h_Hbitmap_snackhead_right);
		break;
   
	}
	BitBlt(hdc,pHead->x*30,pHead->y*30,30,30,hdcmen,0,0,SRCCOPY);

	//2.显示蛇身

	SelectObject(hdcmen,h_Hbitmap_snackbody);
	while(pmark!=NULL)
	{

		BitBlt(hdc,pmark->x*30,pmark->y*30,30,30,hdcmen,0,0,SRCCOPY);

		pmark=pmark->pnext;
	}
	DeleteDC(hdcmen);

	return;
	

}

void Move()
{
	
	snack *pmark=pEnd;
	//1.显示蛇身
	while(pmark->plast!=NULL)
	{
		pmark->x=pmark->plast->x;
		pmark->y=pmark->plast->y;

		pmark=pmark->plast;
	}

	//2.显示蛇头
	switch(drection)
	{
	case UP:
		pmark->y--;
		break;
	case DOWN:
		pmark->y++;
		break;
	case LEFT:
		pmark->x--;
		break;
	case RIGHT:
		pmark->x++;
		break;
	}
	return;
}

void ShowApple(HDC hdc)
{
	HDC hdcmen=CreateCompatibleDC(hdc);
	SelectObject(hdcmen,h_Hbitmap_Apple);
	BitBlt(hdc,apple.x*30,apple.y*30,30,30,hdcmen,0,0,SRCCOPY);
	DeleteDC(hdcmen);

	return;

}

BOOL IsEatApple()
{
	if(pHead->x==apple.x && pHead->y==apple.y)
	{
		return TRUE;
	}

	return FALSE;

}

void NewApple()
{
	int x;
	int y;
	snack *pmark=pHead;
	//1.随机生成一个苹果1-18
	do
	{
		x=rand()%18+1;
		y=rand()%18+1;

		pmark=pHead;  //每次将蛇头赋给pmark
		while(pmark!=NULL)  //遍历蛇头到蛇身
		{
			if(x==pmark->x && y==pmark->y) //如果新生成的坐标与蛇身重合  跳出循环重新生成
			{
			    break;
			}
			
			pmark=pmark->pnext;
		}
		
	}while(pmark!=NULL);

	apple.x=x;
	apple.y=y;

	return;
}

BOOL IsBumpWall()
{
	if(pHead->x==0||pHead->x==19||pHead->y==0||pHead->y==19)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL IsEatItself() //判断是否自咬
{
	snack *pmark=pHead->pnext;
	while(pmark!=NULL)
	{
		if(pHead->x==pmark->x && pHead->y==pmark->y)
		{

			return TRUE;
		}

		pmark=pmark->pnext;
	}
	


	return FALSE;
}
