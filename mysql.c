#include<stdio.h>
#include<stdlib.h>
#include<mysql.h>
#include<string.h>
#include<unistd.h>

void display_product_list(MYSQL *connection)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0, user_input;
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

    printf("\n\n\n\n             Enter any key to go back to the menu: ");
    while((user_input = getchar()) != EOF)
    {
        if(user_input == '\n')
            break;
    }
}

void delete_product(MYSQL *connection)
{
    char product_name[50] ="";
    int  user_input;
    int  index = 0;
    char space[17] = "             ";
    char query[200] = "";

    MYSQL_RES *result_set;

    printf("\e[1;1H\e[2J");
    printf("\n\n");

    printf("\n\n\n\n\n%sPlease enter the name of the product to be deleted: ",space);

    while((user_input = getchar()) != EOF)
    {
        if(index == 49)
            break;
        if(user_input == '\n')
            break;
        else
            product_name[index++] = user_input;
    }

    sprintf(query, "select * from products where product_name = '%s'",product_name);

    if (mysql_query(connection, query))
    {
        fprintf(stderr, "%s\n", mysql_error(connection));
        mysql_close(connection);
        exit(1);
    }

    result_set = mysql_store_result(connection);

    if(mysql_num_rows(result_set))
    {
        memset(query,'\0',sizeof(query));
        if(product_name[0] >= 'a')
            product_name[0] = product_name[0] - 'a' + 'A';  //Upper casing the 1st character

        sprintf(query,"delete from products where product_name = '%s'",product_name);

        if (mysql_query(connection, query))
        {
            fprintf(stderr, "%s\n", mysql_error(connection));
            mysql_close(connection);
            exit(1);
        }

        printf("\n\n%sCurrent list of products after deleting product <%s>:\n\n\n",space,product_name);
    }
    else
        printf("%sProduct \"%s\" doesn't exist in the list of products:\n\n\n",space,product_name);

    display_product_list(connection);
}

void display_product_laptop_inventory(MYSQL *connection)
{
    MYSQL_RES *result_set;
    MYSQL_ROW row;
    int row_count = 0, user_input;
    char space[6] = "     ";
    char query[100] = "";

    sprintf(query, "select * from Laptop");

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
            printf("\n\n\n\n+------+------------+----------------+-----------------+------------------+------------+------------+--------------------+---------------------+-------+\n");
            printf("| S.No | brand_name | processor_name | processor_model | operating_system |    ram     |   price    | hard_disk_capacity | condition_of_laptop | count |\n");
            printf("+------+------------+----------------+-----------------+------------------+------------+------------+--------------------+---------------------+-------+\n");

        }
        printf("| %4s | %-10s |   %-10s   |   %-10s    |    %-10s    | %-10s | %-10s |        %5s       |     %-10s      | %5s |\n",row[9],row[0],row[1],row[2],row[3],row[4],row[5],row[6],row[7],row[8]);
        row_count++;
    }
    while(1);

    if(row_count)
        printf("+------+------------+----------------+-----------------+------------------+------------+------------+--------------------+---------------------+-------+\n");

    else
    {
        printf("\n\n\n\n                                      +--------------------------------------+\n");
        printf("                                      | 0 rows fetched from \"PRODUCTS\" table |\n");
        printf("                                      +--------------------------------------+\n");
    }

    printf("\n\n\n\n             Enter any key to go back to the menu: ");
    while((user_input = getchar()) != EOF)
    {
        if(user_input == '\n')
            break;
    }

}

void add_product(MYSQL *connection)
{
    char product_name[50] ="";
    int  user_input;
    int  index = 0;
    char space[17] = "             ";
    char query[200] = "";

    MYSQL_RES *result_set;

    printf("\e[1;1H\e[2J");
    printf("\n\n");

    printf("\n\n\n\n\n%sPlease enter the name of the product to be entered: ",space);

    while((user_input = getchar()) != EOF)
    {
        if(index == 49)
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
            mysql_close(connection);
            exit(1);
        }
        
        printf("\n\n%sCurrent list of products after adding product <%s>:\n\n\n",space,product_name);
    }
    else
	printf("%sProduct \"%s\" already exists in the list of products:\n\n\n",space,product_name);
	
    display_product_list(connection);
}

void database_connect(int option) 
{
	MYSQL *connection;
	
	char *server = "helios.vse.gmu.edu";
	char *user = "gsingh33";
	char *password = "glupteef";
	char *database = "gsingh33";
	
	connection = mysql_init(NULL);
	
	if (!mysql_real_connect(connection, server, user, password, database, 0, NULL, 0)) 
    {
		fprintf(stderr, "%s\n", mysql_error(connection));
		exit(1);
	}
   
    switch (option) 
    {
        case 1:
        {
            //display_product_laptop_inventory(connection);
            display_product_list(connection);
            break;
        }
        case 2:
        {
            display_product_laptop_inventory(connection);
            //add_product(connection);
            break;
        }
        case 3:
        {
            delete_product(connection);
            break;
        }
    }
}
