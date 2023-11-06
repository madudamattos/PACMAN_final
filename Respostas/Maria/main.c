#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tMapa.h"
#include "tTunel.h"
#include "tMovimento.h"
#include "tPacman.h"

//funcoes t Fantasma--------------
typedef struct{
    char tipo;
    COMANDO movimento;
    bool status;
    tPosicao *posicao;
    bool comida;
} tFantasma;

tFantasma** CriaFantasmas(tMapa *mapa);
void MoveFantasmas(tMapa *mapa, tFantasma **fantasmas);
bool EncontrouFantasma(tFantasma **fantasmas, tPosicao *posicao);
void DesalocaFantasmas(tFantasma **fantasmas);
COMANDO InverteMovimento(COMANDO comando);

//funcoes tArquivo-------------------------------
void ImprimeMapa(tMapa *mapa, tPacman *pacman, FILE *pArquivo);
void GeraArquivoInicializacao(tMapa *mapa, tPacman *pacman);
void GeraArquivoEstatisticas(tPacman *pacman);
void GeraArquivoResumo(tPacman *pacman);
void AtualizaArquivoSaida(tMapa *mapa, tPacman *pacman, FILE *pFile);
void FinalizaArquivoSaida(tPacman *pacman, FILE *pSaida);
FILE* GeraArquivoSaida();
int PrioridadeDoMovimento(COMANDO comando);
char RetornaTeclaEquivalente(COMANDO comando);

// typedef struct{
//     tMapa *mapa;
//     tPacman	*pacman;
//     tFantasma **fantasmas;
//     tSimbolos identificador;
//     int gameOver; 
// }tJogo;

// typedef struct{
//     char comida;
//     char pacman;
//     char fantasmaP;
//     char fantasmaB;
//     char fantasmaC;
//     char fantasmaI;
//     char portal;
//     char parede;
//     char vazio;
// }tSimbolos;

//funcoes de jogo

int main(int argc, char *argv[]){
    char *caminhoConfig = argv[1];
    char caminhoPastaSaida[100];
    FILE *pSaida = NULL;

    if(argc < 2){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 0;
    }

    sprintf(caminhoPastaSaida, "%s/saida/", caminhoConfig);

    //definicoes iniciais
    tMapa *mapa = CriaMapa(caminhoConfig);
    tPacman *pacman = CriaPacman(ObtemPosicaoItemMapa(mapa, '>'));
    tFantasma **fantasmas = CriaFantasmas(mapa);

    //teste printa fantasma

    // printf("%c\n", fantasmas[0]->tipo);
    // printf("%d\n", fantasmas[0]->status);
    // printf("%d\n", fantasmas[0]->movimento);
    // printf("%d %d\n", fantasmas[0]->posicao->linha,fantasmas[0]->posicao->coluna);

    //arquivo inicializacao e saida
    GeraArquivoInicializacao(mapa, pacman);
    pSaida = GeraArquivoSaida(); 
    AtualizaArquivoSaida(mapa, pacman, pSaida);

    //joga o jogo 

    FinalizaArquivoSaida(pacman, pSaida);
    GeraArquivoEstatisticas(pacman);
    GeraArquivoResumo(pacman);
    SalvaTrilhaPacman(pacman);

    return 0;
}

//funcoes tFantasma--------------------------------

tFantasma** CriaFantasmas(tMapa *mapa){
    tFantasma** fantasmas = calloc(4, sizeof(tFantasma*));
    if(fantasmas == NULL){
        printf("Erro na alocacao do vetor de fantasmas\n");
        exit(1);
    }

    int i;

    for(i=0;i<4;i++){
        fantasmas[i] = calloc(1, sizeof(tFantasma));
        fantasmas[i]->movimento = i;

        if(i == 0) fantasmas[i]->tipo = 'B';
        else if (i == 1) fantasmas[i]->tipo = 'P';
        else if (i == 2) fantasmas[i]->tipo = 'I';
        else fantasmas[i]->tipo = 'C';

        fantasmas[i]->posicao = ObtemPosicaoItemMapa(mapa, fantasmas[i]->tipo);
        if(fantasmas[i]->posicao != NULL){
            fantasmas[i]->status = true;
        }
        else{
            fantasmas[i]->status = false;
        }
        
        fantasmas[i]->comida = false;
    }

    return fantasmas;
}

