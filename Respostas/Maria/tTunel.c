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

    tunel->acesso1 = CriaPosicao(linhaAcesso1, colunaAcesso1);
    tunel->acesso2 = CriaPosicao(linhaAcesso2, colunaAcesso2);

    return tunel;
}

bool EntrouTunel(tTunel* tunel, tPosicao* posicao){

    if((posicao->linha == tunel->acesso1->linha && posicao->coluna == tunel->acesso1->coluna) || (posicao->linha == tunel->acesso2->linha && posicao->coluna == tunel->acesso2->coluna)){
        return true;
    }
    else{
        return false;
    }
}

void LevaFinalTunel(tTunel* tunel, tPosicao* posicao){

    if(EntrouTunel(tunel, posicao)){

        if(posicao->linha == tunel->acesso1->linha && posicao->coluna == tunel->acesso1->coluna){
            posicao->linha = tunel->acesso2->linha;
            posicao->coluna = tunel->acesso2->coluna;
        }
        else if(posicao->linha == tunel->acesso2->linha && posicao->coluna == tunel->acesso2->coluna){
            posicao->linha = tunel->acesso1->linha;
            posicao->coluna = tunel->acesso1->coluna;
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