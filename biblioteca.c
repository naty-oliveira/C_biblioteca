#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR 50

typedef struct {
    int id;
    char title[MAX_STR];
    char author[MAX_STR];
    int year;
    int status;                //0 = disponível, 1 = emprestado
    char userEmail[MAX_STR]; 
    int totalemp; 
} Book;

typedef struct {
    char email[MAX_STR];
    char name[MAX_STR];
} User;

//lista de livros
struct ble{
    Book *data;
    struct ble *next;
};
typedef struct ble BookElement;

typedef struct {
    BookElement *first;
    int nextId; //o id sequencial
} BookWDog;

//lista de usuários
struct ule {
    User *data;
    struct ule *next;
};
typedef struct ule UserElement;

typedef struct {
    UserElement *first;
} UserWDog;

//árvore de id (extra)
typedef struct node {
    int id;
    struct node *left;
    struct node *right;
} Node;

typedef struct {
    Node *raiz;
} TreeWDog;

Node *createnode(int id){
    Node *new=malloc(sizeof(Node));
    new->id=id;
    new->left=NULL;
    new->right=NULL;

    return new;
}

BookWDog *createBookWatcher() {
    BookWDog *new=malloc(sizeof(BookWDog));
    new->first=NULL;
    new->nextId=1; //o primeiro livro é id=1

    return new;
}

UserWDog *createUserWatcher() {
    UserWDog *new=malloc(sizeof(UserWDog));
    new->first=NULL;

    return new;
}

TreeWDog *createTreeWatcher() {
    TreeWDog *new=malloc(sizeof(TreeWDog));
    new->raiz=NULL;

    return new;
}

BookElement *createBookElement(int id, char *title, char *author, int year) {
    BookElement *new=malloc(sizeof(BookElement));
    Book *book=malloc(sizeof(Book));
    book->id=id;
    strcpy(book->title,title);
    strcpy(book->author,author);
    book->year=year;
    book->status=0;   //cadastro sempre começa disponível
    strcpy(book->userEmail,"");
    book->totalemp=0;
    new->data=book;
    new->next=NULL;

    return new;
}

UserElement *createUserElement(char *email, char *name) {
    UserElement *new=malloc(sizeof(UserElement));
    User *user=malloc(sizeof(User));
    strcpy(user->email,email);
    strcpy(user->name,name);
    new->data=user;
    new->next=NULL;

    return new;
}