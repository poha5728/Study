
/* V4
 * - 단어 공백 알아서 없애기
 * - 구조체 사용: 전화번호 말고, 이메일과 그룹도 받자
 *
 * - 파일 저장 기능도 넣자
 *   파일형식: 이름# 번호# 메일# 그룹#
 */


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define CAPACITY 100
#define BUFFER_LENGTH 100

// 구조체
typedef struct person{
    char *name;
    char *number;
    char *email;
    char *group;
} Person;

Person directory[CAPACITY];     //Person 타입의 directory 선언

int n;

// 함수 선언
int read_line(FILE*,char*,int);
void load(char*);
void add(char*, char*, char*, char*);
int compose_name(char*, int);
void handle_add(char*);
void save(char*);
int search(char*);
void print_p(Person);
void delete(char*);
void status();
void find(char*);


int main(){
    char command_line[BUFFER_LENGTH];
    char *command, *argument;
    char name_str[BUFFER_LENGTH];           // 이름 제대로 조합시키기

    while(1){
        printf("$ ");
        if(read_line(stdin, command_line, BUFFER_LENGTH) <=0)   // 키보드로 읽는 stdin을 첫 인자로, 입력하지 않았을 때 그냥 다시 프롬프트
            continue;

        command = strtok(command_line, " ");
        if(strcmp(command, "read") ==0){                        // read
            argument = strtok(NULL, " ");
            if(argument == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            load(argument);
        } else if(strcmp(command, "add")==0) {                  // add
            if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            handle_add(name_str);
        } else if(strcmp(command, "find")==0){                  // find
            if(compose_name(name_str, BUFFER_LENGTH) <=0){
                printf("Name required.\n");
                continue;
            }
            find(name_str);
        } else if(strcmp(command, "status") ==0){
            status();
        } else if(strcmp(command, "delete") ==0){
            if(compose_name(name_str, BUFFER_LENGTH) <=0){
                printf("Inavalid arguments.\n");
                continue;
            }
            delete(name_str);
        } else if(strcmp(command, "save") ==0){
            argument = strtok(NULL, " ");
            if(strcmp(argument, "as") !=0){
                printf("Invalid: you need 'as'.\n");
                continue;
            }
            argument = strtok(NULL, " ");
            if(argument == NULL){
                printf("Invalid: you need filename.\n");
                continue;
            }
            save(argument);
        } else if(strcmp(command, "exit")==0){
            break;
        }
    }
    return 0;
}

int read_line(FILE *fp, char str[], int n){
    int ch, i = 0;
    while((ch=fgetc(fp)) != '\n' && ch !=EOF)   //키보드로 읽을 때는 \n, 파일에서 읽을 때는 EOF
        if(i<n)
            str[i++]=ch;
    str[i] = '\0';
    return i;
}

void load(char *fileName){
    char buffer[BUFFER_LENGTH];
    char *name, *number, *email, *group;

    FILE *fp = fopen(fileName, "r");
    if(fp==NULL){
        printf("File Open failed.\n");
        return;
    }

    while(1){
        if(read_line(fp, buffer, BUFFER_LENGTH) <=0)
            break;
        name = strtok(buffer, "#");
        number = strtok(NULL, "#");
        email = strtok(NULL, "#");
        group = strtok(NULL, "#");
        add(name, number, email, group);
    }
    fclose(fp);
}

void add(char *name, char *number, char *email, char *group){
    int i=n-1;
    while(i>=0 && strcmp(directory[i].name, name)>0){
        directory[i+1] = directory[i];      // 구조체로 묶었으므로 이렇게 한방에
        i--;
    }
    directory[i+1].name = strdup(name);
    directory[i+1].number = strdup(number);
    directory[i+1].email = strdup(email);
    directory[i+1].group = strdup(group);

    n++;
}

int compose_name(char str[], int limit){
    char *ptr;
    int length = 0;

    ptr = strtok(NULL, " ");
    if(ptr==NULL)
        return 0;
    strcpy(str, ptr);
    length += strlen(ptr);

    while((ptr = strtok(NULL, " ")) != NULL){
        if(length + strlen(ptr) +1 < limit){
            str[length++] =' ';
            str[length] = '\0';     //strcat를 사용하기 위해
            strcat(str, ptr);       //strcat은 두 인자가 NULL로 끝나야 함
            length += strlen(ptr);
        }
    }
    return length;
}

void handle_add(char *name){
    char number[BUFFER_LENGTH], email[BUFFER_LENGTH], group[BUFFER_LENGTH];
    char empty[]=" ";
    printf("  Phone: ");
    read_line(stdin, number, BUFFER_LENGTH);

    printf("  Email: ");
    read_line(stdin, email, BUFFER_LENGTH);

    printf(" Group: ");
    read_line(stdin, group, BUFFER_LENGTH);

    add(name, (char *)(strlen(number)>0 ? number: empty), (char *)(strlen(email)>0 ? email : empty), (char *)(strlen(group)>0 ? group : empty)); // 입력하지 않으면 공백문자로 대체
}

void save(char *fileName){
    int i;
    FILE *fp = fopen(fileName, "w");
    if(fp==NULL){
        printf("File Open failed.\n");
        return;
    }
    for(i=0; i<n; i++){
        fprintf(fp, "%s#", directory[i].name);
        fprintf(fp, "%s#", directory[i].number);
        fprintf(fp, "%s#", directory[i].email);
        fprintf(fp, "%s#\n", directory[i].group);
    }
    fclose(fp);
}

int search(char *name){
    int i;
    for(i=0; i<n; i++){
        if(strcmp(name, directory[i].name) ==0){
            return i;
        }
    }
    return -1;
}

void print_p(Person p){
    printf("%s: \n", p.name);
    printf("  Phone: %s\n", p.number);
    printf("  Email: %s\n", p.email);
    printf("  Group: %s\n", p.group);
}

void delete(char *name){
    int i = search(name);
    if(i==-1){
        printf("No person named '%s' exists.\n",name);
        return;
    }
    int j=i;
    for(;j<n-1;j++){
        directory[j]=directory[j+1];
    }
    n--;
    printf("%s was deleted. \n",name);
}

void status(){
    int i;
    for(i=0;i<n;i++){
        print_p(directory[i]);
    }
    printf("Total %d persons.\n",n);
}

void find(char *name){
    int index = search(name);
    if(index==-1)
        printf("No person named '%s' exists.\n", name);
    else
        print_p(directory[index]);
}

