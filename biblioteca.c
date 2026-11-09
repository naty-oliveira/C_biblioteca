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
//remoção nas listas

//remoção do livro pelo id
void deleteBookId(BookWDog *watcher, int id) {
    BookElement *aux, *ant = NULL;
    for (aux = watcher->first; aux != NULL; aux = aux->next) {
        if (aux->data->id == id) {
            if (ant == NULL)
                watcher->first = aux->next;
            else
                ant->next = aux->next;
            free(aux->data);
            free(aux);
            return;
        }
    ant = aux;
    }
}

//remoção do user pelo email
void deleteUserEmail(UserWDog *watcher, char *email) {
    UserElement *aux, *ant = NULL;
    for (aux = watcher->first; aux != NULL; aux = aux->next) {
        if (strcmp(aux->data->email, email) == 0) {
            if (ant == NULL)
                watcher->first = aux->next;
            else
                ant->next = aux->next;
            free(aux->data);
            free(aux);
            return;
        }
    ant = aux;
    }
}
//prints

void printBook(Book *b) {
    printf("ID: %d, Titulo: %s, Autor: %s, Ano: %d\n",b->id, b->title, b->author, b->year);
    if (b->status == 0)
        printf("Status: Disponivel\n");
    else
        printf("Status: Emprestado %s\n", b->userEmail);
}

void printUser(User *u) {
    printf("Nome: %s, Email: %s\n", u->name, u->email);
}
//buscas, para saber se o livro/usuario existem antes de fazer as funções

//a árvore descarta os ids iguais
Book *buscarBook(BookWDog *books, TreeWDog *ids, int id) {
    BookElement *aux;
    for (aux=books->first; aux!=NULL; aux=aux->next) {
        if(aux->data->id==id)
            return aux->data;
    }
    return NULL;  
}

User *buscarUser(UserWDog *users, char *email) {
    UserElement *aux;
    for (aux=users->first; aux!=NULL; aux=aux->next)
        if (strcmp(aux->data->email, email)==0) //==0 se são iguais
            return aux->data;
    return NULL;
}
//cadastro

void cadastroBook(BookWDog *books, TreeWDog *ids) {
    char title[MAX_STR], author[MAX_STR];
    int year, id;

    id=books->nextId++; //atribui o valor do id e ja incrementa
    printf("Titulo: ");
    scanf("%s", title);
    printf("Autor: ");
    scanf("%s", author);
    printf("Ano de publicacao: ");
    scanf("%d", &year);

    addBookInOrder(books, createBookElement(id, title, author, year));
    ids->raiz = inserirNode(ids->raiz, createnode(id)); //insere e cria um elemento com o id na arvore

    printf("Livro cadastrado com o codigo %d\n", id);
}

void cadastroUser(UserWDog *users) {
    char email[MAX_STR], name[MAX_STR];

    printf("Email: ");
    scanf("%s", email);
    if (buscarUser(users, email)!= NULL) {
        printf("Email ja cadastrado\n");
        return;
    }
    printf("Nome: ");
    scanf("%s", name);

    addUser(users, createUserElement(email, name));
    printf("Usuario cadastrado\n");
}

//Consulta
void consultaBooks(BookWDog *books, TreeWDog *ids) {
    int op, id, achou=0;
    char author[MAX_STR];

    printf("1-Por codigo ou 2-Por autor: ");
    scanf("%d", &op);

    if (op== 1){
        printf("Codigo: ");
        scanf("%d", &id);

        Book *b = buscarBook(books, ids, id);
        if (b == NULL) 
            printf("Livro não encontrado\n");
        else 
            printBook(b);
    }else{
        printf("Autor: ");
        scanf("%s", author);

        BookElement *aux;
        for (aux=books->first; aux!=NULL; aux=aux->next) {
            if (strcmp(aux->data->author, author) == 0){
                printBook(aux->data);
                achou++;
            }
        }
        if (achou==0) 
            printf("Livro não encontrado\n");  
    }
}

void consultaUsers(UserWDog *users) {
    int op,achou= 0;
    char email[MAX_STR], name[MAX_STR];

    printf("1-Por email ou 2-Por nome: ");
    scanf("%d", &op);

    if (op== 1){
        printf("Email: ");
        scanf("%s", email);

        User *u = buscarUser(users, email);
        if (u == NULL) 
            printf("Usuario não cadastrado\n");
        else 
            printUser(u);
    }else{
        if (op== 2){
        printf("Nome: ");
        scanf("%s", name);

        UserElement *aux;
        for (aux=users->first; aux!=NULL; aux=aux->next) {
            if (strcmp(aux->data->name, name) == 0) {
                printUser(aux->data);
                achou++;
            }
        }
        if (achou==0) 
            printf("Usuario não cadastrado\n");
        }
    }
}

