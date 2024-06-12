#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char description[256];
    char due_date[11];
    int completed;



}Task;

void add_tasks (Task **tasks, int *count, int *capacity ){
    if(*count == *capacity){
        *capacity = (*capacity == 0) ? 1 : (*capacity *=2);
        *tasks = realloc(*tasks, *capacity * sizeof(Task));
        if (*tasks == NULL){
            printf("reallocation faield (add_task).");
            exit(1);
        }
    }

    printf("Enter a description: ");
    getchar();
    fgets((*tasks)[*count].description, 256, stdin);
    (*tasks)[*count].description[strcspn((*tasks)[*count].description, "\n")]=0;

    printf("Enter a due date: ");
    scanf("%s", (*tasks)[*count].due_date);

    (*tasks)[*count].completed = 0;
    (*count)++;
}
void view_tasks (Task *tasks, int count ){
    
    for(int i = 0; i<count; i++){
        printf("%d. [%s] %s - Due: %s\n", i + 1, tasks[i].completed ? "X" : " ", tasks[i].description, tasks[i].due_date);

    }

}
void complete_tasks (Task *tasks, int count ){
    int task_num;
    printf("Enter a task num:");
    scanf("%d", &task_num);

    if(task_num>0 && task_num<=count){
        tasks[task_num - 1].completed = 1;

    }
    else{
    printf("Invalid task num.\n");
    }
}
void delete_tasks (Task **tasks, int *count, int *capacity ){
    int task_num;
    printf("Enter a task num you want to del: ");
    scanf("%d", &task_num);

    if(task_num>0 && task_num<=*count){
        for(int i = task_num - 1; i<*count;i++){
           (*tasks)[i]=(*tasks)[i+1];
        }
        (*count)--;

         if(*count <= *capacity/4 && *capacity>1){
        *capacity /= 2;
        *tasks = realloc(*tasks, *capacity * sizeof(Task));

    }
    if(*tasks == NULL && *count>0){
        printf("Failed to allocate memmory (del)");
        exit(1);
    }

    }
    else{
        printf("invalied task number.");
    }
   

}
void save_tasks (Task *tasks, int count ){
    FILE *file = fopen("MyProject.txt", "w");
    if(file == NULL){
        printf("Failed to write the file.");
        exit(1);

    }
    for(int i = 0; i<count; i++){
        fprintf(file, "%s;%s;%d\n", tasks[i].description, tasks[i].due_date, tasks[i].completed);

    }
    fclose(file);
}
void load_tasks (Task **tasks, int *count, int *capacity ){
    FILE *file = fopen("MyProject.txt", "r");
    if(file = NULL){
        printf("Failed to read the file.");
        return;

    }
    *capacity = 10;
    *tasks = malloc(*capacity * sizeof(Task));
    if(*tasks == NULL){
        printf("failedto allocate memmory (load).");
        exit(1);
    }

    while (fscanf(file, "%[^;];%[^;];%d\n", (*tasks)[*count].description, (*tasks)[*count].due_date, &(*tasks)[*count].completed) != EOF){
        (*count)++;
        if(*count == *capacity){
            (*capacity) *=2;
            *tasks = realloc(*tasks, *capacity * sizeof(Task));
            if(*tasks == NULL){
                printf("failed to allocate memory (load-realloc)");
                exit(1);
            }
        }
    }
    fclose(file);
    

}



int main(){
    Task *tasks = NULL;
    int count = 0;
    int capacity = 0;
    load_tasks(&tasks, &count, &capacity);

    int choice;

    do{
         printf("\nTask Manager\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Complete Task\n");
        printf("4. Delete Task\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_tasks(&tasks, &count, &capacity);
                break;
            case 2:
                view_tasks(tasks, count);
                break;
            case 3:
                complete_tasks(tasks, count);
                break;
            case 4:
                delete_tasks(&tasks, &count, &capacity);
                break;
            case 5:
                save_tasks(tasks, count);
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 5);

    free(tasks);

     return 0;
}





