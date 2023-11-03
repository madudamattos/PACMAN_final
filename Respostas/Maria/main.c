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

    printf("antes do cria mapa");
    tMapa *mapa = CriaMapa(caminhoConfig);

    DesalocaMapa(mapa);

    return 0;
}