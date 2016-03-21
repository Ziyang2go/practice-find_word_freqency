#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
/*
Creat a linked list to store the words found in the file, sort the list and display the top n word that appear most. 

*/

#define MAX_SIZE (1024*1024)

typedef struct aa
{
	char *c;
	int len;
	int freq;
	struct aa *next;
}Node;


void add_node(Node **head, Node *new_node)
{
	if(*head == NULL)
	{
		Node *tmp;
		tmp = (Node*)malloc(sizeof(Node));
		tmp->freq = 1;
		tmp->c = new_node->c;
		tmp->len = new_node->len;
		tmp->next = NULL;
		*head = tmp;
		return;
	}
	else 
	{
		if (!strncmp((*head)->c, new_node->c, (*head)->len)&&((*head)->len == new_node->len))
		{	
			(*head)->freq++;
			return;
		}
		else
			add_node(&(*head)->next, new_node);
	}
}

void store_word(Node ** head, char *buff)
{
	if(buff == NULL)
		return;
	int len = strlen(buff);
	Node temp;
	temp.c = buff;
	temp.len = 1;
	for(int i = 0; i< len; ++i)
	{
		if (isspace(buff[i]))
		{
			add_node(head, &temp);
			while(isspace(buff[i]))
				i++;
			temp.c = buff+i;
			temp.len = 1;
		}
		else 
			temp.len ++;
	}		
}
void print_list(Node *head, int n)
{
	if (head == NULL)
		return;
	Node *p;
	p = head;
	for(int i = 0; i< n; i++)
	{
		for(int i = 0; i< p->len; ++i)
			printf("%c", *(p->c++));
		printf("  : %d\n", p->freq);
		p = p->next;
		if (p == NULL)
			break;
	}
	return;
}
void free_list(Node *head)
{
	if(head == NULL)
		return;
	Node *p = head;
	while(p)
	{
		p = p->next;
		free(head);
		head = p;
	}
	return;
}
Node* merge(Node *l1, Node* l2)
{
	if(!l1||!l2)
		return l1?l1:l2;
	Node *l, dummy;
	l = &dummy;
	while(l1&&l2)
	{
		if(l1->freq<l2->freq)
		{
			l->next = l2;
			l2 = l2->next;
		}
		else
		{
			l->next = l1;
			l1 = l1->next;
		}
		l = l->next;
	}
	if(l1)
		l->next =l1;
	if(l2) 
		l->next = l2;
	return dummy.next;
}
Node *sort_list(Node *head)
{
	if( head == NULL||head->next == NULL)
		return head;
	Node *fast, *slow;
	slow = head;
	fast = head->next->next;
	while(fast&&fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	Node *l1, *l2;
	l1 = sort_list(slow->next);
	slow->next =NULL;
	l2 = sort_list(head);
	return merge(l1,l2);
}
int main(int argc, char* argv[])
{

	int fd;
	int ret;
	char buff[MAX_SIZE];
	if(argc < 2)
		return 0;
	if ((fd = open(argv[1], O_RDONLY)) < 0 )
	{
		perror("open");
		exit(1);
	}
	if(( ret = read(fd, (void*)buff, sizeof(buff))) < 0)
	{
		perror("read");
		exit(1);
	}
	close(fd);
	int n;
	n = atoi(argv[2]);
	Node *head;
	head = NULL;
	store_word(&head, buff);
	head = sort_list(head);
	print_list(head,n);
	free_list(head);
	return 0;
}
