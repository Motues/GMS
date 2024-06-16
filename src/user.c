#include "../include/user.h"

int user_number = 0;
User user[MAX_USER_NUMBER];

/**
 * 创建并初始化用户数据库
 * 函数尝试打开名为"User.db"的SQLite数据库文件，并在其中创建一个名为"User"的表，
 * 如果表已存在，则不进行创建。如果数据库打开或表创建过程中发生错误，函数将关闭数据库连接，并返回NULL。成功时，返回打开的数据库指针。
 * @return 返回打开的数据库指针，或在发生错误时返回NULL。
 */
sqlite3 *Create_database_user()
{
    sqlite3 *db_user; //创建数据库指针
    int rc = sqlite3_open("data/User.db", &db_user); //尝试打开名为"User.db"的数据库文件
    if (rc != SQLITE_OK) { //发生错误
        printf("--> Can't open database: %s <--\n", sqlite3_errmsg(db_user));
        sqlite3_close(db_user);
        return 0;
    }
    printf("--> Opened database User successfully <--\n");
    char *sql = "CREATE TABLE IF NOT EXISTS User(Name varchar(20), Passward varchar(20));"; //创建SQL语句
    char *err_msg = NULL;
    rc = sqlite3_exec(db_user, sql, NULL, NULL, err_msg); //执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Table created successfully <--\n");
    }
    return db_user; //返回User数据库指针
}


/**
 * 查询数据库中指定用户名的数量。
 * @param db_user SQLite3数据库句柄
 * @param user_name 要查询的用户名
 * @return 返回用户名的数量。如果查询出错，返回-1；如果未找到任何匹配的用户名，返回0
 */
int Query_user(sqlite3 *db_user, char *user_name) {
    const char *sql = "SELECT COUNT(*) FROM User WHERE Name = ?"; //创建SQL语句
    sqlite3_stmt *stmt = NULL; //创建SQL指针
    int rc = sqlite3_prepare_v2(db_user, sql, -1, &stmt, NULL); //准备SQL语句
    if (rc != SQLITE_OK) { //发生错误
        printf("--> SQL error: Prepare failed <--\n");
        printf("--> SQL error: %s <--\n", sqlite3_errmsg(db_user));
        return -1;
    }
    rc = sqlite3_bind_text(stmt, 1, user_name, -1, SQLITE_TRANSIENT); //绑定参数
    if (rc != SQLITE_OK) { //发生错误
        printf("--> SQL error: Bind failed <--\n");
        sqlite3_finalize(stmt);
        return -1;
    }
    rc = sqlite3_step(stmt); //执行SQL语句
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);//释放资源
        return 0;
    }
    int count = sqlite3_column_int(stmt, 0); //获取结果
    sqlite3_finalize(stmt); //释放资源
    return count; //返回数量
}

/**
 * 查询用户密码是否正确。
 * @param db_user 用户数据库指针
 * @param user_name 用户名
 * @param user_password 用户输入的密码，用于验证身份
 * @return 如果密码正确，返回1；如果密码错误或用户不存在，返回0
 */
