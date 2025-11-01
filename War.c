#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> //Para strcspm()

#define MAX_TERRITORIOS 6
#define MAX_NOME_STRING 30
#define MAX_COR_STRING 10

typedef struct 
{
    char nome[MAX_NOME_STRING];
    char cor[MAX_COR_STRING];
    int tropas;
    int jaEscolhido; //0 para não 1 para sim

    
}Territorio;

typedef struct 
{
    char territorioASerDominado[MAX_NOME_STRING];
    int totalTerritoriosASerDominados;
    int missao; //Será o número que define a missão 
    char missoaoText[MAX_NOME_STRING];
}Missao;


//O acesso da missão dos territorios será dado pelo index que no caso é o 'totalTerritorios'
Territorio* territorios[MAX_TERRITORIOS];
Missao* missoes[MAX_TERRITORIOS];
int totalTerritorios = 0;

int escolhaMenu = 0;

int atacanteEscolhido = 0;
int defensorEscolhido = 0;
int vitoriaPorMissao = 0;

int territoriosConquistadosIguais = 0;

char territorioVencedor[MAX_NOME_STRING];

void limparBufferEntrada()
{
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void liberarMemoria()
{
    for(int i = 0; i < totalTerritorios; i++)
    {
        if(territorios[i] != NULL)
            free(territorios[i]);

        if(missoes[i] != NULL)
            free(missoes[i]);
    }
}   

int VerificarMissao();
void RegistrarMissao();

void RegistrarTerritorios()
{
    printf("\n==========================================\n");
    printf("\n- Quantos territorios serao.(MIN 2 - MAX 6)\n");
    scanf("%d", &totalTerritorios);
    limparBufferEntrada();
    
    if(totalTerritorios < 2 || totalTerritorios > MAX_TERRITORIOS)
    {
        printf("\nResposta fora do esperado, refaca!\n");
        RegistrarTerritorios();
    }
    else
    {
        liberarMemoria();

        for(int i = 0; i < totalTerritorios; i++)
        {
            territorios[i] = (Territorio*)calloc(1, sizeof(Territorio));
        
            if(territorios[i] == NULL){
                perror("Falha ao colocar memoria de Territorio!");
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("\n==========================================\n");
    printf("\n- Vamos cadastrar os %d territorios iniciais do nosso mundo.\n", totalTerritorios);
    
    for(int i = 0; i < totalTerritorios; i++)
    {
        printf("\n--- Cadastrando Territorio %d ---\n", i + 1);

        //Adiciona nome
        printf("Nome do Territorio: ");
        fgets(territorios[i]->nome, MAX_NOME_STRING, stdin);
        territorios[i]->nome[strcspn(territorios[i]->nome, "\n")] = '\0';

        //Adiciona cor
        printf("Cor do Exercito (ex: Azul, Verde): ");
        fgets(territorios[i]->cor, MAX_COR_STRING, stdin);
        territorios[i]->cor[strcspn(territorios[i]->cor, "\n")] = '\0';
     
        printf("Numero de Tropas: ");
        scanf("%d", &territorios[i]->tropas);
        limparBufferEntrada();
    }

    printf("\nCadastro inicial concluido com sucesso!\n");
    
}

void Mapa()
{
    printf("\n==========================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL");
    printf("\n==========================================\n\n");

    for(int i = 0; i < totalTerritorios; i++)
    {
        printf("%d. %s (Exercito %s, Tropas: %d)\n", i + 1, territorios[i]->nome, territorios[i]->cor, territorios[i]->tropas);
    }
}

void Escolha()
{
    printf("\n=== MENU ====\n");
    printf("1. Atacar\n");
    printf("2. Missoes\n");
    printf("0. Sair\n");
    scanf("%d", &escolhaMenu);
    limparBufferEntrada();
}

void EscolherTerritorio()
{
    for(int i = 0; i < totalTerritorios; i++)
    {
        if(!territorios[i]->jaEscolhido)
            printf("%d. %s\n", i + 1, territorios[i]->nome);
        else
            printf("%d. %s(Ja escolhido )\n", i + 1, territorios[i]->nome);
    }
}

void Defesa()
{
    printf("Escolha o territorio defensor\n");
    EscolherTerritorio();
    scanf("%d", &defensorEscolhido);
    limparBufferEntrada();

    defensorEscolhido--;

    if(defensorEscolhido < 0 || defensorEscolhido > totalTerritorios || territorios[defensorEscolhido]->jaEscolhido)
    {
        printf("\nEscolha fora do esperado ou já feita, refaça\n");
        Defesa();
    }
}

void Ataque()
{
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o territorio atacante\n");
    EscolherTerritorio();
    scanf("%d", &atacanteEscolhido);
    limparBufferEntrada();

    atacanteEscolhido--;
    territorios[atacanteEscolhido]->jaEscolhido = 1;

    if(atacanteEscolhido == -1)
        return;
    else if(atacanteEscolhido < -1 || atacanteEscolhido > totalTerritorios)
    {
        printf("\nEscolha fora do esperado, refaca\n");
        Ataque();
    }

}

void Conquista(Territorio* tDerrotado, Territorio* tVitorioso, int indexDoGanhador)
{
    tDerrotado->tropas--;

    if(tDerrotado->tropas <= 0)
    {
        //Copia a informação(string) do vitorioso
        strcpy(tDerrotado->cor, tVitorioso->cor);
        tDerrotado->tropas = 1;

        printf("\nO territorio %s foi conquistado por %s!\n", tDerrotado->nome, tVitorioso->nome);

        //Se o index do territorio vitorioso seja atacante ou defensor, for de missão
        //de conquista diminui um
        if(missoes[indexDoGanhador]->totalTerritoriosASerDominados > 0)
        {
            missoes[indexDoGanhador]->totalTerritoriosASerDominados--;
        }

        VerificarMissao(indexDoGanhador);
    }
}

void Resultado()
{
    printf("\n--- RESULTADO DA BATALHA ---\n");
    int dadoDoAtacante = rand() % 6 + 1;
    int dadoDoDefensor = rand() % 6 + 1;

    printf("O(a) atacante %s rolou um dado e tirou: %d\n", territorios[atacanteEscolhido]->nome, dadoDoAtacante);
    printf("O(a) defensor(a) %s rolou um dado e tirou: %d\n", territorios[defensorEscolhido]->nome, dadoDoDefensor);
    
    if(dadoDoAtacante > dadoDoDefensor)
    {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa\n");
        Conquista(territorios[defensorEscolhido], territorios[atacanteEscolhido], atacanteEscolhido);
    }
    else if(dadoDoAtacante < dadoDoDefensor)
    {
        printf("VITORIA Da Defesa! O atacante perdeu 1 tropa\n");
        Conquista(territorios[atacanteEscolhido], territorios[defensorEscolhido], defensorEscolhido);
    }
    else if(dadoDoAtacante == dadoDoDefensor)
    {
        printf("EMPATE!\n");
    }

    //Limpeza 
    territorios[atacanteEscolhido]->jaEscolhido = 0;
    atacanteEscolhido = 0;
    defensorEscolhido = 0;
}

int TodosTerritoriosConquistados()
{
    if(totalTerritorios <= 1)
    {
        return 1;
    }

    char *corBase = territorios[0]->cor;

    for(int i = 0; i < totalTerritorios; i++)
    {
        
        //Função strcmp retorna 0 se as funções são iguais
        if(strcmp(corBase, territorios[i]->cor))
        {
            //Se algum território for de cor diferente retorna falso 
            return 0;
        }
    }

    return 1;
    printf("\n!!!TODOS TERRITORIOS CONQUISTADOS!!!\n");
}

void RegistrarMissao()
{
    for(int i = 0; i < totalTerritorios; i++)
    {
        //--- Aloca memoria para missao
        missoes[i] = (Missao*)calloc(1, sizeof(Missao));
    
        if(missoes[i] == NULL){
            perror("Falha ao colocar memoria de Missao!\n");
            exit(EXIT_FAILURE);
        }

        //---
        missoes[i]->missao = rand() % 2 + 1;    
        
        int territorioAlvo = 0;
        
        switch (missoes[i]->missao)
        {
            case 1:
                //Escolhe um territorio sem ser o mesmo
                do
                {
                    territorioAlvo = rand() % totalTerritorios;
                } while (territorioAlvo == i);

                snprintf(missoes[i]->missoaoText, MAX_NOME_STRING,  "Destrua o exercito %s\n", territorios[territorioAlvo]->cor);

                //Copia o nome da cor do exercito alvo
                strcpy(missoes[i]->territorioASerDominado, territorios[territorioAlvo]->cor);
                missoes[i]->territorioASerDominado[strcspn(missoes[i]->territorioASerDominado, "\n")] = '\0';
                break;
            
            case 2:
                missoes[i]->totalTerritoriosASerDominados = rand() % (totalTerritorios / 2) + 1;
                snprintf(missoes[i]->missoaoText, MAX_NOME_STRING, "Conquiste %d territorio(s)\n", missoes[i]->totalTerritoriosASerDominados);
                break;
    
            default:
                perror("\nERRO ao registrar missao!\n");
                break;
        }
    }


}

void MostrarMissoes()
{
    for(int i = 0; i < totalTerritorios; i++)
    {
        if(missoes[i]->missoaoText == NULL){
            perror("\nERROR!!!\n");
        }

        printf("- Missao de %s: %s\n", territorios[i]->nome, missoes[i]->missoaoText);
    }
}

int AjudanteDoVerificarMissao(int index)
{
    char *corBase = missoes[index]->territorioASerDominado;

    switch (missoes[index]->missao)
        {
            case 1:

                for(int i = 0; i < totalTerritorios; i++)
                {
                    
                    //Função strcmp retorna 0(false) se achar a cor nos nós exercitos 
                    //Se não achar a cor do exercito retorna 1(true) 
                    if(strcmp(corBase, territorios[i]->cor))
                    {
                        printf("\nMissao completa! %s Exterminou o exercito alvo!\n", territorios[index]->nome);
                        strcpy(territorioVencedor, territorios[index]->cor);
                        return vitoriaPorMissao = 1;
                    }
                }
                break;
            
            case 2:
                if(missoes[index]->totalTerritoriosASerDominados == 0)
                {
                    printf("\nMissao completa! %s conquistou todos territorios necesssarios!\n", territorios[index]->nome);
                    strcpy(territorioVencedor, territorios[index]->cor);
                    return vitoriaPorMissao = 1;
                }
                break;
        }
        
        //retorna false por padrão
        return 0;
}

//Verifica se algum terrritorio concluiu sua missão designada
int VerificarMissao(int indexDoUltimoAGanharTerritorio)
{
    //Verifica se o ultimo a ganhar territorio concluiu sua missao
    if(AjudanteDoVerificarMissao(indexDoUltimoAGanharTerritorio)){
        return 1;
    }

    //Se não passará por todos os territorios e verificará se ele por
    //acidente concluiu de outro 
    for(int i = 0; i < totalTerritorios; i++)
    {
        if(AjudanteDoVerificarMissao(i)){
            return 1;
        }
    }

    //retorna false por padrão
    return 0;
}

int main()
{
    srand(time(0));

    RegistrarTerritorios();
    RegistrarMissao();
    do
    {
        Mapa();
        Escolha();

        switch (escolhaMenu)
        {
            case 1:
                Ataque();
                Defesa();
                Resultado();
                break;

            case 2:
                MostrarMissoes();
                break;

            case 0:
                printf("\n!!!SAIR!!!\n");
                break;
        }

    }while(!TodosTerritoriosConquistados() && !vitoriaPorMissao && escolhaMenu != 0);

    if(TodosTerritoriosConquistados() && !vitoriaPorMissao){
        printf("\n!!!Parabens Vitoria do Exercito: %s\n", territorios[0]->cor);
    }
    else if(vitoriaPorMissao && !TodosTerritoriosConquistados()){
        printf("\n!!!Exercito %s cocluiu sua missao. Parabens pela vitoria!!!", territorioVencedor);
    }
    else if(vitoriaPorMissao && TodosTerritoriosConquistados())
    {
        printf("\n!!!Exercito %s cocluiu sua missao e conquistou todos territorios. Parabens pela vitoria!!!", territorioVencedor);

    }
    
    liberarMemoria();
    return 0;
}