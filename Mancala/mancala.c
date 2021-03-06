
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <string.h>
#include<time.h> 

//TODO: Put prototypes
//TODO: make console look good and seem to animate
typedef struct node
{
	int value;
	int position;
	int amount;
	bool isStore;
	struct node *next;

} Node;

//Global Variables
Node * start;
bool isPlayerTurn = true;
bool isAI = false;

Node * newNode(int v, Node *n, int pos, int pebbleAmount, bool store)
{
	Node *p = malloc(sizeof(Node));
	p->value = v;
	p->position = pos;
	p->amount = pebbleAmount;
	p->isStore = store;
	p->next = n;
	return p;
}



Node * join(int value, Node * items, int pos, int pebbleAmount, bool store)
{
	return newNode(value, items, pos, pebbleAmount, store);
}

//TODO: make these constants with capitalization
Node * createBoard()
{
	Node * items = 0;
	
	int startingAmount = 4;
	int startingAmountStore = 0;
	for (int i = 1; i <= 14; i++)
	{
		if (i == 7 || i == 14)
		{
			items = join(i, items, 0, 0, false);
		}
		else
		{
			items = join(i, items, 0, 4, false);
		}
		
		
	}

	

	return items;
}



int getNodePosition(Node *n)
{
	return n->value;
}

int getNodeAmount(Node * n)
{
	return n->amount;
}
Node *getNodeNext(Node *n, int value) 
{ 
	
	for (int i = value; i > 1; i--)
	{
		n = n->next;
	}
	return n;
}

bool getNodeBool(Node * n)
{
	return n->isStore;
}

void setNodeValue(Node *n, int v) 
{ 
	n->value = v; return; 
}

void setNodeAmount(Node * n, int v)
{
	n->amount = v; return;
}

void setNodeNext(Node *n, Node *p) 
{ 
	n->next = p; return; 
}

void setNodeBool(Node * n, bool isStore)
{
	n->isStore = isStore; return;
}
int head(Node *items) 
{ 
	return getNodePosition(items); 
}

int generateRandom()
{
	srand(time(0));
	return 7 + rand() % 6 + 1;
}
Node *tail(Node *items)
{ 
	return getNodeNext(items, 2);
}

Node * reverse(Node *a)
{
	Node *items = 0;
	while (a != 0)
	{
		items = join(head(a), items, 0, a->amount, a->isStore);
		a = tail(a);
	}
	return items;
}
void displayBoard(Node *items)
{
	items = reverse(items);
	printf("\n---------------------------------------------------------------------------------\n");
	printf("\t");
	for (int i = 2; i < 8; i++)
	{
		
		printf("\t%d", getNodePosition(getNodeNext(items, i)));
	}
	printf("\n");
	printf("\t");
	for (int i = 2; i < 8; i++)
	{

		printf("\t{%d}", getNodeAmount(getNodeNext(items, i)));
	}
	printf("\n");
	printf("\t%d {%d}", getNodePosition(getNodeNext(items, 1)), getNodeAmount(getNodeNext(items, 1))); //store
	printf("\t\t\t\t\t\t");
	printf("\t{%d} %d ", getNodeAmount(getNodeNext(items, 8)), getNodePosition(getNodeNext(items, 8))); //store
	printf("\n");
	printf("\t");
	for (int i = 14; i > 8; i--)
	{
		printf("\t{%d}", getNodeAmount(getNodeNext(items, i)));
	}
	printf("\n");
	printf("\t");
	for (int i = 14; i > 8; i--)
	{
		printf("\t%d", getNodePosition(getNodeNext(items, i)));
	}
	printf("\n---------------------------------------------------------------------------------\n");
	printf("\n");
	fflush(stdout);

}

void displayNodeElements(Node * a)
{
	printf("value: %d\namount: %d\nisStore: %s\n", getNodePosition(a), getNodeAmount(a), getNodeBool(a) ? "true" : "false");
}


bool distrubute(Node * items, int amount)
{
	bool repeatTurn = false;
	//pick up all the pebbles
	setNodeAmount(items, 0);

	//then distribute them to the adject holes
	for (int i = amount; i > 0; i--)
	{
		
		//this if makes it a circular linked list
		if (getNodePosition(getNodeNext(items, 1)) == 14)
		{
			items = start;
		}
		else
		{
			items = getNodeNext(items, 2);

			if (isPlayerTurn && getNodePosition(getNodeNext(items, 1)) == 14)
			{
				items = start;
			}
			if (!isPlayerTurn && getNodePosition(getNodeNext(items, 1)) == 7)
			{
				items = getNodeNext(items, 2);
			}
		}
		//this if is used to check if a player repeats a turn
		if (i == 1 && isPlayerTurn && getNodePosition(getNodeNext(items, 1)) == 7)
		{
			
			repeatTurn = true;
		}
		else if (i == 1 && !isPlayerTurn && getNodePosition(getNodeNext(items, 1)) == 14)
		{
			
			repeatTurn = true;
		}
		else
		{
			repeatTurn = false;
		}
		

		setNodeAmount(items, getNodeAmount(items) + 1);
	}
	return repeatTurn;
	
}

