#include<stdio.h>
#include<string.h>
#include<mysql.h>

extern void display_product_list(MYSQL*); 
void item_details_user_input(char*, char*, char*, int*, int*);
int  display_laptop_inventory(MYSQL*);
int  display_Refrigerator_inventory(MYSQL*);
int  display_item_inventory(MYSQL*, char*);
void input_serial_no(char*); 

struct node
{
    char   attribute_name[100];
    char   type;
    int    len;
};

void delete_item(MYSQL *connection)
{
    int   user_input, index;
    char  buff[30]="";
    char  query[300]="";
    char  temp_query[200]="";
    char  space[15] = "             ";
    char  user_count[10]="";
    char  id[10]="";
    int   ind = 0;
    MYSQL_RES *result_set;
    MYSQL_ROW row;

    while(1)
    {
        memset(buff,'\0',sizeof(buff));
        memset(query,'\0',sizeof(query));
        memset(temp_query,'\0',sizeof(temp_query));
        memset(id,'\0',sizeof(id));
        memset(user_count,'\0',sizeof(user_count));
        index = 0;

        printf("\e[1;1H\e[2J");
        printf("\n\n\n\n");
        if(ind)
        {
            printf("\n\n%sError: entered product doesn't exist in the product table\n\n",space);
            ind = 0;
        }
        display_product_list(connection);
        printf("\n\n%sPlease select a product from the above displayed list: ",space);

        while((user_input = getchar()) != EOF)
        {
            if(index == 29)
                break;
            if(user_input == '\n')
            {
                if(!index)
                {
                    printf("%sError: product name can't be blank, please re-enter: ",space);
                    continue;
                }
                break;
            }
            buff[index++] = (user_input >= 'A')&&(user_input <= 'Z')?user_input+'a'-'A':user_input;
        }

        if((buff[0] >= 'a')&&(buff[0] <= 'z'))
            buff[0] = buff[0] - 'a' + 'A';

        sprintf(query,"select * from products where product_name = '%s'",buff);
        if (mysql_query(connection, query))
        {
            fprintf(stderr, "%s\n", mysql_error(connection));
            mysql_close(connection);
            exit(1);
        }

        result_set = mysql_use_result(connection);

        row = mysql_fetch_row(result_set);

        if(!row)
        {
            ind = 1;
            continue;
        }

        memset(query, '\0',sizeof(query));
        mysql_free_result(result_set);
        if(!strcmp(buff,"Laptop"))
        {
            if(!display_laptop_inventory(connection))
            {
                printf("\n\n\n\n%sWould you like to delete items from any other product? Y/N ",space);
                while((user_input = getchar()) != EOF)
                {
                    if(user_input == '\n')
                    {
                        if(!index)
                        {
                            printf("\n\n%sError: No valid input has been entered, please try again: ",space);
                            continue;
                        }
                        break;
                    }
                    if((user_input == 'Y')||(user_input == 'y'))
                    {
                        index = 99999999;
                    }
                    if((user_input == 'N')||(user_input == 'n'))
                        index = 9999;
                    if((index != 99999999)&&(index != 9999))
                        index++;
                }
            }
            if(index == 99999999)
                continue;
            if(index == 9999)
                return;
        }
        else if(!strcmp(buff,"Camera"))
        {

        }
        else if(!strcmp(buff,"Refrigerator"))
        {
            if(!display_Refrigerator_inventory(connection))
            {
                printf("\n\n\n\n%sWould you like to delete items from any other product? Y/N ",space);
                while((user_input = getchar()) != EOF)
                {
                    if(user_input == '\n')
                    {
                        if(!index)
                        {
                            printf("\n\n%sError: No valid input has been entered, please try again: ",space);
                            continue;
                        }
                        break;
                    }
                    if((user_input == 'Y')||(user_input == 'y'))
                    {
                        index = 99999999;
                    }
                    if((user_input == 'N')||(user_input == 'n'))
                        index = 9999;
                    if((index != 99999999)&&(index != 9999))
                        index++;
                }
            }
            if(index == 99999999)
                continue;
            if(index == 9999)
                return;
        }
        else if(!strcmp(buff,"Phone"))
        {

        }
        else
        {
            if(!display_item_inventory(connection, buff))
            {
                printf("\n\n\n\n%sWould you like to delete items from any other product? Y/N ",space);
                while((user_input = getchar()) != EOF)
                {
                    if(user_input == '\n')
                    {
                        if(!index)
                        {
                            printf("\n\n%sError: No valid input has been entered, please try again: ",space);
                            continue;
                        }
                        break;
                    }
                    if((user_input == 'Y')||(user_input == 'y'))
                    {
                        index = 99999999;
                    }
                    if((user_input == 'N')||(user_input == 'n'))
                        index = 9999;
                    if((index != 99999999)&&(index != 9999))
                        index++;
                }
            }
            if(index == 99999999)
                continue;
            if(index == 9999)
                return;
        }
        while(1)
        {
            printf("\n\n%sPlease enter the serial no. of the item from the above displayed list: ",space);

            index=0;

            input_serial_no(id);
            sprintf(query,"select * from %s where item_id = '%s%03d'",buff,buff,atoi(id));

            if (mysql_query(connection, query))
            {
                printf("Here0\n");
                fprintf(stderr, "%s\n", mysql_error(connection));
                mysql_close(connection);
                exit(1);
            }

            result_set = mysql_use_result(connection);

            row = mysql_fetch_row(result_set);
            if(!row)
            {
                printf("%sError: incorrect serial number entered, please re-enter: ",space);
                input_serial_no(id);
                continue;
            }
            else
                break;
        }

        index = 0;

        sprintf(query,"Delete from %s where item_id = '%s%03d'",buff,buff,atoi(id));
        mysql_free_result(result_set);
        if(mysql_query(connection, query))
        {
            fprintf(stderr, "%s\n", mysql_error(connection));
            mysql_close(connection);
            exit(1);
        }
        printf("\n\n%sItem delete successfully\n\n",space);
        printf("%sWould you like to delete any other item? Y/N ",space);
        while((user_input = getchar()) != EOF)
        {
            if(user_input == '\n')
            {
                if(!index)
                {
                    printf("\n\n%sError: No valid input has been entered, please try again: ",space);
                    continue;
                }
                break;
            }
            if((user_input == 'Y')||(user_input == 'y'))
            {
                index = 99999999;
            }
            if((user_input == 'N')||(user_input == 'n'))
                index = 9999;
            if((index != 99999999)&&(index != 9999))
                index++;
        }
        if(index == 99999999)
            continue;
        if(index == 9999)
            return;
    }    
}

