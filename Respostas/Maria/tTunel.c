#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "tTunel.h"
#include "tPosicao.h"

/**
 * Cria o tunel dinamicamente.
 * Dadas as posições de linha e coluna dos 2 acessos do tunel,
 * cria duas instancias tPosicao dinamicamente, uma pra cada acesso, insere as
 * inforações das posições passadas como parâmetro. Cria o tTunel dinamicamente.
 * Atribui as duas instancias tPosicao aos acessos do túnel, retorna o ponteiro para
 * o túnel.
 * \param linhaAcesso1 linha da posição do acesso 1 do túnel
 * \param colunaAcesso1 coluna da posição do acesso 1 do túnel
 * \param linhaAcesso2 linha da posição do acesso 2 do túnel
 * \param colunaAcesso2 coluna da posição do acesso 2 do túnel
 */
tTunel* CriaTunel(int linhaAcesso1, int colunaAcesso1, int linhaAcesso2, int colunaAcesso2){
    tTunel* tunel = NULL;

    tunel->acesso1->linha = linhaAcesso1;
    tunel->acesso1->coluna = colunaAcesso1;
    tunel->acesso2->linha = linhaAcesso2;
    tunel->acesso2->coluna = colunaAcesso2;

    return tunel;
}

/**
 * Verifica se entrou no túnel pela posição.
 * Dado o túnel e a posição atual do pacman,
 * verifica se a posição do pacman é a mesma de algum dos 
 * dois acessos do túnel.
 * \param tunel tunel
 * \param posicao posição
 */
bool EntrouTunel(tTunel* tunel, tPosicao* posicao){
    if( (posicao->linha == tunel->acesso1->linha && posicao->coluna == tunel->acesso1->coluna) || (posicao->linha == tunel->acesso2->linha && posicao->coluna == tunel->acesso2->coluna)){
        return true;
    }
    else{
        return false;
    }

}

/**
 * Atualiza a posição para o final do túnel.
 * Caso a posição do pacman seja a mesma de algum dos 2 
 * acessos do túnel, atualiza ela para o outro acesso do túnel.
 * \param tunel tunel
 * \param posicao posição
 */
void LevaFinalTunel(tTunel* tunel, tPosicao* posicao){
    if((posicao->linha == tunel->acesso1->linha && posicao->coluna == tunel->acesso1->coluna)){
        posicao->linha = tunel->acesso2->linha;
        posicao->coluna = tunel->acesso2->coluna;
    }
    else if((posicao->linha == tunel->acesso2->linha && posicao->coluna == tunel->acesso2->coluna)){
        posicao->linha = tunel->acesso1->linha;
        posicao->coluna = tunel->acesso1->coluna;
    }
}
/**
 * Caso o túnel seja diferente de NULL, libera o espaço alocado 
 * para a estrutura tTunel.
 * \param tunel tunel
 */
void DesalocaTunel(tTunel* tunel){
    DesalocaPosicao(tunel->acesso1);
    DesalocaPosicao(tunel->acesso2);
    free(tunel);
}