#include <stdio.h>
#include <string.h>


#define ll long long
#define DELIM " "

typedef struct node
{
	char * dirName;
	struct node *child;
	struct node *sibling;
} node;

node *create_node(char *dir)
{
	node *temp = (node *)malloc(sizeof(node));
	strcpy(temp->dirName, dir);
	temp->child = NULL;
	temp->sibling = NULL;
	return temp;
}



int main()
{
	create_node("/");
	char *dir = 	
	return 0;
}