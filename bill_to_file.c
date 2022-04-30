#include<stdio.h>
#include<string.h>
#include<mysql.h>

void bill_to_file(MYSQL* connection)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0;
    char space[17] = "             ";
    char query[100] = "";
    int  net=0;
    int  total=0;
    char timestamp[50]="";

    FILE *fptr = fopen("bill.txt","w");
    sprintf(query,"select current_timestamp()");
    if(mysql_query(connection, query))
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        exit(1);
    }
    result_set = mysql_use_result(connection);
    row = mysql_fetch_row(result_set);

    strcpy(timestamp,row[0]);
    mysql_free_result(result_set);

    sprintf(query, "select * from cart");
    //printf("\e[1;1H\e[2J");
    //printf("\n\n\n\n");
    if(mysql_query(connection, query))
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        exit(1);
    }

    result_set = mysql_use_result(connection);

    do
    {
        row = mysql_fetch_row(result_set);

        if(!row)
            break;
        else if(row_count == 0)
        {
            fprintf(fptr,"\n\n\n\n%s+---------------------------------------------------------------------------------+\n",space);
            fprintf(fptr,"%s|                             INVOICE:  %-30s            |\n",space,timestamp);
            fprintf(fptr,"%s+---------------------------------------------------------------------------------+\n",space);
            fprintf(fptr,"%s|  S.No  | PRODUCT NAME | BRAND NAME | BRAND MODEL |   PRICE   | COUNT |   Bill   |\n",space);
            fprintf(fptr,"%s+--------+--------------+------------+-------------+-----------+-------+----------+\n",space);
        }
        net = atoi(row[4])*atoi(row[3]);
        total += net;
        fprintf(fptr,"%s| %-6d | %-12s | %-10s | %-11s |  %-7s  | %-5s | %-8d |\n",space,row_count+1,row[0],row[1],row[2],row[3],row[4],net);
        row_count++;
    }
    while(1);

    if(row_count)
    {
        fprintf(fptr,"%s+---------------------------------------------------------------------------------+\n",space);
        fprintf(fptr,"%s|                             Total Bill  - $%-10d                           |\n",space,total);
        fprintf(fptr,"%s+---------------------------------------------------------------------------------+\n\n\n\n\n",space);
    }
}
