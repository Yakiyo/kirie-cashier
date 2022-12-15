/*
* C solution of the project, written by
* Look and modified by Kirie.
* This code doesn't belong to me. Credit goes to
* Look and Kirie
*/

#include <stdio.h>
#include <string.h>

/* N_ITEMS: number of items in the list.txt file. */
/* MAX_CART_ITEMS: maximum number of items a customer can buy (add to cart). */
#define N_ITEMS 9
#define MAX_CART_ITEMS 1000

/* i: Variable for handling loop iterations. */
/* input_option: Variable for handling user option input on the main menu. */
/* input_quantity: Variable for handling user input on the quantity of a specific item the customer wants to buy. */
int i, input_option, input_quantity;
/* input_code: Variable for handling item code from list.txt. */
char input_code[100];
/* total_discount: Variable for storing the total discount we are to give to the user by calculating the total_to_pay. */
/* input_paid: Variable for storing the amount customer has paid. */
/* total_to_pay: Variable for storing the total user has to pay (subtotal minus the total_discount). */
/* total_change: Variable for storing the total change to give to the user (total_to_pay minus input_paid). */
float total_discount, input_paid, total_to_pay, total_change;

/* sentinel: Auxiliar variable for breaking out of the while loop. */
char sentinel[6] = "-1";

/* cache_items: Function for passing items from items.txt to variables in memory, so we don't read the file every time we want to check something. */
/* option_1: Function to run the program option 1 as requested. */
void cache_items();
void option_1();

/* Item: Structure of a single item. */
/* Look: I don't know if you're familiar with this but objects in C are really useful for these kind of problems.
 * Structures act as some sort of "custom variables", just the way you have primitives like char, float, int, etc:
 * 
 * float change = 90.0;
 * int quantity = 78;
 * char[30] hello = "hey there!";
 * 
 * With structs you can create your own "custom variable" which we call objects.
 * Imagine if you could simply do:
 * 
 * Item coke = {"C0001", "Coke", 1.40, 0};
 *
 * And then imagine if you could do:
 *
 * printf("Name: %s", coke.name);
 * printf("Code: %s", coke.code);
 * printf("Price: %f", coke.price);
 *
 * And that prints the information you assigned to it.
 * Yeah, those are structures, often called classes/objects and are really useful for stuff like this so you don't have to create a million variables and keep them synced with the same index for iterating over them.
 * Learn struct, it's really good!
 * */
/* Look: Notice on the struct below I didnt include the quantity because quantity is not a property of an item, so I next create a Stock structure which holds this struct inside it and the item quantity too. */
struct Item {
	char code[10];
	char name[100];
	float price;
};

/* stock: Array structure for storing Items and their respective quantities.
 * This acts just like a real stock where you have items in the stock storage room.
 * */
struct Stock {
	struct Item item;
	int quantity;
} stock[N_ITEMS];

/* cart: Array structure for storing Items and their respective quantities.
 * This acts just like a shopping cart where you put items inside it.
 * */
struct Cart {
	struct Item item;
	int quantity;
} cart[MAX_CART_ITEMS];

/* item_null: Object for a null item. */
/* stock_null: Object for a null stock. */
/* Look: These will be useful for functions later because when we define a function that returns Item, we must return an Item object, but if no object matches the criteria of the function and we need to return nothing, we can return these instead. */
struct Item item_null = {"null", "null", 0.0};
struct Stock stock_null = {{"null", "null", 0.0}, 0};

void
cache_items() {
	FILE *item;
  	item = fopen("item.txt", "r");
	i = 0;
	while (!feof(item)) {
    	fscanf(item, "%s %s %f %d", &stock[i].item.code, &stock[i].item.name, &stock[i].item.price, &stock[i].quantity);
    	i++;
  	}
}

struct Stock
get_stock_from_code(char code[]) {
	for (i = 0; i < N_ITEMS; i++) {
		if (!strcmp(code, stock[i].item.code)) {
			return stock[i];
		}
	}
	return stock_null;
}

struct Item
get_item_from_code(char code[]) {
	for (i = 0; i < N_ITEMS; i++) {
		if (!strcmp(code, stock[i].item.code)) {
			return stock[i].item;
		}
	}
	return item_null;
}

int
items_contains_code(char code[]) {
	return strcmp(get_item_from_code(code).code, item_null.code) ? 1 : 0; // ternary operator in C (1 if true, 0 if false)
}

float
get_cart_subtotal() {
	float _subtotal = 0.0;
	for (i = 0; i < MAX_CART_ITEMS; i++) {
		_subtotal += cart[i].item.price * cart[i].quantity;
	}
	return _subtotal;
}

