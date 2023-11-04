#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tMovimento.h"

/**
 * Dado o número do movimento, o comando e a ação, cria aloca 
 * a estrutura tMovimento e retorna o ponteiro pra ela.
 * \param numeroDoMovimento número do movimento
 * \param comando comando que foi dado para o movimento
 * \param acao a ação gerada pelo movimento
 */
tMovimento* CriaMovimento(int numeroDoMovimento, COMANDO comando, const char* acao){
    tMovimento *movimento = (tMovimento*) calloc(1, sizeof(tMovimento));

    movimento->numeroDoMovimento = numeroDoMovimento;
    movimento->comando = comando;
    strcpy(movimento->acao, acao);
    
    return movimento;
}

int ObtemNumeroMovimento(tMovimento* movimento){
    return movimento->numeroDoMovimento;
}

COMANDO ObtemComandoMovimento(tMovimento* movimento){
    return movimento->comando;
}

char* ObtemAcaoMovimento(tMovimento* movimento){
    return movimento->acao;
}

void DesalocaMovimento(tMovimento* movimento){
    free(movimento);
}
