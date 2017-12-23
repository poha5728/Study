/* V3
 * main 단순화
 * 만약 초기 설정 배열 부족할 때를 대비해 reallocate 작업
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INIT_CAPACITY 3
#define BUFSIZE 50

char **names;
char **numbers;

// Function
void reallocate();
void init_directory();
void process_command();
void load(char*);
void add(char*, char*);
void status();
void delete(char*);
void find(char*);
void save(char*);
int read_line(char*, int);
int search(char*);



int capacity = INIT_CAPACITY;
int n = 0;

char delim[]=" "; // 공백으로 자르자


int main(void){
    init_directory();
    process_command();

    return 0;
}

void init_directory(){
    names = (char **)malloc(INIT_CAPACITY*sizeof(char*));
    numbers = (char **)malloc(INIT_CAPACITY*sizeof(char*));
}

int read_line(char str[], int limit){
    int ch, i=0;

    while((ch=getchar()) != '\n'){
           if(i<limit-1) {
               str[i] = ch;  // i번째에 저장하고 i++진행
               i++;
           }

    }
    str[i]='\0';

    return i;
}

void process_command() {
    char command_line[BUFSIZE];
    char *command, *arguement1, *arguement2;

    while (1) {
        printf("$ ");

        if (read_line(command_line, BUFSIZE) <= 0)      // 입력없이 그냥 엔터만 쳤을 때 처리
            continue;
        command = strtok(command_line, delim);      // delim의 문자열 배열에 들어있는 구분자를 이용하여 strtok함수로 command 토큰 만들기
        // 첫 토큰이 command
        if (command == NULL) continue;              // 토큰없으면 계속
        if (strcmp(command, "read") == 0) {             // read 명령이 들어오면
            arguement1 = strtok(NULL, delim);        // 두번째 토큰이 argument1 = file name
            if (arguement1 == NULL) {                    // 두번째 토큰이 있는지 검사
                printf("File name required\n");
                continue;
            }
            load(arguement1);                   //  파일명을 인자로 주어 load 호출
        } else if (strcmp(command, "add") == 0) {
            arguement1 = strtok(NULL, delim);
            arguement2 = strtok(NULL, delim);

            if(arguement1 ==NULL || arguement2 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }

            add(arguement1,arguement2);
            printf("%s was added successfully.\n", arguement1);

        } else if(strcmp(command,"find")==0){
            arguement1 = strtok(NULL,delim);
            if(arguement1 == NULL){
                printf("Invalid arguments.\n");
                continue;
            }
            find(arguement1);
        } else if(strcmp(command,"status")==0){
            status();
        } else if(strcmp(command,"delete")==0){
            arguement1 = strtok(NULL,delim);
            if(arguement1 == NULL){
                printf("Invalid argument.\n");
                continue;
            }
            delete(arguement1);
        } else if(strcmp(command, "save")==0) {
            arguement1 = strtok(NULL, delim);
            arguement2 = strtok(NULL, delim);

            if (arguement1 == NULL || arguement2 == NULL || strcmp("as", arguement1) != 0) {
                printf("Invalid command format.\n");
                if (strcmp(arguement1, "as") != 0) {
                    printf("You shoud 'add as filename\n");
                }
                continue;
            }
            save(arguement2);
        }else if(strcmp(command, "exit")==0){
            break;
        }
    }
}

void load(char *filename){
    char buf1[BUFSIZE];             // Name
    char buf2[BUFSIZE];             // Number

    FILE *fp = fopen(filename, "r");        // Read file
    if(fp==NULL){
        printf("File Error\n");
        return;
    }
    while((fscanf(fp,"%s",buf1) != EOF)){   // fscanf로 파일을 읽어 첫 공백문자 이전을 buf1에(이름)
        fscanf(fp, "%s", buf2);             // 두번째 공백문자까지를 buf2에(전화번호)
        add(buf1, buf2);
    }
    fclose(fp);
}

void add(char *name, char *number){
    if(n>=capacity)
        reallocate();       //배열이 꽉 찼으면 재할당

    int i=n-1;
    while(i>=0 && strcmp(names[i], name)>0){
        names[i+1] = names[i];
        numbers[i+1] = numbers[i];
        i--;
    }
    names[i+1]=strdup(name);
    numbers[i+1]=strdup(number);

    n++;
}

void reallocate(){          // 더 큰 배열을 만들어 원래 값을 거기에 저장
    int i;
    capacity = capacity*2;
    char **tmp1 = (char**)malloc(capacity*sizeof(char *));
    char **tmp2 = (char**)malloc(capacity*sizeof(char *));

    for(i=0; i<n; i++){
        tmp1[i] = names[i];
        tmp2[i] = numbers[i];
    }
    free(names);                // 원래 배열 해제
    free(numbers);

    names = tmp1;               //names(포인터 변수)가 새로운 변수를 가리키게 한다.(중요)
    names = tmp2;               //배열이름은 포인터변수(주소값)

}

void status(){
    int i;
    for(i=0;i<n;i++){
        printf("%s: %s\n",names[i], numbers[i]);
    }
    printf("Total %d people was registered\n",n);
}

void find(char *name){
    int index = search(name);
    if(index==-1)
        printf("No person named '%s' exists.\n", name);
    else
        printf("%s: %s\n", names[index], numbers[index]);
}

int search(char *name){
    int i;
    for(i=0;i<n;i++){
        if(strcmp(name, names[i])==0){
            return i;               // 인덱스 반환
        }
    }
    return -1;          // 없으면 -1 반환
}

void delete(char *name) {
    int i;
    if(i==-1){
        printf("No person names '%s' exists.\n",name);
        return;
    }
    int j=i;
    for(;j<n-1;j++){
        names[j]=names[j+1];
        numbers[j]=numbers[j+1];
    }
    n--;
    printf("'%s' was deleted successfully.\n",name);
}

void save(char *fileName){
    int i;
    FILE *fp = fopen(fileName, "w");
    if(fp==NULL){
        printf("Open failed.\n");
        return;
    }
    for(i=0;i<n;i++){
        fprintf(fp,"%s %s\n",names[i], numbers[i]);
    }
    fclose(fp);
}
