//
//  main.c
//  assignment
//
//  Created by YOON on 2021/05/20.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//스택 정의.
typedef struct Stack{
    int *arr; //동적할당을 사용할것임.
    int n, top; //자료의 크기와, 스택으로 구현하기 위한 Top인덱스 설정.
}Stack;

//자료형 정의.
typedef struct Queue{
    //S1 = Enqueue작업에 사용될 스택
    //S2 = Dequeue작업에 사용될 스택
    Stack S1,S2;
}Queue;

//스택이 가득 차있는지 확인하는 함수.
int isFull(Stack *st){
    if(st->n == st->top+1)return 1; //가득 차 있는경우 1 반환.
    return 0;
}

//스택이 비어있는지 확인하는 함수.
int isEmpty(Stack *st){
    if(st->top == -1)return 1; //비어있으면 1 반환.
    return 0;
}

//예외처리
int isOverFlow(Stack *En, Stack *De){
    //enqueue작업이 full이면서, dequeue작업이 empty가 아닌 경우 overflow.
    if(isFull(En) && !isEmpty(De)) return 1;
    return 0;
}
int isUnderFlow(Stack *En, Stack *De){
    //작업하려는 큐에 데이터가 없는경우 underflow.
    if(isEmpty(En) && isEmpty(De)) return 1;
    return 0;
}

//스택 초기화
void init_Stack(Stack*st,int n){
    //동적할당(입력받은 크기만큼)
    st->arr=malloc(sizeof(int)*n);
    st->n=n;
    st->top=-1;
    //printf("동적할당 %p\n",st->arr);
    return;
}
//큐 초기화
void init_Queue(Queue *Q,int En_num,int De_num){
    //스택 메쏘드 호출.
    init_Stack(&Q->S1, En_num);
    init_Stack(&Q->S2, De_num);
    return;
}

//삽입
void PUSH(Stack*st, int num){
    //가득 차 있을 경우. overflow 발생
    if(isFull(st)){
        printf("overflow\n");
        return;
    }
    //삽입 O(1) 시간
    st->arr[++st->top]=num;
    return;
}

//출력
int POP(Stack*st){
    //비어있는 경우. underflow발생.
    if(isEmpty(st)){
        printf("underflow\n");
        return -1;
    }
    //추출 O(1) 시간
    return st->arr[st->top--];
}

//스택간의 이동.
int MOVE(Stack*En, Stack*De){
    int cnt=0; //이동횟수를 저장할 변수.
    //비어있는 원소를 옮길경우.
    if(isEmpty(En)){
        printf("Empty Stack\n");
        return -1;
    }
    //옮길 원소가 담을 스택보다 많을 경우
    if(En->top+1 > De->n){
        printf("Lack Stack\n");
        return -1;
    }
    //En에 있던 원소를 De 스택으로 전부 이동.
    while(En->top>=0){
        PUSH(De, POP(En));
        cnt++; //이동횟수 ++
    }
    return cnt; //이동횟수 반환.
}

//enqueue 구현.
void EnQueue(Queue*Q,int num){
    int cnt=0; //이동횟수를 담을 변수 설정.
    //overflow가 일어날 경우. "overflow" 출력.
    if(isOverFlow(&Q->S1, &Q->S2)){
        printf("overflow\n");
        return;
    }
    //삽입하려는 스택이 가득 차 있는 경우, 삽입 스택의 원소를 출력 스택으로 이동.
    if(isFull(&Q->S1)){
        cnt=MOVE(&Q->S1, &Q->S2); //이동했을 경우, 이동횟수를 저장.
    }
    PUSH(&Q->S1, num); //삽입 O(1) 시간.
    printf("+ %d %d\n",num,cnt); // 삽입한 원소와 이동횟수 출력.
    return;
}

//dequeue 구현.
void DeQueue(Queue*Q){
    int cnt=0; //이동횟수를 담을 변수 설정.
    //underflow가 일어날 경우. "underflow" 출력.
    if(isUnderFlow(&Q->S1, &Q->S2)){
        printf("underflow\n");
        return;
    }
    //출력 스택에 원소가 없을 경우, 삽입 스택의 원소를 출력 스택으로 이동.
    if(isEmpty(&Q->S2)){
        cnt=MOVE(&Q->S1, &Q->S2); //이동했을 경우, 이동횟수를 저장.
    }
    printf("- %d %d\n",POP(&Q->S2),cnt);//출력 원소 POP, 이동횟수 출력.
    return;
}

void freeQueue(Queue *Q){
    //printf("동적할당 해제: %p\n",Q->S1.arr);
    //printf("동적할당 해제: %p\n",Q->S2.arr);
    free(Q->S1.arr);
    free(Q->S2.arr);
    return;
}

//문제 구현.
void mkQueue(Queue*Q){
    int n,m; //스택 크기 입력.
    scanf("%d %d%*c",&n,&m);
    //해당 크기로 큐의 스택들을 구현. (동적할당)
    init_Queue(Q, n, m);
    //연산의 개수
    int cnt;
    scanf("%d%*c",&cnt);
    //연산자 및 연산에 사용될 변수 선언.
    char oper;
    int num;
    //연산의 개수만큼 반복.
    for(int i=0;i<cnt;i++){
        //연산자 입력.
        scanf("%c%*c",&oper);
        //해당 연산 실행.
        switch (oper) {
            //EnQueue작업.
            case 'E':
                scanf("%d%*c",&num);
                EnQueue(Q, num);
                break;
            //DeQueue작업.
            case 'D':
                DeQueue(Q);
                break;
            //예외처리.
            default:
                printf("error code!\n");
                break;
        }
    }
    freeQueue(Q);
    return;
}

int main(){
    Queue Q;
    mkQueue(&Q);
}
