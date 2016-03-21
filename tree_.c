#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

typedef struct aa
{
	char* ch;
	int freq;
	struct aa *left;
	struct aa* right;
}NODE;

typedef struct bb
{
	char *ch;
	int len;
}WORD;

void addnode(NODE **root, WORD ch)
{
	if(*root == NULL)
	{
		NODE *tmp;
		tmp = (NODE*) malloc(sizeof(NODE));
		tmp->ch = strndup(ch.ch, ch.len);
		tmp->freq = 1;
		*root = tmp;
	}
	else if( strncmp((*root)->ch, ch.ch, ch.len) < 0) 
		addnode(&(*root)->left, ch);
	else if( strncmp((*root)->ch, ch.ch, ch.len) > 0) 
		addnode(&(*root)->right, ch);
	else
		(*root)->freq++;
}
void construct(NODE** root, char* buff)
{
	if(buff == NULL)
		return;
	WORD tmp;
	tmp.ch = buff;
	tmp.len = 1;
	int len = strlen(buff);
	for(int i = 0; i < len; ++i)
	{
		if ( isspace(buff[i]))
		{
			addnode(root, tmp);
			while(isspace(buff[i]))
				i++;
			tmp.ch = buff +i;
			tmp.len = 1;
		}
		else
			tmp.len++;
	}	
}
void print(NODE *root)
{
	if(root == NULL)
		return ;
	print(root->left);
	printf("word: %s, freq: %d\n", root->ch,root->freq);
	print(root->right);
}

int main(int argc, char* argv[])
{
	int fd;
	char buff[8000];
	if(argc < 2)
		return 0;
	if ((fd = open (argv[1], O_RDONLY)) < 0) {
		perror("open\n");
		exit(1);
	}
	int ret;
	if ((ret = read(fd, (void*)buff, sizeof(buff))) < 0) {
		perror("read\n");
		exit(1);
	}
	NODE *root;
	construct(&root, buff);
	print(root);
	return 0;
}
	
