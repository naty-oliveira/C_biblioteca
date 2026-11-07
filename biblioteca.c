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

//arvore de ids (extra), cadastra e verifica o id dos livros
Node *inserirNode(Node *raiz, Node *new) {
    if(raiz==NULL)
        return new;
    if(raiz->id>=new->id)
        raiz->left=inserirNode(raiz->left,new);
    else
        raiz->right=inserirNode(raiz->right,new);
    return raiz;
}

int buscarNode(Node *raiz, int id) {
    if (raiz == NULL) 
        return 0;
    if (id == raiz->id) 
        return 1;
    if (id < raiz->id) 
        return buscarNode(raiz->left, id);
    return buscarNode(raiz->right, id);
}

Node *removeNode(Node *raiz, int id) {
    if (raiz == NULL) 
        return NULL;
    if (id < raiz->id) 
        raiz->left=removeNode(raiz->left, id);
    else{
        if (id > raiz->id) 
            raiz->right=removeNode(raiz->right, id);
        else {
            //nenhum ou um filho
            if(raiz->left==NULL && raiz->right==NULL){ //se for a folha retorna o filho dela(NULL)
                free(raiz);
                return NULL;
            }
            if (raiz->left==NULL) {
                Node *aux =raiz->right;
                free(raiz);
                return aux;
            }
            if (raiz->right==NULL) {
                Node *aux =raiz->left;
                free(raiz);
                return aux;
            }
            //se tiver dois filhos
            Node *aux=raiz->left;
            while(aux->right!=NULL){ //enquanto o filho esquerdo tiver filho direito (maior dos menores)
                aux=aux->right;
            }
            raiz->id=aux->id;
            raiz->left=removeNode(raiz->left,raiz->id); //procura o valor original 
            }
        }
    return raiz;
}
//inserir nas listas

//insere o livro em ordem pelo id, apesar de ja ser sequencial
void addBookInOrder(BookWDog *watcher, BookElement *new){
    if (watcher->first==NULL) {
        watcher->first=new;
        return;
    }
    BookElement *aux, *ant = NULL;
    for (aux = watcher->first;aux != NULL;aux = aux->next){
        if (aux->data->id > new->data->id)
            break;
        ant = aux;
    }
    if (aux==NULL) {
        ant->next=new;
        return;
    }
    new->next=aux;
    if (ant==NULL)
        watcher->first=new;
    else
        ant->next=new;
}

//insere o user no inicio
void addUser(UserWDog *watcher, UserElement *new){
    new->next = watcher->first;
    watcher->first = new;
}


int main(){

    return 0;
}