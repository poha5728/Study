#include<stdio.h>
#include<string.h>

#define CAPACITY    100
#define BUFFSIZE    20

char *names[CAPACITY];
char *numbers[CAPACITY];
int n = 0;              // number of people saved

void add();
void find();
void status();
void delete();
void load();
void save();

int main(){
    char buffer[BUFFSIZE];  // 명령어 입력


    while(1){
        printf("$ ");
        scanf("%s",buffer); // 입력받기
        if(strcmp(buffer,"status") == 0)
            status();
        else if(strcmp(buffer,"save") == 0)
            save();
        else if(strcmp(buffer,"load") == 0)
            load();
        else if(strcmp(buffer,"find") == 0)
            find();
        else if(strcmp(buffer,"add") == 0)
            add();
        else if(strcmp(buffer,"delete") == 0)
            delete();
        else if(strcmp(buffer,"exit") ==0)
            break;
        else
            printf("???Try again\n");
    }
    return 0;
}

void add(){
    char buf1[BUFFSIZE], buf2[BUFFSIZE];
    int i=n-1;
    scanf("%s",buf1);
    scanf("%s",buf2);
    /* 정렬까지 생각 */
    while(i>=0&&strcmp(names[i],buf1)>0){
        names[i+1]=names[i];
        numbers[i+1]=numbers[i];
        i--;
    }
    names[i+1]=strdup(buf1);
    numbers[i+1]=strdup(buf2);
    n++;                //회원수 증가

    printf("%s was added successfully.\n",buf1);
}

void status(){
    int i=0;
    for(i=0;i<n;i++){
        printf("%s: %s\n",names[i],numbers[i]);
    }
    printf("Total %d people registered\n",n);
}

void find() {
    char buf[BUFFSIZE];
    scanf("%s", buf);
    int index = search(buf);
    if (index == -1)
        printf("No person named %s exists.\n", buf);
    else
        printf("%s: %s\n", buf, numbers[index]);
}


void delete(){
    char buf[BUFFSIZE];
    int j;

    scanf("%s",buf);
    int index = search(buf);

    if(index==-1){
        printf("No person named '%s' exists.\n",buf);
        return;
    }

    for(j=index;j<n-1;j++){
        names[j]=names[j+1];
        numbers[j]=numbers[j+1];
    }
    n--;
    printf("%s was deleted successfully.\n", buf);


}


void load(){
    char fileName[BUFFSIZE];
    char buf1[BUFFSIZE];
    char buf2[BUFFSIZE];

    printf("Filename: ");
    scanf("%s",fileName);

    FILE *fp = fopen(fileName,"r");
    if(fp == NULL){
        printf("Open failed\n");
        return;
    }else{
        printf("Load success\n");
    }

    while((fscanf(fp,"%s",buf1) != EOF)){
        fscanf(fp,"%s",buf2);
        names[n]=strdup(buf1);
        numbers[n]=strdup(buf2);
        n++;
    }
    fclose(fp);
}

void save(){
    int i;
    char fileName[BUFFSIZE];
    char tmp[BUFFSIZE];

    scanf("%s", tmp);       //"as"
    scanf("%s",fileName);

    FILE *fp = fopen(fileName,"w");
    if(fp==NULL){
        printf("Open failed\n");
        return;
    }
    for(i=0;i<n;i++){
        fprintf(fp,"%s %s\n",names[i],numbers[i]);
    }
    fclose(fp);
}

int search(char *name){
    int i;
    for(i=0;i<n;i++){
        if(strcmp(name,names[i])){
            return i;
        }
    }
    return -1;
}