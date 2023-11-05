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
    COMANDO movimento;
    int status;
    tPosicao *posicao;
} tFantasma;


//funcoes de realizar jogo
tFantasma** CriaFantasmas(tMapa *mapa);
void MoveFantasmas(tMapa *mapa, tFantasma **fantasmas);

//funcoes 
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

//funcoes jogo

tFantasma** CriaFantasmas(tMapa *mapa){
    tFantasma** fantasmas = calloc(4, sizeof(tFantasma));

    //cria B
    fantasmas[0]->tipo = 'B';
    fantasmas[0]->movimento = MOV_ESQUERDA;
    fantasmas[0]->posicao = ObtemPosicaoItemMapa(mapa, fantasmas[0]->tipo);
    if(ObtemPosicaoItemMapa(mapa, 'B') == NULL){
        fantasmas[0]->status = 0;
    }
    else{
        fantasmas[0]->status = 1;
    }

    //cria P
    fantasmas[1]->tipo = 'P';
    fantasmas[1]->movimento = MOV_CIMA;
    fantasmas[1]->posicao = ObtemPosicaoItemMapa(mapa, fantasmas[1]->tipo);
    if(ObtemPosicaoItemMapa(mapa, fantasmas[1]->tipo) == NULL){
        fantasmas[1]->status = 0;
    }
    else{
        fantasmas[1]->status = 1;
    }

    //cria I
    fantasmas[2]->tipo = 'I';
    fantasmas[2]->movimento = MOV_BAIXO;
    fantasmas[2]->posicao = ObtemPosicaoItemMapa(mapa, fantasmas[2]->tipo);
    if(ObtemPosicaoItemMapa(mapa, fantasmas[2]->tipo) == NULL){
        fantasmas[2]->status = 0;
    }
    else{
        fantasmas[2]->status = 1;
    }

    //cria C
    fantasmas[3]->tipo = 'C';
    fantasmas[3]->movimento = MOV_DIREITA;
    fantasmas[3]->posicao = ObtemPosicaoItemMapa(mapa, fantasmas[3]->tipo);
    if(ObtemPosicaoItemMapa(mapa, fantasmas[3]->tipo) == NULL){
        fantasmas[3]->status = 0;
    }
    else{
        fantasmas[3]->status = 1;
    }

    return fantasmas;
}

void MoveFantasmas(tMapa *mapa, tFantasma **fantasmas){
    //prevê a próxima posicao do fantasma
    tPosicao* proximaPosicao = NULL;
    int pI, pJ, i;

    for(i=0; i<4; i++){

        if(fantasmas[i]->status == 1){
            pI = ObtemLinhaPosicao(fantasmas[i]->posicao);
            pJ = ObtemColunaPosicao(fantasmas[i]->posicao);

            DesalocaPosicao(fantasmas[i]->posicao);
            
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