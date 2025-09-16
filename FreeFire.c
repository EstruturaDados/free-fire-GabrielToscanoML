#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definindo constantes para controlar alguns atributos
#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20
#define MAX_QUANTIDADE 64

// Declarando uma estrutura do tipo Item, onde cada item tem um nome, tipo e quantidade
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_NOME];
    int quantidade;
} Item;

// Declarando uma No para criar uma lista encadeada que vai representar o Inventário do jogador
typedef struct No {
    Item *item;
    struct No *proximo;
} No;

// Criando um "apelido" para a lista encadeada para evitar ficar escrevendo toda vez que eu for criar um 'Nó' novo
typedef No *ListaEncadeada;

// Funções auxiliares para manipulação da lista encadeada (inventário)
void inicializarInventario(ListaEncadeada *inventario);
void inserirItemNoInventario(ListaEncadeada *inventario, Item *item);
void removerItemDoInventario(ListaEncadeada *inventario, char *nome);
void listarItensDoInventario(ListaEncadeada *inventario);
void buscarItemPeloNome(ListaEncadeada *inventario, char *nome);
void liberarMemoriaDoInventario(ListaEncadeada *inventario);

// funções auxiliares para deixar o código mais limpo
int cadastrarItem(Item *item);
int contarItens(ListaEncadeada inventario);

// declarações de funções para tratar erros
int validaQuantidade(int quantidade);

// declaração da função do menu
void exibirMenu(int *opcao);

int main() {
    int opcao;
    ListaEncadeada inventario;
    inicializarInventario(&inventario);

    do {
        exibirMenu(&opcao);
        switch (opcao)
        {
        case 1: {
            Item *item = malloc(sizeof(Item));
            if(!cadastrarItem(item)) {
                printf("Erro ao cadastrar item. Tente novamente.\n");
                free(item);
                break;
            }
            inserirItemNoInventario(&inventario, item);
            break;
        }
        case 2:
            printf("Digite o nome do item a ser removido: \n");
            char nome[MAX_NOME];
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';
            removerItemDoInventario(&inventario, nome);
            break;
        case 3:
            listarItensDoInventario(&inventario);
            break;
        case 4:
            printf("Digite o nome do item a ser procurado: \n");
            char termo[MAX_NOME];
            fgets(termo, sizeof(termo), stdin);
            termo[strcspn(termo, "\n")] = '\0';
            buscarItemPeloNome(&inventario, termo);
            break;
        case 0:
            liberarMemoriaDoInventario(&inventario);
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opção inválida. Tente novamente.\n");
            break;
        }
    } while(opcao != 0);

    return 0;
}

void exibirMenu(int *opcao) {
    printf("\nMenu Principal:\n");
    printf("Escolha uma opção usando os números de 0 a 4\n");
    printf("1. Adicionar um item\n");
    printf("2. Remover um item\n");
    printf("3. Listar todos os itens\n");
    printf("4. Buscar item pelo nome\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", opcao);
    getchar(); // solução temporária. Pois não impede o usuário de digitar algo como "çdksjalk"
}

void inicializarInventario(ListaEncadeada *inventario) {
    *inventario = NULL;
}

int contarItens(ListaEncadeada inventario) {
    int count = 0;
    No *atual = inventario;
    while (atual != NULL) {
        count++;
        atual = atual->proximo;
    }
    return count;
}


// retorna 0 quando der problema e 1 quando estiver tudo certo
int validaQuantidade(int quantidade) {
    if (quantidade > MAX_QUANTIDADE) {
        printf("Erro: Quantidade excede o limite máximo.\n");
        return 0;
    }

    if (quantidade < 1) {
        printf("Erro: Quantidade inválida. Deve ser entre 1 e %d.\n", MAX_QUANTIDADE);
        return 0;
    }

    return 1;
}

int cadastrarItem(Item *item) {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    char buffer[10]; // tratar o input do usuario na quantidade de itens

    printf("\nCadastrando novo item\n\n");

    printf("Digite o nome do item: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; // Remover o caractere de nova linha
    strcpy(item->nome, nome); // atribuindo o nome ao item

    printf("\nDigite o tipo do item: ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = '\0'; // Remover o caractere de nova linha
    strcpy(item->tipo, tipo); // atribuindo o tipo ao item

    printf("\nDigite a quantidade do item: (máximo 64) ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &quantidade);

    if (!validaQuantidade(quantidade)) {
        printf("Quantidade inválida!\n");
        return 0;
    }

    item->quantidade = quantidade;
    return 1;
}

void inserirItemNoInventario(ListaEncadeada *inventario, Item *item) {
    if (contarItens(*inventario) >= MAX_ITENS) {
        printf("Erro: Inventário cheio.\n");
        return;
    }

    No *novoNo = (No *)malloc(sizeof(No));

    novoNo->item = malloc(sizeof(Item));
    *novoNo->item = *item; // copia os dados

    novoNo->proximo = *inventario;
    *inventario = novoNo;

    printf("Item \"%s\" cadastrado com sucesso!\n", item->nome);
}

void removerItemDoInventario(ListaEncadeada *inventario, char *nome) {
    No *atual = *inventario;
    No *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->item->nome, nome) == 0) {
            if (anterior == NULL) {
                *inventario = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual->item);
            free(atual);
            printf("Item \"%s\" removido com sucesso!\n", nome);
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    printf("Item \"%s\" não encontrado no inventário.\n", nome);
}

void listarItensDoInventario(ListaEncadeada *inventario) {
    No *atual = *inventario;

    printf("\nInventário:\n");
    printf("--------------------------------------------------\n");
    printf("| %-20s | %-10s | %-10s |\n", "Nome", "Tipo", "Quantidade");
    printf("--------------------------------------------------\n");

    while (atual != NULL) {
        printf("| %-20s | %-10s | %-10d |\n", atual->item->nome, atual->item->tipo, atual->item->quantidade);
        atual = atual->proximo;
    }

    printf("--------------------------------------------------\n");
}

void buscarItemPeloNome(ListaEncadeada *inventario, char *nome) {
    No *atual = *inventario;

    while (atual != NULL) {
        if (strcmp(atual->item->nome, nome) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\n", atual->item->nome);
            printf("Tipo: %s\n", atual->item->tipo);
            printf("Quantidade: %d\n", atual->item->quantidade);
            return;
        }
        atual = atual->proximo;
    }

    printf("Item \"%s\" não encontrado no inventário.\n", nome);
}

void liberarMemoriaDoInventario(ListaEncadeada *inventario) {
    No *atual = *inventario;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual->item);
        free(atual);
        atual = proximo;
    }

    *inventario = NULL;
}
