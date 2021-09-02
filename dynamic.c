#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct DYNAMIC_LIST_STRUCT 
{
    size_t size;
    void **items;
    size_t item_size;
} dynamic_list_t;

typedef struct PERSON_STRUCT 
{
    char *name;
} person_t;


person_t *init_person(char *name)
{
    person_t *person = calloc(1,sizeof(struct PERSON_STRUCT));
    person->name = calloc(strlen(name) + 1,sizeof(char));
    strcpy(person->name,name);
    return person;
}

void free_person(void *item)
{
    person_t *person = (person_t *)item;
    free(person->name);
    free(person);
}

void dynamic_list_free(dynamic_list_t *list,void (*free_method)(void *x))
{
    for (int i = 0; i < list->size;i++)
        free_method(list->items[i]);
    free(list->items);
    free(list);
}

dynamic_list_t *init_dynamic_list(size_t item_size)
{
    dynamic_list_t *list = calloc(1, sizeof(struct DYNAMIC_LIST_STRUCT));
    list->item_size = item_size;
    list->size = 0;
    list->items = (void *) 0;
    return list;
}

void *dynamic_list_append(dynamic_list_t *list,void *item)
{
    list->size +=1;
    if (list->items == (void *)0)
        list->items = calloc(list->size,list->item_size);
    else 
        list->items = realloc(list->items,list->size * list->item_size);
    

    list->items[list->size - 1] = item;
    return item;
}

void dynamic_list_shit_left(dynamic_list_t *list,int index)
{
    for (int i = index; i < list->size - 1;i++)
        list->items[i] = list->items[i + 1];
    list->size -= 1;
}

void dynamic_list_shift_right(dynamic_list_t *list,int index)
{
    list->size += 1;
    list->items = realloc(list->items,list->size * list->item_size);
    void *next = list->items[index];
    void *temp;
    free_person(list->items[index++]);
    for (int i = index;i < list->size - 1 ; i++) {
        temp = list->items[i];
        list->items[i] = next;
        next = temp;
    }
}

void *dynamic_list_append_byindex(dynamic_list_t *list,void *item ,int index) 
{
    dynamic_list_shift_right(list,index);
    list->items[index] = item;
    return item;
}

void dynamic_list_remove_byindex(dynamic_list_t *list,int index,void (*free_method)(void *))
{
    if (index == -1) 
        return;
    if (free_method)
        free_method(list->items[index]);
    dynamic_list_shit_left(list,index);
    list->items = realloc(list->items,list->size * list->item_size);
}

int dynamic_list_find(dynamic_list_t *list,void *item)
{
   for (int i = 0; i < list->size ; i++)
       if (item == list->items[i]) { 
           return i;
       }
   return -1;
}

void dynamic_list_remove(dynamic_list_t *list,void *item,void (*free_method)(void *x))
{
    dynamic_list_remove_byindex(list,dynamic_list_find(list,item),free_method);
}

int main(int argc,char *argv[])
{
    dynamic_list_t *list = init_dynamic_list(sizeof(struct PERSON_STRUCT*));
    
    dynamic_list_append(list,init_person("parsa"));
    dynamic_list_append(list,init_person("sedna"));
    dynamic_list_append(list,init_person("sednasdfda"));
    dynamic_list_append(list,init_person("kdasdfsedna"));

    //dynamic_list_remove_byindex(list,0,free_person);
    //dynamic_list_shift_right(list,1);
    dynamic_list_append_byindex(list,init_person("ali"),2);

    for (int i = 0; i < list->size;i++)
        printf("%s\n",((person_t*)list->items[i])->name);
    
    return 0;
}
