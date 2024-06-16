#ifndef USER_H
#define USER_H

#define MAX_USER_NUMBER 100 // 最大用户数

#include <stdio.h>
#include <stdlib.h>
#include "../sqlite/sqlite3.h"

typedef struct User { // 用户结构体
    char name[20];
    char password[20];
} User;

/* 全局变量 */
extern int user_number; // 用户数量
extern User user[MAX_USER_NUMBER]; // 用户结构体

sqlite3 *Create_database_user(); // 创建用户数据库
int Query_user(sqlite3 *db_user, char *user_name); // 查询用户是否存在
int Query_password(sqlite3 *db_user, char *user_name, char *user_password); // 查询用户密码
void Init_user(sqlite3 *db_user); // 初始化用户
void Get_user_imformation_from_database(sqlite3 *db_user); // 更新数据库用户信息
void Add_user(sqlite3 *db_user, char *user_name, char *user_password); // 添加用户
void Delete_user(sqlite3 *db_user, char *user_name); // 删除用户
void Modify_password(sqlite3 *db_user, char *user_name, char *new_password); // 修改密码

#endif // __USER_H__