void consultarEmprestimos(BookWDog *books, UserWDog *users) {
    char email[MAX_STR];
    int achou=0;

    printf("Email do usuario: ");
    scanf("%s", email);
    if (buscarUser(users, email)==NULL){
        printf("Usuario não cadastrado\n");
        return;
    }
    BookElement *aux;
    for (aux=books->first; aux!=NULL; aux=aux->next) {
        if (aux->data->status == 1 && strcmp(aux->data->userEmail, email)==0){
            printBook(aux->data);
            achou++;
        }
    }
    if (achou==0) 
        printf("Usuario nao possui livros emprestados\n");
}
//atualização

void atualizarBook(BookWDog *books, TreeWDog *ids){
    int id;

    printf("Código do livro: ");
    scanf("%d", &id);

    Book *b=buscarBook(books, ids, id);
    if (b==NULL) {
        printf("Livro não encontrado\n");
        return;
    }
    printf("Novo título: ");
    scanf("%s", b->title);
    printf("Novo autor: ");
    scanf("%s", b->author);
    printf("Novo ano: ");
    scanf("%d", &b->year);
    printf("Livro atualizado\n");
}

void atualizarUser(UserWDog *users){
    char email[MAX_STR];

    printf("Email do usuário: ");
    scanf("%s", email);

    User *u=buscarUser(users, email);
    if (u==NULL) {
        printf("Usuário não cadastrado\n");
        return;
    }
    printf("Novo nome: ");
    scanf("%s", u->name);
    printf("Usuário atualizado\n");
}
//excluir

void removeBook(BookWDog *books, TreeWDog *ids){
    int id;

    printf("Código do livro: ");
    scanf("%d",&id);

    Book *b=buscarBook(books, ids, id);
    if(b==NULL){
        printf("Livro não encontrado. Exclusão não realizada\n");
        return;
    }
    if(b->status==1){
        printf("Livro esta emprestado. Exclusão não realizada\n");
        return;
    }
    deleteBookId(books, id);
    ids->raiz=removeNode(ids->raiz, id);
    printf("Livro excluido\n");
}

//quantos livros o usuário tem emprestados
int emprestimoscount(BookWDog *books, char *email){
    int total=0;
    BookElement *aux;
    for (aux=books->first; aux!=NULL; aux=aux->next)
        if (aux->data->status==1 && strcmp(aux->data->userEmail, email) == 0)
            total++;
    return total;
}

void removeUser(BookWDog *books, UserWDog *users){
    char email[MAX_STR];

    printf("Email do usuario: ");
    scanf("%s",email);

    if(buscarUser(users,email)==NULL){
        printf("Usuário não cadastrado. Exclusão não realizada\n");
        return;
    }
    if(emprestimoscount(books, email)>0){
        printf("Usuário possui livros emprestados. Exclusão não realizada\n");
        return;
    }
    deleteUserEmail(users, email);
    printf("Usuário excluido\n");
}
//emprestimo e devolução

void emprestarBook(BookWDog *books, UserWDog *users, TreeWDog *ids){
    int id;
    char email[MAX_STR];

    printf("Código do livro: ");
    scanf("%d",&id);

    Book *b=buscarBook(books, ids, id);
    if(b==NULL){
        printf("Livro não encontrado\n");
        return;
    }
    if(b->status==1){
        printf("Livro já esta emprestado\n");
        return;
    }
    printf("Email do usuário: ");
    scanf("%s", email);

    if (buscarUser(users,email)==NULL){
        printf("Usuário não cadastrado\n");
        return;
    }
    b->status=1;
    strcpy(b->userEmail, email);
    b->totalemp++;
    printf("Empréstimo realizado\n");
}

void devolverBook(BookWDog *books, TreeWDog *ids){
    int id;

    printf("Código do livro: ");
    scanf("%d", &id);

    Book *b=buscarBook(books,ids,id);
    if(b==NULL){
        printf("Livro não encontrado\n");
        return;
    }
    if(b->status==0){
        printf("Livro não estava emprestado\n");
        return;
    }
    b->status = 0;
    strcpy(b->userEmail,"");
    printf("Devolução realizada\n");
}
//estatísticas

void estatisticas(BookWDog *books, UserWDog *users){
    int usermais=0,quant=0,livromais=0;
    BookElement *b;
    UserElement *u;

    //livro mais emprestado, aparece dois ou mais se tiver empate
    for(b=books->first;b!= NULL;b=b->next){
        if(b->data->totalemp>livromais)
            livromais=b->data->totalemp;
    }
    if(livromais==0)
        printf("Nenhum emprestimo\n");
    else{
        printf("Livro(s) mais emprestado(s) (%d vezes):\n",livromais);
        for (b=books->first; b!=NULL; b=b->next){
            if (b->data->totalemp==livromais)
                printBook(b->data);
        }  
    }
    //usuario com mais emprestimo, aparece mais de um se tiver empate
    for (u=users->first;u!=NULL;u=u->next) {
        quant=emprestimoscount(books, u->data->email);
        if (quant>usermais)
            usermais=quant;
    }
    if(usermais==0)
        printf("Usuario com mais emprestimos ativos: nenhum\n");
    else{
        printf("Usuario(s) com mais emprestimos ativos (%d livros):\n",usermais);
        for(u = users->first; u != NULL; u = u->next){
            if (emprestimoscount(books, u->data->email)==usermais)
                printUser(u->data);
        }  
    }
}
//liberar memória

