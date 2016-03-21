#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<ctype.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>

typedef struct aa
{
	char *c;
	int len;
	int freq;
}node;

typedef struct bb
{
	char c[8000];
	node *rst;
	int num;
}t_data;

int cmp(const void* a, const void* b)
{
	node* X_A = (node*)a;
	node* X_B = (node*)b;
	return X_B->freq-X_A->freq;
}

node* remove_index(node* a, int index, int len)
{
	if(a == NULL||index > len|| len == 0)
		return a;
	for(int i = index;i < len;++i)
		a[i] = a[i+1];
	return a;
}
void* find_freq(void *str1)
{
	assert(str1);
	char* str;
	t_data *t;
	t = (t_data*)str1;
	str = t->c;
	int len = strlen(str);
	t->rst = (node*)malloc(sizeof(node*)*len);
	t->rst[0].c = str;
	t->rst[0].len = 0;
	for(int i = 0;i < len;++i)
	{
		if (isspace(str[i]))
		{
			t->num++;
			t->rst[t->num].len=1;
			while(isspace(str[i]))
				i++;
			t->rst[t->num].c = str+i;
		}
		else
			t->rst[t->num].len++;
	}
	int j;
	for(int i = 0;i <= t->num;++i)
	{
		t->rst[i].freq = 1;
		j = i+1;
		while(j <= t->num)
		{
			if(!strncmp(t->rst[i].c,t->rst[j].c,t->rst[i].len)&&(t->rst[i].len == t->rst[j].len))  
			{
				t->rst[i].freq++;
				t->rst = remove_index(t->rst,j,t->num);
				t->num--;
			}
			else 
				j++;
		}
	}
	qsort(t->rst,t->num+1,sizeof(node),cmp);
}

int main(int argc, char* argv[])
{
	int fd;
	const int NUM_OF_THREAD = argc-1;
	int ret;
	pthread_t tid[NUM_OF_THREAD];
	t_data t[NUM_OF_THREAD];
	for(int i = 0;i<NUM_OF_THREAD; i++)
	{	if((fd = open(argv[i+1], O_RDONLY)) < 0)
	{
		perror("open \n");
		exit(1);
	}
	if ( (ret = read(fd, (void*)t[i].c, sizeof(t[i].c))) < 0)
	{
		perror("read\n");
		exit(1);
	}
		t[i].num = 0;
		close(fd);
	}
	for(int i = 0;i < NUM_OF_THREAD; i++)
	{

		if((ret = pthread_create(&tid[i],NULL,find_freq, &t[i])) != 0)
		{
			perror("pthread_create");
			exit(1);
		}
	}
	for(int i = 0;i < NUM_OF_THREAD; i++)
	{

		if((ret = pthread_join(tid[i], NULL)) !=0)
		{
			perror("pthread_join");
			exit(1);
		}
	}
	for(int k = 0; k< NUM_OF_THREAD; ++k)
	{
		for(int i=0;i< 20 ;++i)
		{
			for(int j = 0;j<t[k].rst[i].len;++j)
			printf("%c", *(t[k].rst[i].c++));
		printf(" :%d ", t[k].rst[i].freq);
		}
	
	printf("\n");
	free(t[k].rst);
	}
	return 0;
}
