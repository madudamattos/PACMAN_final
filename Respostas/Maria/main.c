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

    tPosicao *posicao = CriaPosicao(1, 2);

    printf("antes pacman\n");

    tPacman *pacman = CriaPacman(posicao);

    printf("depois pacman\n");

    return 0;
}