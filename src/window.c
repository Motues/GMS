#include "../include/window.h"
#include "../include/student.h"
#include "../include/user.h"
#include "../include/tools.h"


//设置输出颜色
void Set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/**
 * 设置窗口大小
 * width: 窗口宽度
 * height: 窗口高度
*/
void Set_size() {
    int width = 100, height = 30;
    char cmd[128];
    sprintf(cmd, "mode con cols=%d lines=%d", width, height);
    system(cmd);
    SetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE, GetWindowLongPtrA(GetConsoleWindow(),GWL_STYLE)& ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
}
//设置窗口标题
void Set_title() {
    SetConsoleTitle("GMS");
}
/**
 * 移动光标到指定位置
 * @param x 横坐标
 * @param y 纵坐标
*/
void Goto_xy(int x, int y) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // 获取控制台句柄
    // if (hOut == INVALID_HANDLE_VALUE) {
    //     printf("--> Failed to get cursor position! <--\n");
    //     return;
    // }
    // printf("--> Get cursor position Successfully! <--\n");
    COORD pos = {x, y}; // 设置光标位置
    int f = SetConsoleCursorPosition(hOut, pos); // 移动光标
    // if (!f) {
    //     printf("--> Failed to set cursor position! <--\n");
    //     DWORD error = GetLastError();
    //     printf("--> Error code: %d <--", error);
    //     return;
    // }
	// printf("--> Set cursor position Successfully! <--\n");
}
/**
 * 模拟密码输入
 * @param passward 密码字符串对应指针
*/
void Input_password(char *passward) {
    char ch = 0;
    int i = 0;
    while(1) {
        ch = getch();
        if (ch == '\r') { // 回车
            passward[i] = '\0';
            printf("\n");
            break;
        }
        if (ch == '\b') { // 退格
            if (i > 0) {
                i--;
                printf("\b \b");
            }
            continue;
        }
        printf("*");
        passward[i++] = ch;
    }
}
/**
 * 模拟字符串输入
 * @param input 字符串对应指针
*/
void Input(char *input) {
    char ch = 0;
    int i = 0;
    while(1) {
        ch = getch();
        if (ch == '\r') { // 回车
            input[i] = '\0';
            printf("\n");
            break;
        }
        if (ch == '\b') {// 退格
            if (i > 0) {
                i--;
                printf("\b \b");
            }
           continue;
        }
        printf("%c", ch);
        input[i++] = ch;
    }
}

//欢迎界面
void Wellcome() {
    system("cls");//清屏
    Set_color(light_blue);
	printf(
	"                                                               \n"
	"                         CCCC        MM   MM        SSSSS      \n"
	"                        C           M  M M  M      S           \n"
	"                        C   CC      M   M   M       SSSSS      \n"
	"                        C    C      M   M   M            S     \n"
	"                         CCCC       M       M       SSSSS      \n"
	"                                                      V 1.0    \n"
	);
	Set_color(golden);
	printf(
	"                                                                Made By Motues, Jack\n"
	);
	Set_color(white);
	printf("Please press any key to continue.\n");
    getchar();
}

//登录界面
int Login(sqlite3 *db_user, char *current_user) {
    char id[20], password[20];
    int input_error = 0;
    Login:
    system("cls");
    printf(
    "                                                                \n"
	"                      |------------------------------------------|\n"
    "                      |                                          |\n"
	"                      |                   Login                  |\n"
    "                      |                                          |\n"
	"                      |        Username:                         |\n"
	"                      |        Password:                         |\n"
    "                      |                                          |\n"
	"                      |        Press Enter to Login              |\n"
    );
    if(input_error) {
        printf("                      |            ");
        Set_color(red);
        printf("Invalid Input!");
        Set_color(white);
        printf("                |\n");
    }else {
        printf("                      |                                          |\n");
    }
    printf("                      |------------------------------------------|\n");
    Goto_xy(40, 5);
    Input(id);//输入用户名
    if(strcmp(id, "q") == 0) return -1;
    // gets(id);
    Goto_xy(40, 6);
    Input_password(password);//输入密码
    // gets(password);
    if (Query_user(db_user, id) == 0) { // 如果用户不存在，返回登录界面
        input_error = 1;
        goto Login;
    }
    if (Query_password(db_user, id, password) == 0) { // 如果密码错误，返回登录界面
        input_error = 1;
        goto Login;
    }
    strcpy(current_user, id); // 保存当前登录用户
    return 0;
}