int display_Refrigerator_inventory(MYSQL* connection)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0;
    char space[17] = "             ";
    char query[100] = "";

    sprintf(query, "select * from Refrigerator order by id");
    printf("\e[1;1H\e[2J");
    printf("\n\n\n\n");
    if (mysql_query(connection, query))
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
            printf("\n\n\n\n%s+--------+------------+-------------+----------+-------+------------+-----------+\n",space);
            printf("%s|  S.No  | BRAND NAME | BRAND MODEL | CAPACITY | DOORS |   PRICE    |   COUNT   |\n",space);
            printf("%s+--------+------------+-------------+----------+-------+------------+-----------+\n",space);
        }
         printf("%s| %-6s | %-10s | %-11s | %-8s | %-5s | %-10s | %-9s |\n",space,row[6],row[0],row[1],row[2],row[3],row[4],row[5]);
        row_count++;
    }
    while(1);

    if(row_count)
         printf("%s+--------+------------+-------------+----------+-------+------------+-----------+\n",space);
    else
    {
        printf("\n\n\n\n                                      +---------------------------------------+\n");
        printf("                                      | Sorry, This product is out of stock.  |\n");
        printf("                                      +---------------------------------------+\n");
        return 0;
    }
    return 1;
}

int display_laptop_inventory(MYSQL *connection)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0;
    char query[100] = "";

    sprintf(query, "select * from Laptop order by id");

    printf("\e[1;1H\e[2J");
    printf("\n\n\n\n");
    if (mysql_query(connection, query))
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
            printf("\n\n\n\n +------+------------+----------------+-----------------+------------------+------------+------------+--------------------+---------------------+-------+\n");
            printf(" | S.No | brand_name | processor_name | processor_model | operating_system |    ram     |   price    | hard_disk_capacity | condition_of_laptop | count |\n");
            printf(" +------+------------+----------------+-----------------+------------------+------------+------------+--------------------+---------------------+-------+\n");

        }
        printf(" | %4s | %-10s |   %-10s   |   %-10s    |    %-10s    | %-10s | %-10s |        %5s       |     %-10s      | %5s |\n",row[9],row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7],row[8]);
        row_count++;
    }
    while(1);

    if(row_count)
        printf(" +------+------------+----------------+-----------------+------------------+------------+------------+--------------------+---------------------+-------+\n");

    else
    {
        printf("\n\n\n\n                                      +---------------------------------------+\n");
        printf("                                      | Sorry, This product is out of stock.  |\n");
        printf("                                      +---------------------------------------+\n");
        return 0;
    }
    return 1;
}


int display_item_inventory(MYSQL* connection, char *product_name)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0;
    char space[17] = "             ";
    char query[100] = "";

    sprintf(query, "select * from %s order by id",product_name);
    printf("\e[1;1H\e[2J");
    printf("\n\n\n\n");
    if (mysql_query(connection, query))
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
            printf("\n\n\n\n%s+--------+------------+-------------+------------+------------------+\n",space); 
            printf("%s|  S.No  | BRAND NAME | BRAND MODEL |   PRICE    | AVAILABLE PIECES |\n",space);
            printf("%s+--------+------------+-------------+------------+------------------+\n",space);
        }
        printf("%s| %-6s | %-10s | %-11s | %-10s | %-16s |\n",space,row[4],row[0],row[1],row[2],row[3]);
        row_count++;
    }
    while(1);

    if(row_count)
        printf("%s+--------+------------+-------------+------------+------------------+\n",space);
    else
    {
        printf("\n\n\n\n                                      +---------------------------------------+\n");
        printf("                                      | Sorry, This product is out of stock.  |\n");
        printf("                                      +---------------------------------------+\n");
        return 0;
    } 
    return 1;
}

