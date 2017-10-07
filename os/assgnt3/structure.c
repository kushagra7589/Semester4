#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//implementation of a free list as a doubly linked list.

int max_size = 200; // a variable to hold the max size of the address space - that is the max size that is supported by this is memory manager.

typedef struct FreeListNode {
	int status;	//whether free or in allocated, also stores the index if allocated
	int start_addr;		//starting address of the node.. issued when first allocated.
	int end_addr;		//ending address of the node.. issued when first allocated.
	struct FreeListNode *next;		//pointer to the next node in the free list
	struct FreeListNode *prev; 		//pointer to the previous node in the free list
	
} fln;

int getSize(fln *temp) {		//returns the size of node
	return temp->end_addr - temp->start_addr + 1;
}

fln* createNode(int status, int start_addr, int end_addr)
{
	fln *new_node = (fln *)malloc(sizeof(fln));
	new_node->status = status;
	new_node->start_addr = start_addr;
	new_node->end_addr = end_addr;
	// new_node->prev = p;
	new_node->next = NULL;
	return new_node;
}


fln* worst_fit_match(fln *head, int len) {				//returns the position in free list where insertion needs to be done.
	fln *temp = head;
	int curr_size, curr_diff, max_diff = 0;
	fln *insert = NULL;
	while(temp != NULL)
	{
		if(temp->status == -1) 		//if the current node is free
		{
			curr_size = getSize(temp);
			curr_diff = curr_size - len;
			// printf("Size of free node : %d\n", curr_size);
			if(curr_diff >= max_diff) 
			{		//if we find a worse fit than the current fit.
				max_diff = curr_diff;
				insert = temp;
			}
		}
		temp = temp->next;
	}
	return insert;
}

bool can_be_allocated(int end_Addr)
{
	if(end_Addr >= max_size)
		return false;
	return true;
}

void insertNode(fln **where, int status, int len)
{
	fln *new_node = createNode(status, (*where)->end_addr + 1, (*where)->end_addr + len); 
	new_node->next = (*where)->next;
	if(new_node-> next)
		(*where)->next->prev = new_node;
	(*where)->next = new_node;
	new_node->prev = (*where);
}

void split(fln **what)			//splits a free node into an allocated node and a free node
{
	(*what)->start_addr = (*what)->prev->end_addr + 1;
}

void toString(fln *temp)
{
	printf("status - %5d | start_addr = %5d | end_addr = %5d | size = %5d\n", temp->status, temp->start_addr, temp->end_addr, getSize(temp));
}

void print(fln *head)
{
	fln *temp = head;
	while(temp != NULL)
	{
		if(temp->status != -1)
			toString(temp);
		temp = temp->next;
	}
}

void free_coalesce(fln **what)			//free a node and coalesce it with the next and previous if necessary
{
	(*what)->status = -1;
	if((*what)->prev && (*what)->prev->status == -1)
	{
		*what = (*what)->prev;
		(*what)->end_addr = (*what)->next->end_addr;
		fln *temp = (*what)->next;
		(*what)->next = (*what)->next->next;
		if((*what)->next)
			(*what)->next->prev = (*what);
		free(temp);
	}
	if((*what)->next && (*what)->next->status == -1)
	{
		(*what)->end_addr = (*what)->next->end_addr;
		fln *temp = (*what)->next;
		(*what)->next = (*what)->next->next;
		if((*what)->next)
			(*what)->next->prev = (*what);
		free(temp);
	}
}

fln* search(fln *head, int index)			//to search for the node to be freed
{
	fln *temp = head;
	while(temp != NULL && temp->status != index)
		temp = temp->next;
	return temp;
}

int main()
{
	fln *HEAD = NULL;		//initially the free list is empty;
	//for taking input on prompt
	char string[20];
	int len;
	int ptr;

	int index = 1;
	
	printf("%s\n", "This interface implements worst fit algorithm for managing free space");
	while(1)
	{
		printf("%s", "> ");
		scanf("%s", string);

		if(strcmp(string, "malloc") == 0)
		{
			scanf("%d", &len);
			//start checking free list from start and try to see using worst fit algorithm whether there is a space -- SPLITTING OPERATION
			if(len > 0)	
			{
				fln *where = worst_fit_match(HEAD, len);
				int success = 0;
				if(where == NULL)
				{
					fln* END = HEAD;
					while(END && END->next != NULL)
					{
						END = END->next;
					}
					if(END)
					{
						if(END->end_addr + len < max_size)
						{
							insertNode(&END, index, len);
							success = 1;
						}
						else
						{
							printf("%s\n", "Error, cannot allocate so much memory.");
							success = 0;
						}
					}
					else
					{
						if(len < max_size)
						{
							fln *new_node = createNode(index, 0, len-1); 
							new_node->next = HEAD;
							if(HEAD)
								HEAD->prev = new_node;
							new_node->prev = NULL;
							HEAD = new_node;
							success = 1;
						}
						else
						{
							printf("%s\n", "Error, cannot allocate so much memory.");
							success = 0;
						}
					}
				}
				else
				{
					where = where->prev;
					if(where)
					{
						if(can_be_allocated(where->end_addr + len))
						{
							insertNode(&where, index, len);
							if(where->next->next)
								split(&where->next->next);
							success = 1;
						}
						else
						{
							printf("%s\n", "Error, cannot allocate so much memory.");
							success = 0;
						}
					}
					else
					{
						if(len < max_size)
						{
							fln *new_node = createNode(index, 0, len-1); 
							new_node->next = HEAD;
							if(HEAD)
								HEAD->prev = new_node;
							new_node->prev = NULL;
							HEAD = new_node;
							if(HEAD->next)
								split(&HEAD->next);
							success = 1;
						}
						else
						{
							printf("%s\n", "Error, cannot allocate so much memory.");
							success = 0;
						}
					}
				}
				if(success)
					index += 1;
			}	
			else
			{
				printf("%s\n", "Please provide a memory size greater than zero.");
			}
		}
		else if(strcmp(string, "free") == 0)
		{
			scanf("%d", &ptr);
			//go to this position in the free list and set the status to free. If the previous node is free and combine them. if the next node is free then combine them -- COALESCE OPERATION
			fln *what = search(HEAD, ptr);
			if(what)
			{
				free_coalesce(&what);
			}
			else
			{
				printf("%s\n", "This malloc call has already been freed.");
			}
		}
		else if(strcmp(string, "print") == 0)
		{
			print(HEAD);
		}
		else if(strcmp(string, "exit") == 0)
		{
			break;
		}
		else
		{
			printf("Invalid command - %s Please enter a valid command.\n", string);
		}
	}

	return 0;
}