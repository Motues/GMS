#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sqlite/sqlite3.h"
#include "../include/student.h"
#include "../include/user.h"
#include "../include/tools.h"
#include "../include/window.h"


sqlite3 *db_stu, *db_user; // 数据库
char current_user[20]; // 当前登录的用户



void Init() {
    Set_size();
    Set_title();
    db_stu = Create_database_student(); // 创建学生数据库
    Create_table_student(db_stu); // 创建学生表
    Init_student(db_stu); // 初始化十个学生内容
    Get_student_imformation_from_database(db_stu); //从数据库读取学生数据，存储到Stu结构体中
    db_user = Create_database_user(); // 创建用户数据库
    Init_user(db_user); // 初始化管理员和十个用户内容
    Get_user_imformation_from_database(db_user); //从数据库读取用户数据，存储到User结构体中
    Update_course(db_stu);
    Sleep(1500);
    // for (int i = 0; i<user_number; i++) {
    //     printf("%-15s%s\n",user[i].name, user[i].password);
    // }
    // getchar();
}

int main() {
    Init(); // 初始化
    Wellcome(); // 显示欢迎信息
    int f;
    Loginin:
    f = Login(db_user, current_user); // 登录，0表示登录成功，-1表示退出系统
    if (f == 0) {
        if (current_user[0] == 'U') {
            Student_menu(db_user, db_stu, current_user); //学生
        }else {
            Admin_menu(db_user, db_stu, current_user); // 管理员
        }
        goto Loginin;
    }
    Goto_xy(0,12);
    sqlite3_close(db_stu); // 关闭数据库连接
    sqlite3_close(db_user);
    return 0;
}