int Query_password(sqlite3 *db_user, char *user_name, char *user_password) {
    for (int i = 0; i < user_number; i++) {
        if (strcmp(user[i].name, user_name) == 0) {
            if (strcmp(user[i].password, user_password) == 0) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }
}

/**
 * 初始化用户信息。
 * 该函数从文件"data/User.txt"中读取用户姓名和密码，并将这些信息插入到SQLite数据库的User表中。
 * 如果用户已存在，则跳过该用户的信息插入。
 * @param db_user 用户数据库指针
 */
void Init_user(sqlite3 *db_user) {
    FILE *fp; 
    fp = fopen("data/User.txt", "r"); //打开文件
    if (fp == NULL) { //文件打开失败
        printf("--> Can't open file <--\n");
        return;
    }
    char name[20], passward[20];
    //读取文件
    while(fscanf(fp, "%s%s", name, passward) != EOF){
        if (Query_user(db_user, name) > 0) {//用户已存在
            continue;
        }
        char sql[100], *err_msg = NULL;
        sprintf(sql, "INSERT INTO User VALUES('%s', '%s');", name, passward); //创建SQL语句
        //printf("sql: %s\n", sql);
        int rc = sqlite3_exec(db_user, sql, NULL, NULL, err_msg);//执行SQL语句
        
        if (rc != SQLITE_OK) {
            printf("--> SQL error: %s <--\n", err_msg);
            sqlite3_free(err_msg);
        }
    }
    printf("--> User inited successfully <--\n");
    fclose(fp);//关闭文件
}

/**
 * 获取用户信息
 * 该函数通过提供的参数填充用户信息结构体数组中的下一个位置。
 * 它从输入参数中提取用户名和密码，并将其存储在全局用户数组中，然后增加用户数量计数
 */
int get_user_information(void *data, int argc, char **argv, char **azColName) {
    strcpy(user[user_number].name, argv[0]); //获取用户名
    strcpy(user[user_number].password, argv[1]); //获取密码
    user_number++; //增加用户数量
    return 0;
}
/**
 * 从数据库中获取用户信息
 * 查询的结果将通过预定义的回调函数get_user_information处理。
 * 
 * @param db_user 用户数据库指针
 */
void Get_user_imformation_from_database(sqlite3 *db_user) {
    user_number = 0; //初始化用户数量
    char *sql = "SELECT * FROM User", *err_msg = NULL; //创建SQL语句
    int rc = sqlite3_exec(db_user, sql, get_user_information,NULL, &err_msg); //执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> User imformation getted successfully <--\n");
    }
}

/**
 * 添加用户到数据库
 * 该函数首先检查用户是否已存在，如果存在，则不添加并返回；否则，构造SQL语句插入新用户信息到数据库。
 * 如果插入操作失败，将打印错误信息并释放错误消息内存；如果插入操作成功，将打印成功添加用户的提示信息。
 * @param db_user 用户数据库指针
 * @param user_name 新用户的用户名
 * @param user_passward 新用户的密码
 */
void Add_user(sqlite3 *db_user, char *user_name, char *user_passward) {
    if (Query_user(db_user, user_name) > 0) { //用户已存在
        printf("--> User already exists <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "INSERT INTO User VALUES('%s', '%s');", user_name, user_passward);//创建SQL语句
    int rc = sqlite3_exec(db_user, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> User added successfully <--\n");
    }
}

/**
 * 删除数据库中的用户
 * 本函数首先尝试查询指定用户名是否存在，若不存在，则直接返回。
 * 存在时，构造SQL语句删除该用户，并执行SQL语句。
 * @param db_user 用户数据库指针
 * @param user_name 待删除的用户名
 */
void Delete_user(sqlite3 *db_user, char *user_name) {
    if (Query_user(db_user, user_name) == 0) {//用户不存在
        printf("--> User not exists <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "DELETE FROM User WHERE Name = '%s';", user_name);//创建SQL语句
    int rc = sqlite3_exec(db_user, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> User deleted successfully <--\n");
    }
}

/**
 * 修改用户密码函数
 * 该函数首先检查用户是否存在，如果存在，则通过SQL语句更新用户的密码
 * @param db_user 用户数据库指针
 * @param user_name 需要修改密码的用户名
 * @param new_passward 新的用户密码
 */
void Modify_password(sqlite3 *db_user, char *user_name, char *new_passward) {
    if (Query_user(db_user, user_name) == 0) {//用户不存在
        printf("--> User not exists <--\n");
        return;
    }
    char sql[100], *err_msg = NULL;
    sprintf(sql, "UPDATE User SET Passward = '%s' WHERE Name = '%s';", new_passward, user_name);//创建SQL语句
    int rc = sqlite3_exec(db_user, sql, NULL, NULL, err_msg);//执行SQL语句
    if (rc != SQLITE_OK) {
        printf("--> SQL error: %s <--\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("--> Password modified successfully <--\n");
    }
}