bool winCondition(Node * items)
{
	int sum = 0;
	int sum2 = 0;
	for (int i = 1; i < 7; i++)
	{
		sum += getNodeAmount(getNodeNext(items, i));
	}

	for (int i = 13; i > 7; i--)
	{
		sum2 += getNodeAmount(getNodeNext(items, i));
	}

	if (sum == 0)
	{
		setNodeAmount(getNodeNext(items, 14), getNodeAmount(getNodeNext(items, 14)) + sum2);
		for (int i = 13; i > 7; i--)
		{
			setNodeAmount(getNodeNext(items, i), 0);
		}
	}
	if (sum2 == 0)
	{
		setNodeAmount(getNodeNext(items, 7), getNodeAmount(getNodeNext(items, 7)) + sum);
		for (int i = 1; i < 7; i++)
		{
			setNodeAmount(getNodeNext(items, i), 0);
		}
		
	}



	sum = sum + sum2;

	if (sum == 0)
	{
		return true;
	}
	else
	{
		return false;
	}


	
}


void * mancala()
{
	Node * items = reverse(createBoard());
	int input;
	int amount;
	bool repeatTurn = false;
	int choice;
	bool isPlaying = true;
	
	//get 7 and 14 be a store
	setNodeBool(getNodeNext(items, 14), true);
	setNodeBool(getNodeNext(items, 7), true);

	//make sure 1 is the start
	start = getNodeNext(items, 1);

	displayBoard(items);
	
	printf("Would like to play a two player game or with an AI? type 1 for two player, 0 for AI\n");
	scanf("%d\n", &choice);
	if (choice == 1)
	{
		printf("YOU CHOSE TWO PLAYERS!\n\n");
		//Sleep(2000);
	}
	else if (choice == 0)
	{
		printf("YOU CHOSE AI!\n\n");
		//Sleep(2000);
	}
	else
	{
		printf("Invalid Input!\n\n");
		return 0;
	}
	
	Sleep(2000);

	while (isPlaying)
	{
		repeatTurn = false;
		//check win condition
		if (winCondition(items))
		{
			if (getNodeAmount(getNodeNext(items, 7)) > getNodeAmount(getNodeNext(items, 14)))
			{
				printf("PLAYER 1 WIN");
				return 0;
			}
			else if (getNodeAmount(getNodeNext(items, 7)) > getNodeAmount(getNodeNext(items, 14)))
			{
				printf("PLAYER 2 WIN");
				return 0;
			}
			else
			{
				printf("IT'S A DRAW");
				return 0;
			}
		}

		if (isPlayerTurn)
		{
			printf("PLAYER 1 TURN\n");
			printf("Input a number 1-6 for Player One\n");
			printf("Move: ");
			scanf("%d", &input);
		}
		else
		{
			//print dots
			if (choice == 0)
			{
				for (int each = 0; each < 4; ++each)
				{
					fprintf(stdout, "%s ", ".");
					Sleep(250);
					fflush(stdout);
				}
				printf("\n");

				printf("PLAYER 2 TURN\n");
				printf("Input a number 8 - 13 for Player Two\n");
				input = generateRandom();
				while (getNodeAmount(getNodeNext(items, input)) == 0)
				{
					input = generateRandom();
				}

				printf("Move: %d", input);
			}
			else if (choice == 1)
			{
				printf("PLAYER 2 TURN\n");
				printf("Input a number 8 - 13 for Player Two\n");
				printf("Move: ");
				scanf("%d", &input);
			}
			
			
			
			//scanf("%d", &input);
		}
		
		//system("cls");

		if (isPlayerTurn && (input < 1 || input > 6))
		{
			printf("\n\nINVALID INPUT. Input must be between 1 and 6\n");
		}
		
		else if (!isPlayerTurn && (input < 8 || input > 13))
		{
			printf("\n\nINVALID INPUT. Input must be between 8 and 13\n");
		}
		else
		{
			amount = getNodeAmount(getNodeNext(items, input));
			if (amount != 0)
			{
				repeatTurn = distrubute(getNodeNext(items, input), amount);
			}
			else
			{
				printf("\n\nINVALID INPUT. Hole does no contain any pebbles\n");
				repeatTurn = true;
			}
			
			if (!repeatTurn)
			{
				isPlayerTurn = !isPlayerTurn;
			}
			else
			{
				printf("\nREPEAT TURN");
			}
			
		}
		
		
		//system("cls");
		displayBoard(items);
		




		

		
	}

	return 0;
}








int main()
{
	system("cls");
	
	mancala();
	//printf("%d\n", generateRandom());
	//fprintf(stdout, "STARTED!");
	//fflush(stdout);
	//for (int i = 0; i < 10; i++) {
	//	Sleep(500);
	//	fputc('.', stdout);
	//	fflush(stdout);
	//}
	return 0;
}
