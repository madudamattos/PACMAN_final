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
    tMovimento *movimento = (tMovimento*) malloc(sizeof(tMovimento));

    movimento->numeroDoMovimento = numeroDoMovimento;
    movimento->comando = comando;
    strcpy(movimento->acao, acao);
    
    return movimento;
}
/**
 * Retorna o número do movimento
 * \param movimento movimento
 */
int ObtemNumeroMovimento(tMovimento* movimento){
    return movimento->numeroDoMovimento;
}
/**
 * Retorna o comando do movimento
 * \param movimento movimento
 */
COMANDO ObtemComandoMovimento(tMovimento* movimento){
    return movimento->comando;
}

/**
 * Retorna a ação do movimento
 * \param movimento movimento
 */
char* ObtemAcaoMovimento(tMovimento* movimento){
    return movimento->acao;
}
/**
 * Libera o espaço alocado para a estrutura tMovimento
 * 
 * \param movimento movimento
 */
void DesalocaMovimento(tMovimento* movimento){
    free(movimento);
}