void MoveFantasmas(tMapa *mapa, tFantasma **fantasmas){
    //prevê a próxima posicao do fantasma
    tPosicao* proximaPosicao = NULL;
    int pI, pJ, i;

    for(i=0; i<4; i++){

        if(fantasmas[i]->status == true){
            pI = ObtemLinhaPosicao(fantasmas[i]->posicao);
            pJ = ObtemColunaPosicao(fantasmas[i]->posicao);
            
            if(fantasmas[i]->movimento == MOV_CIMA){
                proximaPosicao = CriaPosicao(pI - 1, pJ);
            }
            else if(fantasmas[i]->movimento == MOV_BAIXO){
                proximaPosicao = CriaPosicao(pI + 1, pJ);
            }
            else if(fantasmas[i]->movimento == MOV_DIREITA){
                proximaPosicao = CriaPosicao(pI, pJ + 1);
            }
            else if(fantasmas[i]->movimento == MOV_ESQUERDA){
                proximaPosicao = CriaPosicao(pI, pJ - 1);
            }   

            //verifica se a proxima posicao é valida e move 
            if(EncontrouParedeMapa(mapa, proximaPosicao)){
                //inverte o padrao de movimento e ajusta a proxima posicao
                fantasmas[i]->movimento = InverteMovimento(fantasmas[i]->movimento);

                DesalocaPosicao(proximaPosicao);

                if(fantasmas[i]->movimento == MOV_CIMA){
                    proximaPosicao = CriaPosicao(pI - 1, pJ);
                }
                else if(fantasmas[i]->movimento == MOV_BAIXO){
                    proximaPosicao = CriaPosicao(pI + 1, pJ);
                }
                else if(fantasmas[i]->movimento == MOV_DIREITA){
                    proximaPosicao = CriaPosicao(pI, pJ + 1);
                }
                else if(fantasmas[i]->movimento == MOV_ESQUERDA){
                    proximaPosicao = CriaPosicao(pI, pJ - 1);
                }
                
            }

            //volta a comida pra posicao anterior
            if(fantasmas[i]->comida == true){
                AtualizaItemMapa(mapa, fantasmas[i]->posicao, '*');
                fantasmas[i]->comida = false;
            }

            //verifica se a proxima casa que vai tem uma comida
            if(EncontrouComidaMapa(mapa, proximaPosicao)){
                fantasmas[i]->comida = true;
            }

            AtualizaItemMapa(mapa, proximaPosicao, fantasmas[i]->tipo);
            
            DesalocaPosicao(fantasmas[i]->posicao);
            fantasmas[i]->posicao = ClonaPosicao(proximaPosicao);
            DesalocaPosicao(proximaPosicao);
        }

    }

    return;
}

void DesalocaFantasmas(tFantasma **fantasmas){
    int i=0; 

    for(i=0;i<4;i++){
        DesalocaPosicao(fantasmas[i]->posicao);
        free(fantasmas[i]);
    }

    free(fantasmas);
}

bool EncontrouFantasma(tFantasma **fantasmas, tPosicao *posicao){
    int i;

    for(i=0; i<4; i++){
        if(SaoIguaisPosicao(fantasmas[i]->posicao, posicao)){
            return true;
        }
    }

    return false;
}

COMANDO InverteMovimento(COMANDO comando){
    COMANDO movimentoOposto;
    
    if(comando == 1){
        movimentoOposto = 2;
    }
    else if(comando == 2){
        movimentoOposto = 1;
    }
    else if(comando == 3){
        movimentoOposto = 0;
    }
    else if(comando == 0){
        movimentoOposto = 3;
    }
    
    return movimentoOposto;
}

//funcoes tArquivo -------------------------------------

void ImprimeMapa(tMapa *mapa, tPacman *pacman, FILE *pArquivo){
    int L = ObtemNumeroLinhasMapa(mapa);
    int C = ObtemNumeroColunasMapa(mapa);
    tPosicao *posicao = NULL;
    tPosicao *posicaoPacman = ClonaPosicao(ObtemPosicaoPacman(pacman));
    int i, j;

    for(i=0; i<L;i++){
        for(j=0; j<C;j++){
            
            posicao = CriaPosicao(i, j);
            fprintf(pArquivo, "%c", ObtemItemMapa(mapa, posicao));
            DesalocaPosicao(posicao);
        }
        fprintf(pArquivo, "\n");
    }

    DesalocaPosicao(posicaoPacman);
}

void GeraArquivoInicializacao(tMapa *mapa, tPacman *pacman){
    char arquivoInicializacao[100];
    FILE *pInit = NULL;
    
    sprintf(arquivoInicializacao, "inicializacao.txt");

    pInit = fopen(arquivoInicializacao, "w");

    ImprimeMapa(mapa, pacman, pInit);

    tPosicao* posicaoPacman = ObtemPosicaoPacman(pacman);
    int Lp = ObtemLinhaPosicao(posicaoPacman);
    int Cp = ObtemColunaPosicao(posicaoPacman);


    fprintf(pInit, "Pac-Man comecara o jogo na linha %d e coluna %d\n", Lp + 1, Cp + 1);

    fclose(pInit);
}