//显示个人信息
void Show_information_student(char *current_user) {
    system("cls");
    char name[20], sex[10];
    int age;
    for(int i = 0; i < student_number; i++) {
        if(strcmp(current_user, stu[i].id) == 0) {
            strcpy(name, stu[i].name);
            strcpy(sex, stu[i].sex);
            age = stu[i].age;
            break;
        }
    }
    printf("Welcome ");
    Set_color(golden);
    printf("%s", name);
    Set_color(white);
    printf(" !\n\n");
    // 显示个人信息，包括姓名、性别、年龄
    printf(
    "Your imformations are shown below.\n"
    "----------------------------------------------------------------------------------------------------\n"
    );
    printf("Name:    %s\n", name);
    printf("Sex:     %s\n", sex);
    printf("Age:     %d\n", age);
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Press any key to continue.");
    getchar();

}
//修改性别
void Modify_gender_student(sqlite3 *db_stu, char *current_user) {
    system("cls");
    char name[20], sex[10];
    int age;
    for(int i = 0; i < student_number; i++) {
        if(strcmp(current_user, stu[i].id) == 0) {
            strcpy(name, stu[i].name);
            strcpy(sex, stu[i].sex);
            age = stu[i].age;
            break;
        }
    }
    printf("Welcome ");
    Set_color(golden);
    printf("%s", name);
    Set_color(white);
    printf(" !\n\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char new_sex[10];
    printf("Your current gender:%s\n", sex); //显示当前性别
    Input_gender:
    printf("Please enter a new gender( male or female):");
    Input(new_sex); //输入性别
    if(strcmp(new_sex, "q") == 0) return;
    if(strcmp(new_sex, "male") != 0 && strcmp(new_sex, "female") != 0) { //判断输入性别是否合法
        Set_color(red);
        printf("Invalid Input!\n");
        Set_color(white);
        goto Input_gender;
    }else {
        Modify_sex(db_stu, current_user, new_sex); //修改性别
        Get_student_imformation_from_database(db_stu); //更新学生数据库信息
    }
    printf("Press any key to continue.");
    getchar();
}
//修改年龄
void Modify_age_student(sqlite3 *db_stu, char *current_user) {
    system("cls");
    char name[20], sex[10];
    int age;
    for(int i = 0; i < student_number; i++) {
        if(strcmp(current_user, stu[i].id) == 0) {
            strcpy(name, stu[i].name);
            strcpy(sex, stu[i].sex);
            age = stu[i].age;
            break;
        }
    }
    printf("Welcome ");
    Set_color(golden);
    printf("%s", name);
    Set_color(white);
    printf(" !\n\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    int new_age;
    printf("Your current age:%d\n", age); //显示当前年龄
    Input_age:
    printf("Please enter a new age:");
    scanf("%d", &new_age); //输入新年龄
    getchar();
    if(new_age <= 0){
        Set_color(red);
        printf("Invalid Input!\n");
        Set_color(white);
        goto Input_age;
    }
    Modify_age(db_stu, current_user, new_age); //修改年龄
    Get_student_imformation_from_database(db_stu); //更新学生数据库信息
    printf("Press any key to continue.");
    getchar();
}

int f_password; //判断密码是否修改
//修改密码
void Modify_password_student(sqlite3 *db_user, char *current_user) {
    f_password = 0; //初始化
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char new_password1[20], new_password2[20], old_password[20];
    Input_password:
    printf("Please enter your old password:");
    Input_password(old_password);// 输入旧密码
    if(strcmp(old_password, "q") == 0) return;
    if(Query_password(db_user, current_user, old_password) != 1) { //如果密码错误，返回从新输入
        Set_color(red);
        printf("Invalid Input!\n");
        Set_color(white);
        goto Input_password;
    }else {
        Modify_password:
        printf("Please enter a new password:");
        Input_password(new_password1); // 输入新密码
        printf("Please enter the new password again:");
        Input_password(new_password2); // 再次输入新密码
        if(strcmp(new_password1, new_password2) != 0) { // 判断两次输入的密码是否一致
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            goto Modify_password;
        }else {
            Modify_password(db_user, current_user, new_password1); // 修改密码
            Get_user_imformation_from_database(db_user); // 更新用户信息
            f_password = 1;
        }
    }
    printf("Press any key to reload.");
    getchar();
}
//修改个人信息
void Modify_information_student(sqlite3 *db_stu, sqlite3 *db_user, char *current_user) {
    Modify_information:
    system("cls");
    char name[20], sex[10];
    int age;
    for(int i = 0; i < student_number; i++) {
        if(strcmp(current_user, stu[i].id) == 0) {
            strcpy(name, stu[i].name);
            strcpy(sex, stu[i].sex);
            age = stu[i].age;
            break;
        }
    }
    printf("Welcome ");
    Set_color(golden);
    printf("%s", name);
    Set_color(white);
    printf(" !\n\n");
    /* 显示菜单*/
    printf(
    "                                                                \n"
	"                      |------------------------------------------|\n"
    "                      |                                          |\n"
	"                      |                   Menu                   |\n"
    "                      |                                          |\n"
    "                      |             1. Show information          |\n"
	"                      |             2. Modify Gender             |\n"
	"                      |             3. Modify Age                |\n"
    "                      |             4. Modify Password           |\n"
    "                      |             0. Return                    |\n"
	"                      |                                          |\n"
    "                      |------------------------------------------|\n"
    "                  Please enter the corresponding number and press enter.\n"
    "\n");
    printf("Your number( between 0 and 4 ) :");
    int opt;
    Modify_input:
    scanf("%d", &opt); // 输入操作
    getchar();
    switch (opt) {
        case 1:
            Show_information_student(current_user); // 显示学生信息
            break;
        case 2:
            Modify_gender_student(db_stu, current_user); // 修改学生性别
            break;
        case 3:
            Modify_age_student(db_stu, current_user); // 修改学生年龄
            break;
        case 4:
            Modify_password_student(db_user, current_user); // 修改学生密码
            if(f_password) return;
            break;
        case 0:
            return; // 返回
        default: // 非法输入
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            goto Modify_input;
            break;
    }
    goto Modify_information;
}

//打印单个学生信息
void Print_score(sqlite3 *db_stu, char *current_user, char *name) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", name);
    Set_color(white);
    printf(" !\n\n");
    printf(
    "Your scores are shown below.\n"
    "----------------------------------------------------------------------------------------------------\n"
    );
    for (int i = 0; i < student_number; i++) {
        if(strcmp(current_user, stu[i].id) == 0) {
            for (int j = 0; j < stu[i].course_number; j++) {
                if(strcmp(stu[i].course[j].state, "Failed") == 0) {
                    printf("%-10s",stu[i].course[j].name);
                    Set_color(red);
                    printf("%d\n", stu[i].course[j].score);
                    Set_color(white);
                }else if(strcmp(stu[i].course[j].state, "Deferred") == 0){
                    printf("%-10s",stu[i].course[j].name);
                    Set_color(light_blue);
                    printf("%d\n", stu[i].course[j].score);
                    Set_color(white);
                }else printf("%-10s%d\n",stu[i].course[j].name, stu[i].course[j].score);
            }
            printf("\nYour average score:%.2f\n", stu[i].average_score);
            break;
        }
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Press any key to continue.");
    getchar();
}
//学生菜单
void Student_menu(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    Student_menu:
    system("cls");
    char name[20];
    for(int i = 0; i < student_number; i++) {
        if(strcmp(current_user, stu[i].id) == 0) {
            strcpy(name, stu[i].name);
            break;
        }
    }
    printf("Welcome ");
    Set_color(golden);
    printf("%s", name);
    Set_color(white);
    printf(" !\n\n");
    /* 菜单 */
    printf(
    "                                                                \n"
	"                      |------------------------------------------|\n"
    "                      |                                          |\n"
	"                      |                   Menu                   |\n"
    "                      |                                          |\n"
	"                      |             1. Query Score               |\n"
	"                      |             2. Modify Information        |\n"
    "                      |             0. Logout                    |\n"
	"                      |                                          |\n"
    "                      |------------------------------------------|\n"
    "                  Please enter the corresponding number and press enter.\n"
    "\n");
    Student_input:
    printf("Your number( between 0 and 2 ) :");
    int opt;
    scanf("%d", &opt);
    getchar();
    switch (opt) {
        case 1:
            Print_score(db_stu, current_user, name); // 打印成绩
            break;
        case 2:
            Modify_information_student(db_stu, db_user, current_user); // 修改信息
            if(f_password){ // 如果修改密码，则退出从新登录
                f_password = 0;
                return;
            }
            break;
        case 0:
            return;// 退出
        default: // 无效输入
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            goto Student_input;
            break;
    }
    goto Student_menu;
}

//查询特定学生信息
void Query_specific_student(sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    char stu_id[20];
    Enter_id:
    printf("Please enter the student ID you want to query :");
    Input(stu_id);
    if(strcmp(stu_id, "q") == 0) return;
    if(Query_student(db_stu, stu_id) <= 0) {
        Set_color(red);
        printf("The student does not exist!\n");
        Set_color(white);
        goto Enter_id;
    }
    printf(
    "The scores are shown below.\n"
    "----------------------------------------------------------------------------------------------------\n"
    );
    for (int i = 0; i < student_number; i++) {
        if(strcmp(stu_id, stu[i].id) == 0) {
            for (int j = 0; j < stu[i].course_number; j++) {
                if(strcmp(stu[i].course[j].state, "Failed") == 0) {
                    printf("%-10s",stu[i].course[j].name);
                    Set_color(red);
                    printf("%d\n", stu[i].course[j].score);
                    Set_color(white);
                }else if(strcmp(stu[i].course[j].state, "Deferred") == 0){
                    printf("%-10s",stu[i].course[j].name);
                    Set_color(light_blue);
                    printf("%d\n", stu[i].course[j].score);
                    Set_color(white);
                }else printf("%-10s%d\n",stu[i].course[j].name, stu[i].course[j].score);
            }
            printf("\nThe average score:%.2f\n", stu[i].average_score);
            break;
        }
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Press any key to continue.");
    getchar();
}

//查询所有学生信息
void Query_all_students(sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    Get_student_imformation_from_database(db_stu);//更新学生数据库信息

    printf("If you want to sort, please input the corresponding number, otherwise for not sort!\n");
    printf("1. Average\n2. Id\n3. Name\n");
    for(int i = 0; i < all_course_number; i++){
        printf("%d. %s\n", i + 4, course_name[i]);
    }
    int opt;
    scanf("%d", &opt); //输入排序选项
    getchar();
    if(opt == 1) Sort_student_average(stu, student_number); //排序
    if(opt == 2) Sort_student_id(stu, student_number);
    if(opt == 3) Sort_student_name(stu, student_number);
    if(opt == 4) Sort_student_chinese(stu, student_number);
    if(opt == 5) Sort_student_math(stu, student_number);
    if(opt == 6) Sort_student_english(stu, student_number);

    /* 打印标题 */
    printf(
    "The students' scores are shown below.\n"
    "----------------------------------------------------------------------------------------------------\n"
    "Id          Name      "
    );
    for(int i = 0; i < all_course_number; i++) printf("%-10s", course_name[i]);
    printf("Average\n");
    /* 打印内容 */
    for(int i = 0; i < student_number; i++) {
        printf("%-12s%-10s", stu[i].id, stu[i].name);
        for(int j = 0; j < stu[i].course_number; j++) {
            if(strcmp(stu[i].course[j].state, "Failed") == 0) Set_color(red);
            if(strcmp(stu[i].course[j].state, "Deferred") == 0) Set_color(light_blue);
            printf("%-10d", stu[i].course[j].score);
            Set_color(white);
        }
        if(stu[i].average_score < 60) Set_color(red);
        printf("%-10.2f\n", stu[i].average_score);
        Set_color(white);
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Press any key to continue.");
    getchar();
    return;
}
//查询学生成绩
void Query_score_admin(sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    Get_student_imformation_from_database(db_stu);//更新学生数据库信息
    printf(
    "                                                                \n"
	"                      |------------------------------------------|\n"
    "                      |                                          |\n"
	"                      |                   Menu                   |\n"
    "                      |                                          |\n"
	"                      |             1. Specific student          |\n"
	"                      |             2. All students              |\n"
    "                      |             0. Logout                    |\n"
	"                      |                                          |\n"
    "                      |------------------------------------------|\n"
    "                  Please enter the corresponding number and press enter.\n"
    "\n");
    Query_score_input:
    printf("Your number( between 0 and 2 ) :");
    int option;
    scanf("%d", &option);
    getchar();
    switch (option) {
        case 1:
            Query_specific_student(db_stu, current_user);
            break;
        case 2:
            Query_all_students(db_stu, current_user);
            break;
        case 0:
            return;
        default: // 无效输入
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            goto Query_score_input;
            break;
    }
}


// 修改学生的某一门成绩
void Enter_score_direct(sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("Please enter the score as follows.\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char student_id_input[20];
    Enter_id:
    printf("Please input the student's id:");
    Input(student_id_input); // 输入学生id
    if(strcmp(student_id_input, "q") == 0) return;
    if(Query_student(db_stu, student_id_input) <= 0) { // 判断学生是否存在
        Set_color(red);
        printf("The student does not exist!\n");
        Set_color(white);
        goto Enter_id;
    }
    for (int i = 0; i < all_course_number; i++) { // 输出课程列表
        printf("[%s] ", course_name[i]);
    }
    char course_name_input[20];
    Enter_course:
    printf("\nPlease input the course name:");
    Input(course_name_input); // 输入需要修改的课程名
    if(Query_course(db_stu, course_name_input) == -1) { // 查询课程是否存在
        Set_color(red);
        printf("The course does not exist!\n");
        Set_color(white);
        goto Enter_course;
    }
    int score;
    Enter_score:
    printf("Please input the score:");
    scanf("%d", &score); // 输入需要修改的课程成绩
    // getchar();
    if(score > 100) { // 判断输入的成绩是否合法
        Set_color(red);
        printf("The score should be less than 100!\n");
        Set_color(white);
        goto Enter_score;
    }
    Modify_score(db_stu, student_id_input, course_name_input, score); // 修改成绩
    Get_student_imformation_from_database(db_stu); // 更新学生信息
    printf("Press any key to continue.");
    getchar();
}
//从文件批量读入学生成绩，不存在的学生自动创建
void Enter_score_file(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("Please enter the score as follows.\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Please fill in the content in the following format:\n"
           "U202311605 Ava female 18 100 82 93\n\n");
    printf("Please enter the file path(e.g. data/Students.txt):");
    char file_path[30];
    Input(file_path); // 输入文件路径
    if(strcmp(file_path, "q") == 0) return;
    Read_from_file(db_user, db_stu, file_path); // 从文件读取数据
    Get_student_imformation_from_database(db_stu); //更新学生数据库信息
    printf("Press any key to continue.");
    getchar();
}
//管理员修改成绩界面
void Enter_score_admin(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    /* 打印菜单*/
    printf(
    "                                                                \n"
	"                      |------------------------------------------|\n"
    "                      |                                          |\n"
	"                      |                   Menu                   |\n"
    "                      |                                          |\n"
	"                      |             1. Direct entry              |\n"
	"                      |             2. File entry                |\n"
    "                      |             0. Return                    |\n"
	"                      |                                          |\n"
    "                      |------------------------------------------|\n"
    "                  Please enter the corresponding number and press enter.\n"
    "\n");
    int opt;
    printf("Your number( between 0 and 2 ) :");
    scanf("%d", &opt);
    getchar();
    switch (opt) {
        case 1:
            Enter_score_direct(db_stu, current_user); // 直接修改某个同学的成绩
            break;
        case 2:
            Enter_score_file(db_user, db_stu, current_user); // 从文件批量录入
            break;
        case 0:
            return;// 返回
        default:
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            break;
    }
}

// 查询学生信息，包括ID，姓名、性别、年龄
void Query_student_admin(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf(
    "The students' information are shown below.\n"
    "----------------------------------------------------------------------------------------------------\n"
    "Id          Name      Gender    Age\n"
    );
    for(int i = 0; i < student_number; i++) {
        printf("%-12s%-10s%-10s%d\n", stu[i].id, stu[i].name, stu[i].sex, stu[i].age);
    }
    printf("----------------------------------------------------------------------------------------------------\n");

    printf("Press any key to continue.");
    getchar();
}

// 修改学生信息，包括姓名、性别、年龄
void Modify_student_admin(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    /* 显示学生信息 */
    printf(
    "The students' information are shown below.\n"
    "----------------------------------------------------------------------------------------------------\n"
    "Id          Name      Gender    Age\n"
    );
    for(int i = 0; i < student_number; i++) {
        printf("%-12s%-10s%-10s%d\n", stu[i].id, stu[i].name, stu[i].sex, stu[i].age);
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    char student_id_input[20];
    Input_id:
    printf("Please enter the student's id:"); // 输入学生id
    Input(student_id_input);
    if(strcmp(student_id_input, "q") == 0) return;
    if(Query_student(db_stu, student_id_input) <= 0) { // 判断学生是否存在
        Set_color(red);
        printf("The student does not exist!\n");
        Set_color(white);
        goto Input_id;
    }
    printf("[Name] [Gender] [Age]\n");
    char student_information_input[20];
    Input_information:
    printf("Please input the information you need to change:");
    Input(student_information_input); // 输入需要修改的信息
    if(strcmp(student_information_input, "Name") == 0) { // 修改姓名
        char new_name[20];
        printf("Please input the new name:");
        Input(new_name);
        Modify_name(db_stu, student_id_input, new_name);
        Get_student_imformation_from_database(db_stu);
    } else if(strcmp(student_information_input, "Gender") == 0) { // 修改性别
        char new_sex[20];
        printf("Please input the new gender:");
        Input(new_sex);
        Modify_sex(db_stu, student_id_input, new_sex);
        Get_student_imformation_from_database(db_stu);
    }else if(strcmp(student_information_input, "Age") == 0) { // 修改年龄
        int new_age;
        printf("Please input the new age:");
        scanf("%d", &new_age);
        getchar();
        Modify_age(db_stu, student_id_input, new_age);
        Get_student_imformation_from_database(db_stu);
    }else { // 无效输入
        Set_color(red);
        printf("Invalid Input!\n");
        Set_color(white);
        goto Input_information;
    }
    printf("Press any key to continue.");
    getchar();
}

// 添加学生的基本信息，包括ID，姓名，性别，年龄
void Add_student_admin(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("Please enter the student's informaton as follows.\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char student_name_input[20], student_id_input[20], student_sex_input[10];
    int student_age_input;
    Input_id:
    printf("Please input the student's id:");
    Input(student_id_input); // 输入学生id
    if(strcmp(student_id_input, "q") == 0) return;
    if(Query_student(db_stu, student_id_input) > 0) { // 判断学生是否存在
        Set_color(red);
        printf("The student already exists!\n");
        Set_color(white);
        goto Input_id;
    }
    printf("Please input the student's name:");
    Input(student_name_input); // 输入学生姓名
    printf("Please input the student's gender:");
    Input(student_sex_input); // 输入学生性别
    printf("Please input the student's age:");
    scanf("%d", &student_age_input); // 输入学生年龄
    getchar();
    Add_student(db_stu, student_id_input, student_name_input, student_sex_input, student_age_input); // 添加学生
    Add_user(db_user, student_id_input, "123456"); // 添加用户
    Get_student_imformation_from_database(db_stu); // 更新学生信息
    Get_user_imformation_from_database(db_user); // 更新用户信息
    printf("Press any key to continue.");
    getchar();
}

// 删除学生
void Delete_student_admin(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char student_id_input[20];
    Input_id:
    printf("Please enter the student's id:");
    Input(student_id_input); // 输入学生id
    if(strcmp(student_id_input, "q") == 0) return;
    if(Query_student(db_stu, student_id_input) <= 0) { // 判断学生是否存在
        Set_color(red);
        printf("The student does not exist!\n");
        Set_color(white);
        goto Input_id;
    }
    Delete_student(db_stu, student_id_input); // 删除学生
    Delete_user(db_user, student_id_input); // 删除用户
    Get_student_imformation_from_database(db_stu); // 更新学生信息
    Get_user_imformation_from_database(db_user); // 更新用户信息
    printf("Press any key to continue.");
    getchar();
}

// 保存数据到指定文件
void Save_to_file(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("Please enter the informaton as follows.\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char file_name[30];
    Input_file_name:
    printf("Please enter the file name:");
    Input(file_name); // 输入文件名
    if(strcmp(file_name, "q") == 0) return;
    FILE *fp = fopen(file_name, "w");
    if(fp == NULL) { // 文件不存在
        Set_color(red);
        printf("The file does not exist!\n");
        Set_color(white);
        goto Input_file_name;
    }
    /* 按照对应格式输出 */
    fprintf(fp, "Id          Name      Gender    Age       ");
    for(int i = 0; i < all_course_number; i++) fprintf(fp, "%-10s", course_name[i]);
    fprintf(fp, "Average\n");
    for(int i = 0;i < student_number; i++) {
        fprintf(fp, "%-12s %-10s %-10s %-10d", stu[i].id, stu[i].name, stu[i].sex, stu[i].age);
        for(int j = 0; j < stu[i].course_number; j++) {
            fprintf(fp, "%-10d", stu[i].course[j].score);
        }
        fprintf(fp, "%.2f\n", stu[i].average_score);
    }
    fclose(fp); // 关闭文件
    printf("The data has already save to %s.\n", file_name);
    printf("Press any key to continue.");
    getchar();
}

// 修改密码
void Modify_password_admin(sqlite3 *db_user, char *current_user) {
    f_password = 0; //初始化
    system("cls");
    printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    char new_password1[20], new_password2[20], old_password[20];
    Input_password:
    printf("Please enter your old password:");
    Input_password(old_password);// 输入旧密码
    if(strcmp(old_password, "q") == 0) return;
    if(Query_password(db_user, current_user, old_password) != 1) { //如果密码错误，返回从新输入
        Set_color(red);
        printf("Invalid Input!\n");
        Set_color(white);
        goto Input_password;
    }else {
        Modify_password:
        printf("Please enter a new password:");
        Input_password(new_password1); // 输入新密码
        printf("Please enter the new password again:");
        Input_password(new_password2); // 再次输入新密码
        if(strcmp(new_password1, new_password2) != 0) { // 判断两次输入的密码是否一致
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            goto Modify_password;
        }else {
            Modify_password(db_user, current_user, new_password1); // 修改密码
            Get_user_imformation_from_database(db_user); // 更新用户信息
            f_password = 1;
        }
    }
    printf("Press any key to reload.");
    getchar();
}
//管理员菜单
void Admin_menu(sqlite3 *db_user, sqlite3 *db_stu, char *current_user) {
    Admin_menu:
    f_password = 0;
    system("cls");
     printf("Welcome ");
    Set_color(golden);
    printf("%s", current_user);
    Set_color(white);
    printf(" !\n\n");
    printf(
    "                                                                \n"
	"                      |------------------------------------------|\n"
    "                      |                                          |\n"
	"                      |                   Menu                   |\n"
    "                      |                                          |\n"
    "                      |             1. Query score               |\n" // 查询成绩
    "                      |             2. Enter score               |\n" // 录入成绩，包括直接录入和文件录入
    "                      |             3. Query Student             |\n" // 查询学生信息
    "                      |             4. Modify Student            |\n" // 修改学生信息
    "                      |             5. Add Student               |\n" // 添加学生
    "                      |             6. Delete Student            |\n" // 删除学生
    "                      |             7. Save to File              |\n" // 保存到文件
    "                      |             8. Modify Password           |\n" // 修改密码
    // "                      |             9. Add Course                |\n"
    // "                      |             10. Delete Course             |\n"
    "                      |             0. Logout                    |\n" // 退出
	"                      |                                          |\n"
    "                      |------------------------------------------|\n"
    "                  Please enter the corresponding number and press enter.\n"
    "\n");
    printf("Your number( between 0 and 8 ) :");
    int opt;
    Modify_input:
    scanf("%d", &opt);
    getchar();
    switch (opt) {
        case 1:
            Query_score_admin(db_stu, current_user);
            break;
        case 2:
            Enter_score_admin(db_user, db_stu, current_user);
            break;
        case 3:
            Query_student_admin(db_user, db_stu, current_user);
            break;
        case 4:
            Modify_student_admin(db_user, db_stu, current_user);
            break;
        case 5:
            Add_student_admin(db_user, db_stu, current_user);
            break;
        case 6:
            Delete_student_admin(db_user, db_stu, current_user);
            break;
        case 7:
            Save_to_file(db_user, db_stu, current_user);
            break;
        case 8:
            Modify_password_admin(db_user, current_user);
            if(f_password == 1) {
                f_password = 0;
                return;
            }
            goto Admin_menu;
            break;
        case 0:
            return;
        default:
            Set_color(red);
            printf("Invalid Input!\n");
            Set_color(white);
            goto Modify_input;
            break;
    }
    goto Admin_menu;
}