void add_items_in_product(MYSQL* connection)
{
    int  user_input;
    char product_name[30]="";
    char space[17] = "             ";
    char query[400] = "";
    char brand_name[10]="";
    char brand_model[10]="";
    int  price = 123;
    int  id =2;
    int  count=0;
    MYSQL_RES* result_set;
    MYSQL_ROW  row;

    display_product_list(connection);
    item_details_user_input(product_name,brand_name,brand_model,&price,&count);

    sprintf(query, "select * from products where product_name = '%s'",product_name);
    if (mysql_query(connection, query))
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        exit(1);
    }

    result_set = mysql_store_result(connection);

    id = mysql_num_rows(result_set);

    sprintf(query,"insert into %s(brand_name,brand_model,price,count,id,item_id) values('%s','%s',%d,%d,%d,'%s%03d')",product_name,brand_name,brand_model,price,count,id,product_name,id);

    if (mysql_query(connection, query))
    {
        const char *ptr = mysql_error(connection);

        if(!strncmp(ptr,"Duplicate",9))
        {
            printf("%sRow already exists, updating the count\n",space);
            memset(query,'\0',sizeof(query));
            sprintf(query,"select count from %s where brand_name = '%s' and brand_model = '%s'",product_name,brand_name,brand_model);

            if (mysql_query(connection, query))
            {
                fprintf(stderr, "%s\n", mysql_error(connection));
                mysql_close(connection);
                exit(1);
            }
            result_set = mysql_store_result(connection);
            row = mysql_fetch_row(result_set);

            count = count + atoi(row[0]);
            memset(query,'\0',sizeof(query));
            sprintf(query,"update %s set count = %d where brand_name = '%s' and brand_model = '%s'",product_name,count,brand_name,brand_model);
            if (mysql_query(connection, query))
            {
                fprintf(stderr, "%s\n", mysql_error(connection));
                mysql_close(connection);
                exit(1);
            }
            mysql_close(connection);
        }
    }
    printf("\n\n\n\n             Enter any key to go back to the menu: ");
    while((user_input = getchar()) != EOF)
    {
        if(user_input == '\n')
            break;
    }
}


void item_details_user_input(char* product_name, char* brand_name, char* brand_model, int* price, int* count)
{
    int  index = 0;
    int  user_input=0;
    char space[15] = "             ";
    char buff[10]="";

    printf("%sEnter the product name from above list: ",space);
    while((user_input = getchar()) != EOF)
    {
        if(index == 29)
            break;
        if(user_input == '\n')
        {
            if(index == 0)
            {
                printf("%sError: product name cannot be blank, please re-enter\n",space);
                continue;
            }
            break;
        }
        else
            product_name[index++] = user_input;
    }

    index = 0;
    user_input = 0;
    printf("\n%sEnter the brand name: ",space);
    while((user_input = getchar()) != EOF)
    {
        if(index == 9)
            break;
        if(user_input == '\n')
        {
            if(index == 0)
            {
                printf("%sError: brand name cannot be blank, please re-enter\n",space);
                continue;
            }
            break;
        }
        else
            brand_name[index++] = user_input;
    }

    index = 0;
    user_input = 0;
    printf("\n%sEnter the brand model: ",space);
    while((user_input = getchar()) != EOF)
    {
        if(index == 9)
            break;
        if(user_input == '\n')
        {
            if(index == 0)
            {
                printf("%sError: brand model cannot be blank, please re-enter\n",space);
                continue;
            }
            break;
        }
        else
            brand_model[index++] = user_input;
    }

    index = 0;
    user_input = 0;
    printf("\n%sEnter the price: ",space);
    while((user_input = getchar()) != EOF)
    {
        if(index == 9)
            break;
        if(user_input == '\n')
        {
            if(index == 0)
            {
                printf("%sError: price cannot be blank, please re-enter\n",space);
                continue;
            }
            break;
        }
        else if((user_input < '0')||(user_input > '9'))
        {
            printf("%sError: price should be a digit, please re-enter\n",space);
            memset(buff,'\0',sizeof(buff));
            continue;
        }
            buff[index++] = user_input;
    }
    *price = atoi(buff);

    memset(buff,'\0',sizeof(buff));

    index = 0;
    user_input = 0;
    printf("\n%sEnter the Count of this item: ",space);
    while((user_input = getchar()) != EOF)
    {
        if(index == 9)
            break;
        if(user_input == '\n')
        {
            break;
        }
        else if((user_input < '0')||(user_input > '9'))
        {
            printf("%sError: Count should be a digit, please re-enter\n",space);
            memset(buff,'\0',sizeof(buff));
            continue;
        }
            buff[index++] = user_input;
    }
    *count = atoi(buff);
}