void freeBooks(BookWDog *watcher) {
    BookElement *aux, *prox;
    for (aux=watcher->first; aux!=NULL; aux=prox) {
        prox=aux->next;      //guarda o próximo antes de apagar
        free(aux->data);
        free(aux);
    }
    free(watcher);
}

void freeUsers(UserWDog *watcher) {
    UserElement *aux, *prox;
    for (aux=watcher->first;aux!=NULL; aux=prox) {
        prox=aux->next;
        free(aux->data);
        free(aux);
    }
    free(watcher);
}

void freeNodes(Node *raiz) {
    if (raiz==NULL) 
        return;
    freeNodes(raiz->left);
    freeNodes(raiz->right);
    free(raiz);
}

void freeTree(TreeWDog *watcher) {
    freeNodes(watcher->raiz);
    free(watcher);
}
//Menus

//Mostra as opções de um submenu e lê a escolha
int subMenu(char *op) {
    int opcao;
    printf("\n%s  0-Voltar: ", op);
    scanf("%d", &opcao);
    return opcao;
}

void cadastroMenu(BookWDog *books, UserWDog *users, TreeWDog *ids){
    int op=subMenu("1-Livros  2-Usuarios");

    while (op!= 0){
        if(op== 1) 
            cadastroBook(books, ids);
        else 
            if(op== 2) 
                cadastroUser(users);
            else 
                printf("Opcao invalida\n");
        op=subMenu("1-Livros  2-Usuarios");
    }
}

void buscarMenu(BookWDog *books, UserWDog *users, TreeWDog *ids){
    int op=subMenu("1-Livros  2-Usuarios  3-Emprestimos");

    while(op!= 0){
        if (op== 1) 
            consultaBooks(books, ids);
        else 
            if(op== 2) 
                consultaUsers(users);
            else 
                if(op== 3) 
                    consultarEmprestimos(books, users);
                else 
                    printf("Opcao invalida\n");
        op=subMenu("1-Livros  2-Usuarios  3-Emprestimos");
    }
}

void atualizarMenu(BookWDog *books, UserWDog *users, TreeWDog *ids){
    int op=subMenu("1-Livros  2-Usuarios");

    while (op!= 0){
        if(op== 1) 
            atualizarBook(books, ids);
        else 
            if(op==2) 
                atualizarUser(users);
            else 
                printf("Opçãoo inválida\n");
        op=subMenu("1-Livros  2-Usuarios");
    }
}

void removerMenu(BookWDog *books, UserWDog *users, TreeWDog *ids){
    int op=subMenu("1-Livros  2-Usuarios");

    while(op!=0){
        if(op==1) 
            removeBook(books, ids);
        else 
            if(op==2) 
                removeUser(books, users);
            else 
                printf("Opcao invalida\n");
        op=subMenu("1-Livros  2-Usuarios");
    }
}

//Mostra o menu principal e lê a opção
int mainMenu() {
    int op;

    printf("\n__________S2_BIBLIOTECA _S2__________\n");
    printf("1-Cadastro\n2-Consulta\n3-Atualização\n4-Exclusão\n");
    printf("5-Empréstimo\n6-Devolução\n7-Estatísticas\n0-Sair\n");
    
    printf("Opção: ");
    scanf("%d", &op);
    return op;
}

int main(){
    BookWDog *books=createBookWatcher();
    UserWDog *users=createUserWatcher();
    TreeWDog *ids=createTreeWatcher();
    int op=mainMenu();

    while (op!= 0) {
        if (op== 1)
            cadastroMenu(books, users, ids);
        else
            if (op== 2)
                buscarMenu(books, users, ids);
            else
                if(op==3)
                    atualizarMenu(books,users,ids);
                else
                    if(op==4)
                        removerMenu(books,users,ids);
                    else
                        if(op==5)
                            emprestarBook(books,users,ids);
                        else
                            if(op==6)
                                devolverBook(books,ids);      
                            else
                                if(op==7)
                                    estatisticas(books,users);
                                else
                                    printf("Opção inválida\n");
        op=mainMenu();
    }
    printf("Acabou!\n");
    freeBooks(books);
    freeUsers(users);
    freeTree(ids);

    return 0;
}