#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Para strcspm()

#define MAX_TERRITORIOS 5
#define MAX_NOME_STRING 30
#define MAX_COR_STRING 10

struct Territorio
{
    char nome[MAX_NOME_STRING];
    char cor[MAX_COR_STRING];
    int tropas;
};

void limparBufferEntrada()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}



int main()
{
    struct Territorio territorios[MAX_TERRITORIOS];
    int totalTerritorios = 0;
    
    printf("==========================================\n");
    printf("\n- Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n");
    
    do
    {
        printf("\n--- Cadastrando Territorio %d ---\n", totalTerritorios + 1);
        printf("Nome do Territorio: ");
        fgets(territorios[totalTerritorios].nome, MAX_NOME_STRING, stdin);

        printf("Cor do Exercito (ex: Azul, Verde): ");
        fgets(territorios[totalTerritorios].cor, MAX_COR_STRING, stdin);

        territorios[totalTerritorios].nome[strcspn(territorios[totalTerritorios].nome, "\n")] = '\0';
        territorios[totalTerritorios].cor[strcspn(territorios[totalTerritorios].cor, "\n")] = '\0';
     
        printf("Numero de Tropas: ");
        scanf("%d", &territorios[totalTerritorios].tropas);
        limparBufferEntrada();

        totalTerritorios++;
    }while(totalTerritorios < MAX_TERRITORIOS);

    printf("Cadastro inicial concluido com sucesso!\n");
    printf("\n==========================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL");
    printf("\n==========================================\n");
    
    for(int i = 0; i < totalTerritorios; i++)
    {
        printf("\nTERRITORIO %d:\n", i + 1);
        printf("   - Nome: %s:\n", territorios[i].nome);
        printf("   - Dominado por: Exercito %s\n", territorios[i].cor);
        printf("   - Tropas: %d\n", territorios[i].tropas);
    }
    return 0;
}