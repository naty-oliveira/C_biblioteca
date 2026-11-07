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