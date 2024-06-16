#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <windows.h>
#include "../sqlite/sqlite3.h"


//设置颜色对应数字
#define transparent 0
#define deep_blue 1
#define deep_green 2
#define red 4
#define deep_purple 5
#define golden 6
#define white 7
#define grey 8
#define light_green 10
#define light_blue 11
#define purple 13
#define flesh 14
#define light_white 15

void Set_color(int color); //设置颜色
void Set_title(); //设置窗口标题
void Set_size(); //设置窗口大小
void Goto_xy(int x, int y); //移动光标

void Wellcome(); //欢迎界面
int Login(sqlite3 *db_user, char *current_user); //登录界面 
void Student_menu(sqlite3 *db_user, sqlite3 *db_stu, char *current_user); //用户菜单
void Admin_menu(sqlite3 *db_user, sqlite3 *db_stu, char *current_user); //管理员菜单




#endif // __WINDOW_H__
