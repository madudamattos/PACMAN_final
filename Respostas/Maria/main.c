#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tMapa.h"
#include "tTunel.h"
#include "tMovimento.h"
#include "tPacman.h"

typedef struct{
    char tipo;
    char movimento;
    int status;
    tPosicao posicaoFantasma;
} tGhost;

void ImprimeMapa(tMapa *mapa, tPacman *pacman, FILE *pArquivo);

//funcoes de geracao de arquivo
void GeraArquivoInicializacao(tMapa *mapa, tPacman *pacman);
void GeraArquivoEstatisticas(tPacman *pacman);
void GeraArquivoResumo(tPacman *pacman);
void AtualizaArquivoSaida(tMapa *mapa, tPacman *pacman, FILE *pFile);
void FinalizaArquivoSaida(tPacman *pacman, FILE *pSaida);
FILE* GeraArquivoSaida();

//funcoes auxiliares
COMANDO InverteMovimento(COMANDO comando);
int PrioridadeDoMovimento(COMANDO comando);
char RetornaTeclaEquivalente(COMANDO comando);


int main(int argc, char *argv[]){
    char *caminhoConfig = argv[1];
    char caminhoPastaSaida[100];
    FILE *pSaida = NULL;

    if(argc < 2){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 0;
    }

    sprintf(caminhoPastaSaida, "%s/saida/", caminhoConfig);

    tMapa *mapa = CriaMapa(caminhoConfig);

    tPacman *pacman = CriaPacman(ObtemPosicaoItemMapa(mapa, '>'));
 
    
    //arquivo saida
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

void ImprimeMapa(tMapa *mapa, tPacman *pacman, FILE *pArquivo){
    int L = ObtemNumeroLinhasMapa(mapa);
    int C = ObtemNumeroColunasMapa(mapa);
    tPosicao *posicao = NULL;
    tPosicao *posicaoPacman = ClonaPosicao(ObtemPosicaoPacman(pacman));
    int i, j;

    for(i=0; i<L;i++){
        for(j=0; j<C;j++){
            posicao = CriaPosicao(i, j);
            if(SaoIguaisPosicao(posicao, posicaoPacman)){
                fprintf(pArquivo, ">");
            }
            else{
                fprintf(pArquivo, "%c", ObtemItemMapa(mapa, posicao));
            }
            DesalocaPosicao(posicao);
        }
        fprintf(pArquivo, "\n");
    }

    DesalocaPosicao(posicaoPacman);
}

//funcoes de geracao de arquivo

void GeraArquivoInicializacao(tMapa *mapa, tPacman *pacman){
    char arquivoInicializacao[100];
    FILE *pInit = NULL;
    
    sprintf(arquivoInicializacao, "inicializacao.txt");

    printf("6");
    pInit = fopen(arquivoInicializacao, "w");

    printf("7");
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

// typedef struct{
//     COMANDO MOVIMENTO; 
//     int frutas;
//     int colisoes;
//     int usos;
//     int posicao;
// } tRanking;

// void generateRankingFile(tPacman pacman){
//     FILE *pRank = NULL;
//     char arquivoRanking[100];
//     int i, j, k;
//     int best_score, best_j;
//     int class[4];
//     int best=-1;

//     sprintf(arquivoRanking, "ranking.txt");

//     pRank = fopen(arquivoRanking, "w");
    
//     game.rank[0].type = 'w';
//     game.rank[1].type = 'a';
//     game.rank[2].type = 's';
//     game.rank[3].type = 'd';

//     //zera todos os movimentos no começo
//     for(i=0;i<4;i++){
//         game.rank[i].frutas=0;
//         game.rank[i].colisoes=0;
//         game.rank[i].usos=0;
//         game.rank[i].posicao= -1;
//         class[i] = -1;
//     }

//     //inicializa todos os movimentos
//     for(j=1;j<=game.pacman.moveCounter;j++){
//         for(i=0;i<4;i++){
//             if(game.pacman.moves[j].moveInput == game.rank[i].type){
//                 game.rank[i].usos++;
//                 if(game.pacman.moves[j].moveResult == GOTfrutas){
//                     game.rank[i].frutas++;
//                 }
//                 else if(game.pacman.moves[j].moveResult == WALLCOLISION){
//                     game.rank[i].colisoes++;
//                 }
//             }
//         }
//     }

//     //aqui começa a logica de organização do ranking/////////////////

//     // acha o maior rank e coloca no 0, ate fechar o vetor
//     for(i=0;i<4;i++){
//         best_score = -1;
//         best_j = -1;
            
//         for(j=0;j<4;j++){
        
//             if(game.rank[j].posicao == -1){
//                 if(game.rank[j].frutas > best_score){
//                     best_j = j;
//                     best_score = game.rank[j].frutas;
//                 }
//                 else if(game.rank[j].frutas == best_score){
//                     //se for igual vai comparar a colisao nas paredes
//                     if(game.rank[j].colisoes < game.rank[best_j].colisoes){
//                         best_j = j;
//                     }
//                     else if(game.rank[j].colisoes == game.rank[best_j].colisoes){
//                         //se for igual vai comparar o uso
//                         if(game.rank[j].usos > game.rank[best_j].usos){
//                             best_j = j;
//                         }
//                         else if(game.rank[j].usos == game.rank[best_j].usos){
//                             //se for igual vai olhar a ordem alfabetica
//                             if(letterPriority(game.rank[j].type)> letterPriority(game.rank[best_j].type)){
//                                 best_j = j;
//                             }
        
//                         }
//                     }
//                 }
//             }
//         }
        
//         game.rank[best_j].posicao = i;
        
//     }
    
//     ///aqui termina a organização lógica/////////////////////////////
    
//     //imprime o ranking
    
//     for(i=0;i<4;i++){
//         for(j=0;j<4;j++){
//             if(game.rank[j].posicao == i){
//                 fprintf(pRank, "%c,%d,%d,%d\n", game.rank[j].type, game.rank[j].frutas, game.rank[j].colisoes, game.rank[j].usos);
//             }
//         }
//     }
   
//     fclose(pRank);

//     return;
// }

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

//funcoes auxiliares ------------------------------------------------

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