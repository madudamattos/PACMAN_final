#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"
#include "tPacman.h"

/**
 * Cria o pacman dinamicamente. Caso dê erro na alocação da estrutura tPacman, 
 * retorna NULL. 
 * Caso a posição passada como parâmetro seja NULL, retorna NULL.
 * Caso não dê erros, retorna o ponteiro para o tPacman alocado.
 * \param posicao Ponteiro para tPosicao
 */
tPacman* CriaPacman(tPosicao* posicao){
    tPacman* pacman = calloc(1, sizeof(tPacman));

    pacman->posicaoAtual->coluna = posicao->coluna;
    pacman->posicaoAtual->linha = posicao->linha;

    return pacman;
}
/**
 * Clona o pacman dinamicamente, apenas com relação a posição.
 * Aloca outro pacman apenas copiando as informações de linha e coluna do original (passado como parâmetro).
 * \param pacman pacman
 */
tPacman* ClonaPacman(tPacman* pacman){
    tPacman* pacmanClone = calloc(1, sizeof(tPacman));

    pacmanClone->posicaoAtual->coluna = pacman->posicaoAtual->coluna;
    pacmanClone->posicaoAtual->linha = pacman->posicaoAtual->linha;

    return pacmanClone;
}
/**
 * Clona a lista historico de movimentos significativos do pacman.
 * Aloca dinamicamente todas as estruturas do histórico de
 * movimentos do pacman original (passado como parâmetro): a lista e os movimentos da lista.
 * E por fim copia as informações do histórico original para o clone.
 * Retorna um ponteiro para o tMovimento** clone.
 * \param pacman pacman
 */
tMovimento** ClonaHistoricoDeMovimentosSignificativosPacman(tPacman* pacman);

/**
 * Retorna a posição do pacman.
 * 
 * \param pacman pacman
 */
tPosicao* ObtemPosicaoPacman(tPacman* pacman){
    tPosicao* posicaoPacman = NULL;

    posicaoPacman->linha = pacman->posicaoAtual->linha;
    posicaoPacman->coluna = pacman-> posicaoAtual->linha;

    return posicaoPacman;
}
/**
 * Retorna se o pacman está vivo ou morto.
 * 
 * \param pacman pacman
 */
int EstaVivoPacman(tPacman* pacman){
    if(pacman->estaVivo == 1){
        return 1;
    }
    else{
        return 0;
    }
}
/**
 * Função que irá mover o pacman no mapa, atualizando sua posição.
 * Dado o pacman, o mapa, e o comando do jogador, a posição do pacman
 * é atualizada. Consultas ao mapa deverão ser feitas para ver se
 * a posição pode ser realmente atualizada ou não, como por exemplo,
 * se o pacman encontrou uma parede ele não se move.
 * 
 * \param pacman pacman
 * \param mapa o mapa que contem o pacman
 * \param comando o comando para onde irá o pacman
 */
void MovePacman(tPacman* pacman, tMapa* mapa, COMANDO comando){

}

/**
 * Aloca a trilha dinamicamente.
 * Caso a trilha seja igual a NULL, a matriz int** deverá ser
 * alocada dinamicamente com os valores de linha e coluna.
 * 
 * \param pacman pacman
 * \param nLinhas número de linhas da trilha
 * \param nColunas número de colunas da trilha
 */
void CriaTrilhaPacman(tPacman* pacman, int nLinhas, int nColunas){

}

/**
 * Atualiza na trilha a posição por onde passou o pacman.
 * Dado o pacman, com suas informações de posição e trilha na estrutura,
 * atualiza o item da trilha, referente a posição atual do pacman,
 * para o valor int correspondente ao número do movimento atual do pacman.
 * \param pacman pacman
 */
void AtualizaTrilhaPacman(tPacman* pacman){

}

/**
 * Salva a trilha em um arquivo na raíz junto com o binário.
 * 
 * \param pacman pacman
 */
void SalvaTrilhaPacman(tPacman* pacman){

}

/**
 * Insere na lista de movimentos um novo movimento significativo.
 * Dado o pacman, o comando do jogador, e a ação significativa,
 * cria um movimento significativo tMovimento com essas informações e
 * insere na lista de movimentos significativos do pacman.
 * 
 * \param pacman pacman
 * \param comando o comando do movimento
 * \param acao a ação do movimento
 */
void InsereNovoMovimentoSignificativoPacman(tPacman* pacman, COMANDO comando, const char* acao){

}

/**
 * Seta pacman para morto.
 * 
 * \param pacman pacman
 */
void MataPacman(tPacman* pacman){
    pacman->estaVivo = 0;
}
/**
 * Caso o pacman seja diferente de NULL, libera o espaço 
 * alocado para a estrutura tPacman
 * 
 * \param pacman pacman
 */
void DesalocaPacman(tPacman* pacman){

}

/**
 * Retorna o número atual de movimentos do pacman
 * 
 * \param pacman pacman
 */
int ObtemNumeroAtualMovimentosPacman(tPacman* pacman){
    return 0;
}

/**
 * Retorna o número de movimentos sem pontuar do pacman
 * 
 * \param pacman pacman
 */
int ObtemNumeroMovimentosSemPontuarPacman(tPacman* pacman){
    return 0;
}

/**
 * Retorna o número de colisões com a parede do pacman
 * 
 * \param pacman pacman
 */
int ObtemNumeroColisoesParedePacman(tPacman* pacman){
    int totalColisoes = pacman->nColisoesParedeBaixo + pacman->nColisoesParedeCima + pacman->nColisoesParedeDireita + pacman->nColisoesParedeEsquerda;

    return totalColisoes;
}

int ObtemNumeroMovimentosBaixoPacman(tPacman* pacman){
    return pacman->nMovimentosBaixo;
}

int ObtemNumeroFrutasComidasBaixoPacman(tPacman* pacman){
    return pacman->nFrutasComidasBaixo;
}

int ObtemNumeroColisoesParedeBaixoPacman(tPacman* pacman){
    return pacman->nColisoesParedeBaixo;
}

int ObtemNumeroMovimentosCimaPacman(tPacman* pacman){
    return pacman->nMovimentosCima;
}

int ObtemNumeroFrutasComidasCimaPacman(tPacman* pacman){
    return pacman->nFrutasComidasCima;
}

int ObtemNumeroColisoesParedeCimaPacman(tPacman* pacman){
    return pacman->nColisoesParedeCima;
}

int ObtemNumeroMovimentosEsquerdaPacman(tPacman* pacman){
    return pacman->nMovimentosEsquerda;
}

int ObtemNumeroFrutasComidasEsquerdaPacman(tPacman* pacman){
    return pacman->nFrutasComidasEsquerda;
}

int ObtemNumeroColisoesParedeEsquerdaPacman(tPacman* pacman){
    return pacman->nColisoesParedeEsquerda;
}

int ObtemNumeroMovimentosDireitaPacman(tPacman* pacman){
    return pacman->nMovimentosDireita;
}

int ObtemNumeroFrutasComidasDireitaPacman(tPacman* pacman){
    return pacman->nFrutasComidasDireita;
}

int ObtemNumeroColisoesParedeDireitaPacman(tPacman* pacman){
    return pacman->nColisoesParedeDireita;
}

int ObtemNumeroMovimentosSignificativosPacman(tPacman* pacman){
    return pacman->nMovimentosSignificativos;
}

int ObtemPontuacaoAtualPacman(tPacman* pacman){
    int pontuacao = pacman->nFrutasComidasBaixo + pacman->nFrutasComidasCima + pacman->nFrutasComidasDireita + pacman->nFrutasComidasEsquerda;

    return pontuacao;
}