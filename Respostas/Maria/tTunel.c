#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "tTunel.h"
#include "tPosicao.h"


tTunel* CriaTunel(int linhaAcesso1, int colunaAcesso1, int linhaAcesso2, int colunaAcesso2){
    tTunel* tunel = NULL;
    tunel = calloc(1, sizeof(tTunel));

    if(tunel == NULL){
        printf("Erro na alocacao do tunel\n");
        exit(1);
    }

    tunel->acesso1->linha = linhaAcesso1;
    tunel->acesso1->coluna = colunaAcesso1;
    tunel->acesso2->linha = linhaAcesso2;
    tunel->acesso2->coluna = colunaAcesso2;

    return tunel;
}

bool EntrouTunel(tTunel* tunel, tPosicao* posicao){

    if(posicao == tunel->acesso1 || posicao == tunel->acesso2){
        return true;
    }
    else{
        return false;
    }
}

void LevaFinalTunel(tTunel* tunel, tPosicao* posicao){

    if(EntrouTunel(tunel, posicao)){
        if(posicao == tunel->acesso1){
            posicao = tunel->acesso2;
        }
        else if(posicao == tunel->acesso2){
            posicao = tunel->acesso1;
        }
    }
   
}

void DesalocaTunel(tTunel* tunel){

    if(tunel != NULL) {
        DesalocaPosicao(tunel->acesso1);
        DesalocaPosicao(tunel->acesso2);
        free(tunel);
    }

}