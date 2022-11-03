#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//..//
#include "../include/pilhas.h"

void verify_recurrence(struct lists_s *list, char *array, int tam){
    int count = 1;
    for(int i = 0; i < tam; i++){
        if(array[i] == array[i + 1]){
            count++;
        }else{
            list_sorted_insert(list, array[i], count);
            count = 1;
        }
    }
}

void sort(char *array, int tam){
    for(int i = 0; i < tam; i++){
        for(int j = (i + 1); j < tam; j++){
            if(array[i] > array[j]){
                char aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }
}

int main(){
	int fd = open("548279.jpg", O_RDONLY);
	int fd2 = open("arq2", O_WRONLY | O_CREAT, 0764);

	LIST *list;
	list_create(&list);

	ssize_t r;
	char *buff = (char *)malloc(sizeof(char));
	char *array = (char *)malloc(sizeof(char));

	int i = 0;
	while(1){
		r = read(fd, buff, 1);
		if(!r)
			break;
		array = realloc(array, i + 1);
		array[i] = *buff;
		i++;
	}

	sort(array, i);

	verify_recurrence(list, array, i);

	while(1){
		*buff = get_item(list);
		if(*buff == -1)
			break;
		write(fd2, buff, 1);
	}

	close(fd);
	close(fd2);
	return 0;
}
