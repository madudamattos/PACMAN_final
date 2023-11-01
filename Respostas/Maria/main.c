#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tMapa.h"
#include "tTunel.h"
#include "tMovimento.h"
#include "tPacman.h"

int main(int argc, char *argv[]){
    char *caminhoConfig = argv[1];

    if(argc < 2){
        printf("ERRO: O diretorio de arquivos de configuracao nao foi informado\n");
        return 0;
    }

    tMapa *mapa = CriaMapa(caminhoConfig);

    printf("%d", mapa->nMaximoMovimentos);

    int i, j;
    int L = ObtemNumeroLinhasMapa(mapa);
    int C = ObtemNumeroColunasMapa(mapa);

    for(i=0; i<L; i++){
       for(j=0; j<C; i++){
           printf("%c", mapa->grid[i][j]);
       }
       printf("\n");
    }

    return 0;
}