#include<stdio.h>
#include<string.h>

#define CAPACITY 100
#define BUFFSIZE  20

char *names[CAPACITY];      //Name
char *numbers[CAPACITY];    //Phone

int n=0;                    //number of people

void add();
void find();
void status();
void delete();


int main(){
    char command[BUFFSIZE];

    while(1){
        printf("$ ");
        scanf("%s",command);

        if(strcmp(command,"add") == 0)
            add();
        else if(strcmp(command,"find") == 0)
            find();
        else if(strcmp(command,"status") ==0)
            status();
        else if(strcmp(command,"delete") ==0)
            delete();
        else if(strcmp(command,"exit") ==0)
            break;
        else
            printf("We don't have that operation. please try more.");

    }
    return 0;
}

void add(){
    char buf1[BUFFSIZE], buf2[BUFFSIZE];
    printf("- name: ");
    scanf("%s",buf1);
    printf("- phone number: ");
    scanf("%s",buf2);

    names[n]=strdup(buf1);
    numbers[n]=strdup(buf2);
    n++;

    printf("%s was added successfully.\n",buf1);

}

void find(){
    char buf[BUFFSIZE];
    scanf("%s",buf);

    int i;
    for(i=0;i<n;i++){
        if(strcmp(buf,names[i])==0){
            printf("%s\n",numbers[i]);
            return;
        }
    }
    printf("No person named\n");

}

void status(){
    char buf[BUFFSIZE];
    scanf("%s",buf);

    int i;
    for(i=0;i<n;i++){
        printf("%s: %s\n",names[i],numbers[i]);
    }
    printf("Total %s registered",n);
}

void delete()
{
    char buf[BUFFSIZE];
    scanf("%s",buf);

    int i;
    for(i=0;i<n;i++){
        if(strcmp(buf,names[i])==0) {
            names[i] = names[n - 1];
            numbers[i] = numbers[n - 1];
            n--;
            printf("%s is deleted", buf);
            return;
        }

    }
    printf("No person named '%s' exists.\n",buf);
}