int
get_cart_number_of_items_with_code(char code[]) {
	int _n = 0;
	for (i = 0; i < MAX_CART_ITEMS; i++) {
		if (strcmp(cart[i].item.code, code) == 0) {
			_n += cart[i].quantity;
		}
	}
	return _n;
}

void
option_1() {
	i = 0;
	while (strcmp(input_code, sentinel) != 0) {
		/* 1. Cashier will enter the item code for the item that customer wanted to purchase. */
		printf("Please enter the item code: ");
		scanf("%s", input_code);
		printf("\n");
		/* 4. The cashier will repeat the step 1 - 3 until all the items have been entered to the program, then the cashier will enter '-' to end the process. */
		if (strcmp(input_code, sentinel) == 0) {
			break;
		}
		/* 2. The program will check the item code from the text file, to ensure it is a valid item in the store. If it is a valid item, proceed to step 3, otherwise prompt an error message for the user to enter the item code again. */
		if (!items_contains_code(input_code)) {
			printf("Invalid item code, please try again.\n");
			continue;
		}
		/* 3. The program will ask for the quantity and will calculare the subtotal of the item based on the item price and the quantity of the item. */
		printf("Please enter the quantity: ");
		scanf("%d", &input_quantity);
		printf("\n");
		struct Item _item = get_item_from_code(input_code);
		struct Cart _cart = {_item, input_quantity};
		cart[i] = _cart;
		/* Look: Here we can simply display the price of each item stack the customer has bought, which is simply the price of the item multiplied by the quantity of items. */
		printf("Price: %.2f\n", _cart.item.price * _cart.quantity);
		/* Look: We also display the subtotal which calls the get_cart_subtotal function which is the prices of all items added to the cart up until now, without any discount applied. */
		printf("Subtotal: %.2f\n", get_cart_subtotal());
		i++;
	}
	/* Look: After the while loop gets exited when cashier inputs -1 (when break occurs), we can assume no more items will be added to the cart and apply the discounts accordingly to the cart subtotal. */
	/* 5. Special promotion is available with the following condition: */
	/* Look: Let's initialize a variable to keep track of the total discount amount. It will start at RM0.00 and well be adding each discounted amount to it. */
	total_discount = 0.0f;
	/* 5.1. Every four (4) A0002 purchase. */
	/* Look: The discount for 5.1 is basically dividing the number of A0002 by 4, get the integer part (discard the decimal part) and multiply by 2. Since get_cart_number_of_items_with_code returns an integer, the division by 4 (an integer) will be by default on C also an integer, so we don't need to do anything to get rid of the decimal part because C does it for us already. */
	total_discount += (get_cart_number_of_items_with_code("A0002") / 4) * 2;
	/* 5.3. Combo deal B0001 and B0003. */
	/* Look: Here we have a function to retrieve the number of items inside the cart with a specific code. */
	int _a = get_cart_number_of_items_with_code("B0001");
	int _b = get_cart_number_of_items_with_code("B0003");
	/* Look: This ternary operation gets the minimum value between _a and _b and adds to the total discount. */
	/* Look: If you think enough about it, the minimum between the number of items B0001 and B0003 is exactly what 5.3 wants. */
	total_discount += (_a > _b) ? _b : _a;
	/* Look: Really, learn ternary operators, they are really easy and simple and you save lots of time by using them, instead of writing lines and lines of if/else. You almost never want to use if/else. */
	/* 5.2. Nett purchase more than or equal to RM100. */
	/* Look: 5.2 must be calculated after the discount according to what was requested. */
	total_discount += ((get_cart_subtotal() - total_discount) >= 100) ? 0.15 * get_cart_subtotal() : 0.0;
	/* 6. The program will prompt the payable amount by the customer. */
	/* Look: The total customer needs to pay is the cart subtotal minus the total discount we calculated before. */
	total_to_pay = get_cart_subtotal() - total_discount;
	/* Look: We can print the total amount the customer has to pay. */
	printf("The total is: RM%.2f\n", total_to_pay);
	/* Look: We ask the user for the amount customer has paid. */
	printf("Please enter the amount the customer has paid: ");
    scanf("%f", &input_paid);
	printf("\n");
	/* Look: The change the cashier needs to give back to customer is simply the amount paid by the customer minus the total. */
	total_change = input_paid - total_to_pay;
	/* Look: We display the change the cashier needs to give back to the customer. */
	printf("The change is: RM%.2f\n", total_change);
}

int
main() {
	cache_items();
  	printf("Welcome to MYSTORE\n\n");
  	printf("\t1.Start purchasing\n");
  	printf("\t2.Show available stock\n");
  	printf("\t3.Exit\n");
  	scanf("%d", &input_option);

	switch (input_option) {
		case 1:
			option_1();
			break;
  	}
	return 0;
}
