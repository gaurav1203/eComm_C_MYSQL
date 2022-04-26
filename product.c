#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>

void display_product_list(MYSQL *connection)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0;
    char space[17] = "                ";
    char query[100] = "";

    sprintf(query, "select * from products order by product_id");

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
            printf("\n\n\n\n%s+------------+--------------------------------+\n",space); //column names to be printed only once (before any row data is printed)
            printf("%s| PRODUCT ID |      PRODUCT NAME              |\n",space);
            printf("%s+------------+--------------------------------+\n",space);
        }
        printf("%s| %-10s | %-30s |\n",space,row[0],row[1]);
        row_count++;
    }
    while(1);

    if(row_count)
        printf("%s+------------+--------------------------------+\n",space);
    else
    {
        printf("\n\n\n\n                                      +--------------------------------------+\n");
        printf("                                      | 0 rows fetched from \"PRODUCTS\" table |\n");
        printf("                                      +--------------------------------------+\n");
    }
}


char* add_product(MYSQL *connection)
{
    char *product_name = malloc(30);
    int  user_input;
    int  index = 0;
    char space[17] = "             ";
    char query[200] = "";


    MYSQL_RES *result_set;

    printf("\e[1;1H\e[2J");
    printf("\n\n");
    memset(product_name,'\0',30);
    printf("\n\n\n\n\n%sPlease enter the name of the product to be entered: ",space);

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

    sprintf(query, "select * from products where product_name = '%s'",product_name);

    if (mysql_query(connection, query))
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        memset(product_name,'\0',30);
        product_name = NULL;
        exit(1);
    }

    result_set = mysql_store_result(connection);

    if(!mysql_num_rows(result_set))
    {
        memset(query,'\0',sizeof(query));
        if(product_name[0] >= 'a')
            product_name[0] = product_name[0] - 'a' + 'A';  //Upper casing the 1st character

        sprintf(query,"insert into products (product_name) values(\"%s\")",product_name);

        if (mysql_query(connection, query))
        {
            fprintf(stderr, "%s\n", mysql_error(connection));
            memset(product_name,'\0',30);
            product_name = NULL;
            mysql_close(connection);
            exit(1);
        }

        printf("\n\n%sCurrent list of products after adding product <%s>:\n\n\n",space,product_name);
    }
    else
        printf("%sProduct \"%s\" already exists in the list of products:\n\n\n",space,product_name);

    display_product_list(connection);
    return product_name;
}

void create_product_table(MYSQL *connection, char *product_name)
{
    char query[300] = "";

    sprintf(query, "create table %s(brand_name varchar(10),brand_model varchar(10),price decimal(10,2) default 0, count int default 0,id int,item_id varchar(15), primary key(brand_name,brand_model))",product_name);

    if (mysql_query(connection, query))
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        exit(1);
    }
    printf("Table for product \"%s\" created\n",product_name); 
}

void item_details_user_input(char* product_name, char* brand_name, char* brand_model, int* price, int* count)
{
    int  index = 0;
    int  user_input=0;
    char space[15] = "      ";
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

void add_items_in_product(MYSQL* connection)
{
    int  user_input;
    char product_name[30]="";
    char space[17] = "                ";
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
    return;
}
