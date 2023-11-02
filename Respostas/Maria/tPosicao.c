#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tPosicao.h"

/**
 * Cria a posição dinamicamente.
 * Dados os valores de linha e coluna, 
 * aloca dinamicamente o tipo tPosicao e retorna o ponteiro.
 * \param linha linha da posição
 * \param coluna coluna da posição
 */
tPosicao* CriaPosicao(int linha, int coluna){
    tPosicao* posicao = (tPosicao*) calloc (1,sizeof(tPosicao));
   if(posicao == NULL){
        printf("Erro na alocacao de memória da posicao!");
    }

    posicao->linha = linha;
    posicao->coluna = coluna;

    return posicao;
}

/**
 * Clona a posição (construtor de cópia)
 * Aloca outra posicao com as informaçoes da original (passada como parâmetro).
 * \param posicao posição
 */
tPosicao* ClonaPosicao(tPosicao* posicao){
    tPosicao* posicaoClone = (tPosicao*) malloc (sizeof(tPosicao));
    if(posicaoClone == NULL){
        printf("Erro na alocacao de memória da posicao!");
    }

    *posicaoClone = *posicao;

    return posicaoClone;
}

int ObtemLinhaPosicao(tPosicao* posicao){
    return posicao->linha;
}

int ObtemColunaPosicao(tPosicao* posicao){
    return posicao->coluna;
}
/**
 * Atualiza a posição a atual por uma nova posição.
 * Dada a posição atual, atualiza as informações da posição 
 * nova com a da posição atual.
 * \param posicaoAtual posição atual
 * \param posicaoNova posição nova
 */
void AtualizaPosicao(tPosicao* posicaoAtual, tPosicao* posicaoNova){
    posicaoAtual->linha = posicaoNova->linha;
    posicaoAtual->coluna = posicaoNova->coluna;
}

/**
 * Verifica se 2 posições são iguais comparando seus atributos.
 * Retorna verdadeiro caso são iguais, e falso caso contrário.
 * \param posicao1 posição 1
 * \param posicao2 posição 2
 */
bool SaoIguaisPosicao(tPosicao* posicao1, tPosicao* posicao2){
    if(posicao1 == NULL || posicao2 == NULL) return NULL;
    
    if((posicao1->linha == posicao2->linha) && (posicao1->coluna == posicao2->coluna)){
        return true;
    }
    else{
        return false;
    }
}
/**
 * Libera o espaço alocado para a estrutura tPosicao
 * 
 * \param posicao posicao
 */
void DesalocaPosicao(tPosicao* posicao){
    free(posicao);
    return;
}