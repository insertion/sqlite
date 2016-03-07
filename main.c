/*  
 * File:   sqlite_test.cpp 
 * Author: willie
 * 
 * Created on 2016年3月3日 
 */  
  
#include <stdlib.h>  
#include <stdio.h>  
#include "sqlite3.h"  

/*
 *函数名：  _sql_callback 
 *作者：    willie
 *最后修改：2016-3-3 
 *功能：    it is a callback fuction which is invoked for each result row coming out of the evaluated SQL statements
 *输入参数：notused    is a void * argument getting from sqlite3_exec()
            argc       is the number of columns in the sqlite3_exec result, sqlite3_column_count().
            argv       is an array of pointers to strings obtained as if from sqlite3_column_text(), one for each column
            szColName  is an array of pointers to strings where each entry represents the name of corresponding result column as obtained from sqlite3_column_name().
 *返回值：  类型（int)
 *          returns non-zero, the sqlite3_exec() routine returns SQLITE_ABORT without invoking the callback again and 
 *          without running any subsequent SQL statements
 */
static int _sql_callback(void *notused, int argc, char **argv, char **szColName)  
{  
    int i = 0;  
      
    printf("notused:0x%x, argc:%d\n", notused, argc);  
    for (i = 0; i < argc; i++)  
    {  
        printf("%s = %s\n", szColName[i], argv[i] == 0 ? "NULL" : argv[i]);  
    }  
    printf("\n");  
      
    return 0;  
} 
 sqlite3 *db;          //数据库对象 
 sqlite3_stmt *ppStmt; //语句预处理对象 
 const char *sSQL1 = "create table users(userid varchar(20) PRIMARY KEY, age int, birthday datetime);";  
 const char *sSQL2 = "insert into users values('wang', 20, '1989-5-4');";  
 const char *sSQL3 = "select * from users;";  
/* 
 *  
 */  
int open_db()
 {
 
 	if(sqlite3_open("./hello.db",&db)==SQLITE_OK)
 	{
	 	printf("数据库建立/连接成功\n");
		return SQLITE_OK; 
	}
	else
	    return SQLITE_BUSY;
 	//sqlite3_prepare_v2(db,"create table wanglei(userid varchar(20) PRIMARY KEY, age int, birthday datetime);",-1,&ppStmt,0);
	//These routines only compile the first statement in zSql
	//sqlite3_step(ppStmt);//SQLITE_OK表明程序执行成功，SQLITE_ROW表明返回多行结果，需要继续执行sqlite3_step() 
 }
 int create_tab()
 {
 	if(sqlite3_prepare_v2(db,sSQL1,-1,&ppStmt,0)==SQLITE_OK)
 		printf("create_tab:SQL解析正确!\n");
	/*
	 *函数名：  sqlite3_prepare_v2 
	 *作者：    willie
	 *最后修改：2016-3-3 
	 *功能：    pr-compile the SQL statement
	 *输入参数：db    is a db crete by sqlite3_open()
	            zSql  is the statement,语句语法要正确 
	            nbyte  -1 表示读到末尾,正数表示读几个字节 
	            ppstmt  用来存储编译后的sql语句
				pztail 若非空，则用来存储第二个sql语句的首字节，因为该函数只编译第一个语句，剩下的保存在pztail中，一般该参数置0 
	 *返回值：  类型（int)
	 *          returns SQLITE_OK(0)或者error code 
	 */
	 sqlite3_step(ppStmt);
	 /*
	 *函数名：  sqlite3_step() 
	 *作者：    willie
	 *最后修改：2016-3-3 
	 *功能：    excute the pr-complied sql statement
	 *输入参数： 
	            ppstmt  用来存储编译后的sql语句
	 *返回值：  类型（int)
	 *          returns SQLITE_DONE(语句执行结束),SQLITE_ROW(得到第一行结果，还未结束，需要继续执行step),SQLITE_BUSY,SQLITE_ERROR 
	 */
	 sqlite3_finalize(ppStmt);
	// The application must finalize every prepared statement in order to avoid resource leaks
 }
 int insert_db()
 {
 	if(sqlite3_prepare_v2(db,"insert into users values('wanglei',25,'1993-09-12');",-1,&ppStmt,0)==SQLITE_OK)
 		printf("insert_db:SQL解析正确\n");
 	if(sqlite3_step(ppStmt)==SQLITE_DONE)
 		printf("插入数据成功\n");
 	sqlite3_finalize(ppStmt);
 	return 0;
 }
 int select_db()
 {
 	sqlite3_prepare_v2(db,"select * from users;",-1,&ppStmt,0);
 	int index=0;
 	while(sqlite3_step(ppStmt)==SQLITE_ROW)
 	{
 		int col_count=sqlite3_column_count(ppStmt);//获取行的列数
		for(index=0;index<col_count;index++)
		{
			const char *col_name=sqlite3_column_name(ppStmt,index);//返回列名 
			printf("name:%s\t",col_name);
 			printf("value:%s\n",sqlite3_column_text(ppStmt,index));//以字符串形式返回值 
	    }	
	}
 	sqlite3_finalize(ppStmt);
 	return 0;
 }
 void close_db()
 {
 	sqlite3_close(db);
 }
int main(int argc, char** argv)  
{ 
	open_db();
	create_tab(); 
	insert_db();
	select_db();
	close_db();
} 
