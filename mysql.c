#include<stdio.h>
#include<stdlib.h>
#include<mysql.h>
#include<string.h>
#include<unistd.h>

extern void  create_product_table(MYSQL*, char*);
extern void  display_product_list(MYSQL*);
extern char* add_product(MYSQL*);
extern void  add_items_in_product(MYSQL*);
extern void  delete_product(MYSQL*);
extern void  display_items(MYSQL*);
extern void  add_to_cart(MYSQL*);
extern void  clear_cart(MYSQL*);
extern void  bill_to_file(MYSQL*);
extern void  delete_item(MYSQL*);

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
    delete_item(connection);
    //bill_to_file(connection);
    //clear_cart(connection);
    //add_to_cart(connection);
    return;
    switch (option) 
    {
        case 1:
        {
            create_product_table(connection,add_product(connection));
            break;
        }
        case 2:
        {
            add_items_in_product(connection);
            //display_product_laptop_inventory(connection);
            //add_product(connection);
            break;
        }
        case 3:
        {
            delete_product(connection);//to delete the product from products table and drop that product's table
            break;
        }
    }
}
