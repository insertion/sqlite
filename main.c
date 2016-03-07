/*  
 * File:   sqlite_test.cpp 
 * Author: willie
 * 
 * Created on 2016��3��3�� 
 */  
  
#include <stdlib.h>  
#include <stdio.h>  
#include "sqlite3.h"  

/*
 *��������  _sql_callback 
 *���ߣ�    willie
 *����޸ģ�2016-3-3 
 *���ܣ�    it is a callback fuction which is invoked for each result row coming out of the evaluated SQL statements
 *���������notused    is a void * argument getting from sqlite3_exec()
            argc       is the number of columns in the sqlite3_exec result, sqlite3_column_count().
            argv       is an array of pointers to strings obtained as if from sqlite3_column_text(), one for each column
            szColName  is an array of pointers to strings where each entry represents the name of corresponding result column as obtained from sqlite3_column_name().
 *����ֵ��  ���ͣ�int)
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
 sqlite3 *db;          //���ݿ���� 
 sqlite3_stmt *ppStmt; //���Ԥ������� 
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
	 	printf("���ݿ⽨��/���ӳɹ�\n");
		return SQLITE_OK; 
	}
	else
	    return SQLITE_BUSY;
 	//sqlite3_prepare_v2(db,"create table wanglei(userid varchar(20) PRIMARY KEY, age int, birthday datetime);",-1,&ppStmt,0);
	//These routines only compile the first statement in zSql
	//sqlite3_step(ppStmt);//SQLITE_OK��������ִ�гɹ���SQLITE_ROW�������ض��н������Ҫ����ִ��sqlite3_step() 
 }
 int create_tab()
 {
 	if(sqlite3_prepare_v2(db,sSQL1,-1,&ppStmt,0)==SQLITE_OK)
 		printf("create_tab:SQL������ȷ!\n");
	/*
	 *��������  sqlite3_prepare_v2 
	 *���ߣ�    willie
	 *����޸ģ�2016-3-3 
	 *���ܣ�    pr-compile the SQL statement
	 *���������db    is a db crete by sqlite3_open()
	            zSql  is the statement,����﷨Ҫ��ȷ 
	            nbyte  -1 ��ʾ����ĩβ,������ʾ�������ֽ� 
	            ppstmt  �����洢������sql���
				pztail ���ǿգ��������洢�ڶ���sql�������ֽڣ���Ϊ�ú���ֻ�����һ����䣬ʣ�µı�����pztail�У�һ��ò�����0 
	 *����ֵ��  ���ͣ�int)
	 *          returns SQLITE_OK(0)����error code 
	 */
	 sqlite3_step(ppStmt);
	 /*
	 *��������  sqlite3_step() 
	 *���ߣ�    willie
	 *����޸ģ�2016-3-3 
	 *���ܣ�    excute the pr-complied sql statement
	 *��������� 
	            ppstmt  �����洢������sql���
	 *����ֵ��  ���ͣ�int)
	 *          returns SQLITE_DONE(���ִ�н���),SQLITE_ROW(�õ���һ�н������δ��������Ҫ����ִ��step),SQLITE_BUSY,SQLITE_ERROR 
	 */
	 sqlite3_finalize(ppStmt);
	// The application must finalize every prepared statement in order to avoid resource leaks
 }
 int insert_db()
 {
 	if(sqlite3_prepare_v2(db,"insert into users values('wanglei',25,'1993-09-12');",-1,&ppStmt,0)==SQLITE_OK)
 		printf("insert_db:SQL������ȷ\n");
 	if(sqlite3_step(ppStmt)==SQLITE_DONE)
 		printf("�������ݳɹ�\n");
 	sqlite3_finalize(ppStmt);
 	return 0;
 }
 int select_db()
 {
 	sqlite3_prepare_v2(db,"select * from users;",-1,&ppStmt,0);
 	int index=0;
 	while(sqlite3_step(ppStmt)==SQLITE_ROW)
 	{
 		int col_count=sqlite3_column_count(ppStmt);//��ȡ�е�����
		for(index=0;index<col_count;index++)
		{
			const char *col_name=sqlite3_column_name(ppStmt,index);//�������� 
			printf("name:%s\t",col_name);
 			printf("value:%s\n",sqlite3_column_text(ppStmt,index));//���ַ�����ʽ����ֵ 
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