void GeraArquivoEstatisticas(tPacman *pacman){
    char arquivoEstatisticas[100];

    FILE *pEstatisticas = NULL;

    sprintf(arquivoEstatisticas, "estatisticas.txt");

    pEstatisticas = fopen(arquivoEstatisticas, "w");

    fprintf(pEstatisticas, "Numero de movimentos: %d\n", ObtemNumeroAtualMovimentosPacman(pacman));
    fprintf(pEstatisticas, "Numero de movimentos sem pontuar: %d\n", ObtemNumeroMovimentosSemPontuarPacman(pacman));
    fprintf(pEstatisticas, "Numero de colisoes com parede: %d\n", ObtemNumeroColisoesParedePacman(pacman));
    fprintf(pEstatisticas, "Numero de movimentos para baixo: %d\n", ObtemNumeroMovimentosBaixoPacman(pacman));
    fprintf(pEstatisticas, "Numero de movimentos para cima: %d\n", ObtemNumeroMovimentosCimaPacman(pacman));
    fprintf(pEstatisticas, "Numero de movimentos para esquerda: %d\n", ObtemNumeroMovimentosEsquerdaPacman(pacman));
    fprintf(pEstatisticas, "Numero de movimentos para direita: %d\n", ObtemNumeroMovimentosDireitaPacman(pacman));
    
    fclose(pEstatisticas);

    return;
}

void GeraArquivoResumo(tPacman *pacman){
    char arquivoResumo[100];
    int i;
    FILE *pResumo = NULL;

    sprintf(arquivoResumo, "resumo.txt");

    pResumo = fopen(arquivoResumo, "w");
    
    tMovimento** cloneHistoricoMovimentos = ClonaHistoricoDeMovimentosSignificativosPacman(pacman);

    int nMovSig = ObtemNumeroMovimentosSignificativosPacman(pacman);

    for(i=0; i<nMovSig;i++){

        int numeroMovimento = ObtemNumeroMovimento(cloneHistoricoMovimentos[i]);
        char teclaMovimento = RetornaTeclaEquivalente(ObtemComandoMovimento(cloneHistoricoMovimentos[i]));
        char* acaoMovimento = ObtemAcaoMovimento(cloneHistoricoMovimentos[i]);

        fprintf(pResumo, "Movimento %d (%c) %s\n", numeroMovimento, teclaMovimento, acaoMovimento);
    }

    fclose(pResumo);

    for(i=0; i<nMovSig; i++){
        DesalocaMovimento(cloneHistoricoMovimentos[i]);
    }
    free(cloneHistoricoMovimentos);

    return;
}

FILE* GeraArquivoSaida(){
    FILE *pSaida;
    char arquivoSaida[100];

    sprintf(arquivoSaida, "saida.txt");

    pSaida = fopen(arquivoSaida, "w");

    return pSaida;
}

void AtualizaArquivoSaida(tMapa *mapa, tPacman *pacman, FILE *pFile){
    int i; 
    char placeHolder = 'w';

    fprintf(pFile, "Estado do jogo apos o movimento '%c':\n", placeHolder);
    ImprimeMapa(mapa, pacman, pFile);
    
    int pontuacaoAtual = ObtemNumeroFrutasComidasBaixoPacman(pacman) + ObtemNumeroFrutasComidasCimaPacman(pacman) + ObtemNumeroFrutasComidasDireitaPacman(pacman) + ObtemNumeroFrutasComidasEsquerdaPacman(pacman);

    fprintf(pFile, "Pontuacao: %d\n\n", pontuacaoAtual);
    
    return;
}

void FinalizaArquivoSaida(tPacman *pacman, FILE *pSaida){

    if(1){
        fprintf(pSaida, "Voce venceu!\n");
    }
    else if(0){
        fprintf(pSaida,"Game over!\n");
    }

    int pontuacaoFinal = ObtemNumeroFrutasComidasBaixoPacman(pacman) + ObtemNumeroFrutasComidasCimaPacman(pacman) + ObtemNumeroFrutasComidasDireitaPacman(pacman) + ObtemNumeroFrutasComidasEsquerdaPacman(pacman);

    fprintf(pSaida,"Pontuacao final: %d\n", pontuacaoFinal);

    fclose(pSaida);
}

int PrioridadeDoMovimento(COMANDO comando){
    int prioridade = 0;

    if(comando == 0){
        prioridade = 3;
    }
    else if(comando == 3){
        prioridade = 2;
    }
    else if(comando == 1){
        prioridade = 1;
    }
    else{
        prioridade = 0;
    }

   return prioridade;  
}

char RetornaTeclaEquivalente(COMANDO comando){
    char tecla = '\0';

    switch (comando){
    case 0:
        tecla = 'a';
        break;
    case 1:
        tecla = 'w';
        break;
    case 2:
        tecla = 's';
        break;
    case 3:
        tecla = 'd';
        break;
    default:
        break;
    }

    return tecla;
}

//t----------------------------------------