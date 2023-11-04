#include "tPosicao.h"
#include "tMovimento.h"
#include "tMapa.h"
#include "tPacman.h"

tPacman* CriaPacman(tPosicao* posicao){
    tPacman* pacman = calloc(1, sizeof(tPacman));

    if(pacman == NULL){
        printf("Erro na alocação do pacman\n");
        return NULL;
    }
    
    if(posicao == NULL){
        printf("Posicao do pacman invalida\n");
        return NULL;
    }
    
    pacman->posicaoAtual = ClonaPosicao(posicao);
    pacman->estaVivo = 1;
 
    //ver se não precisa fazer a inicializacao das outras coisas

    pacman->historicoDeMovimentosSignificativos = (tMovimento **) calloc(1, sizeof(tMovimento*));
    if(pacman->historicoDeMovimentosSignificativos == NULL){
        printf("Erro na alocacao do vetor de Movimentos significativos\n");
        exit(1);
    }

    pacman->trilha = NULL;
    // CriaTrilhaPacman(pacman, pacman->nLinhasTrilha, pacman->nColunasTrilha);
    // AtualizaTrilhaPacman(pacman);

    return pacman;
}

tPacman* ClonaPacman(tPacman* pacman){
    tPacman* pacmanClone = calloc(1, sizeof(tPacman));

    pacmanClone->posicaoAtual = ClonaPosicao(pacman->posicaoAtual);

    return pacmanClone;
}

tMovimento** ClonaHistoricoDeMovimentosSignificativosPacman(tPacman* pacman){
    tMovimento** cloneMovimento = NULL;

    int nMovSig = ObtemNumeroMovimentosSignificativosPacman(pacman);
    int i;

    cloneMovimento = (tMovimento **)calloc(nMovSig, sizeof(tMovimento*));
    
    for(i=0; i<nMovSig; i++){
        cloneMovimento[i] = CriaMovimento(pacman->historicoDeMovimentosSignificativos[i]->numeroDoMovimento, pacman->historicoDeMovimentosSignificativos[i]->comando, pacman->historicoDeMovimentosSignificativos[i]->acao);
    }

    return cloneMovimento;
}

tPosicao* ObtemPosicaoPacman(tPacman* pacman){
    tPosicao* posicaoPacman = NULL;

    posicaoPacman = ClonaPosicao(pacman->posicaoAtual);

    return posicaoPacman;
}
 
int EstaVivoPacman(tPacman* pacman){
    if(pacman->estaVivo == 1){
        return 1;
    }
    else{
        return 0;
    }
}

void MovePacman(tPacman* pacman, tMapa* mapa, COMANDO comando){
    //prevê a próxima posicao 
    tPosicao* proximaPosicao = NULL;
    int pI = ObtemLinhaPosicao(pacman->posicaoAtual);
    int pJ = ObtemColunaPosicao(pacman->posicaoAtual);

    if(comando == MOV_CIMA){
        proximaPosicao = CriaPosicao(pI - 1, pJ);
        pacman->nMovimentosCima++;
    }
    else if(comando == MOV_BAIXO){
        proximaPosicao = CriaPosicao(pI + 1, pJ);
        pacman->nMovimentosBaixo++;
    }
    else if(comando == MOV_DIREITA){
        proximaPosicao = CriaPosicao(pI, pJ + 1);
        pacman->nMovimentosDireita++;
    }
    else if(comando == MOV_ESQUERDA){
        proximaPosicao = CriaPosicao(pI, pJ - 1);
        pacman->nMovimentosEsquerda++;
    }

    //verifica se a proxima posicao é valida e move 

    int ppI = ObtemLinhaPosicao(proximaPosicao);
    int ppJ = ObtemColunaPosicao(proximaPosicao);
    
    if(EncontrouParedeMapa(mapa, proximaPosicao)){
        //a proxima posicao volta a ser a posicao atual, ou seja ele fica no mesmo lugar.
        proximaPosicao = ClonaPosicao(pacman->posicaoAtual); 

        //atualiza contador colisao
        if(comando == MOV_CIMA){
            pacman->nColisoesParedeCima++;
        }
        else if(comando == MOV_BAIXO){
            pacman->nColisoesParedeBaixo++;
        }
        else if(comando == MOV_DIREITA){
            pacman->nColisoesParedeDireita++;
        }
        else if(comando == MOV_ESQUERDA){
            pacman->nColisoesParedeEsquerda++;
        }

        //adiciona movimento significativo;
        char* acao = "colidiu com a parede";
        InsereNovoMovimentoSignificativoPacman(pacman, comando, acao);
    
    }
    else if(PossuiTunelMapa(mapa)){
        tTunel* tunelMapa = ObtemTunelMapa(mapa);
        if(EntrouTunel(tunelMapa, proximaPosicao)){
            //atualiza a trilha com a posicao do ultimo portal
            pacman->trilha[ppI][ppJ] = ObtemNumeroAtualMovimentosPacman(pacman);
            
            LevaFinalTunel(tunelMapa, proximaPosicao);
        }
    }

    //verifica se morreu OU se pontuou;
    if(EncontrouComidaMapa(mapa, proximaPosicao)){

        //atualiza o contador
        if(comando == MOV_CIMA){
            pacman->nFrutasComidasCima++;
        }
        else if(comando == MOV_BAIXO){
            pacman->nFrutasComidasBaixo++;
        }
        else if(comando == MOV_DIREITA){
            pacman->nFrutasComidasDireita++;
        }
        else if(comando == MOV_ESQUERDA){
            pacman->nFrutasComidasEsquerda++;
        }

        AtualizaItemMapa(mapa, proximaPosicao, ' ');
        //adicionamovimento significativo
        char* acao = "pegou comida";
        InsereNovoMovimentoSignificativoPacman(pacman, comando, acao);
    }

    //atualiza a posicao atual do pacman para a proxima posicao
    
    DesalocaPosicao(pacman->posicaoAtual);

    pacman->posicaoAtual = ClonaPosicao(proximaPosicao);
    
    DesalocaPosicao(proximaPosicao);
    
    AtualizaTrilhaPacman(pacman);
    
    return;
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
    int i, j;

    if(pacman->trilha != NULL) return;

    pacman->nLinhasTrilha = nLinhas;
    pacman->nColunasTrilha = nColunas;

    pacman->trilha = (int **) calloc(nLinhas, sizeof(int*));

    for(i=0; i<nLinhas; i++){
        pacman->trilha[i] = (int *) calloc(nColunas, sizeof(int));
    } 
 
    for(i=0; i<nLinhas; i++){
        for(j=0; j<nColunas; j++){
            pacman->trilha[i][j] = -1;
        }
    }

    i = ObtemLinhaPosicao(pacman->posicaoAtual);
    j = ObtemColunaPosicao(pacman->posicaoAtual);

    pacman->trilha[i][j] = 0;

    return;
}

/**
 * Atualiza na trilha a posição por onde passou o pacman.
 * Dado o pacman, com suas informações de posição e trilha na estrutura,
 * atualiza o item da trilha, referente a posição atual do pacman,
 * para o valor int correspondente ao número do movimento atual do pacman.
 * \param pacman pacman
 */
void AtualizaTrilhaPacman(tPacman* pacman){
    int i = ObtemLinhaPosicao(pacman->posicaoAtual);
    int j = ObtemColunaPosicao(pacman->posicaoAtual);
    int nMovAtual = ObtemNumeroAtualMovimentosPacman(pacman);

    pacman->trilha[i][j] = nMovAtual;
}

/**
 * Salva a trilha em um arquivo na raíz junto com o binário.
 * 
 * \param pacman pacman
 */
void SalvaTrilhaPacman(tPacman* pacman){
    char nomeArquivo[100];
    FILE *pTrilha = NULL;

    sprintf(nomeArquivo, "trilha.txt");

    pTrilha = fopen(nomeArquivo, "w");

    int L = pacman->nLinhasTrilha;
    int C = pacman->nColunasTrilha;
    int i, j;

    for(i=0; i<L;i++){
        for(j=0; j<C;j++){
            if(pacman->trilha[i][j] == -1){
                fprintf(pTrilha, "# ");
            }
            else{
                fprintf(pTrilha, "%d ", pacman->trilha[i][j]);
            }
        }
        fprintf(pTrilha, "\n");
    }

    // fprintf(pTrilha, "nada acontece");
    fclose(pTrilha);
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

    pacman->nMovimentosSignificativos++;

    pacman->historicoDeMovimentosSignificativos = realloc(pacman->historicoDeMovimentosSignificativos, (pacman->nMovimentosSignificativos)*sizeof(tMovimento*));

    int numeroMovimento = ObtemNumeroAtualMovimentosPacman(pacman);

    pacman->historicoDeMovimentosSignificativos[pacman->nMovimentosSignificativos - 1] = CriaMovimento(numeroMovimento, comando, acao);
}

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
    
    if(pacman == NULL) return;

    int nMovSig = ObtemNumeroMovimentosSignificativosPacman(pacman);
    int i;

    for(i=0; i<nMovSig;i++){
        DesalocaMovimento(pacman->historicoDeMovimentosSignificativos[i]);
    }
    free(pacman->historicoDeMovimentosSignificativos);

    if(pacman->trilha != NULL){
        for(i=0; i<pacman->nLinhasTrilha; i++){
            free(pacman->trilha[i]);
        }
        free(pacman->trilha);
    }
    
    DesalocaPosicao(pacman->posicaoAtual);

    free(pacman);
}

int ObtemNumeroAtualMovimentosPacman(tPacman* pacman){
    int nMovCima = ObtemNumeroMovimentosCimaPacman(pacman);
    int nMovBaixo = ObtemNumeroMovimentosBaixoPacman(pacman);
    int nMovDireita = ObtemNumeroMovimentosDireitaPacman(pacman);
    int nMovEsquerda = ObtemNumeroMovimentosEsquerdaPacman(pacman);

    int nMovTotais = nMovCima + nMovBaixo + nMovDireita + nMovEsquerda;
    
    return nMovTotais;
}

int ObtemNumeroMovimentosSemPontuarPacman(tPacman* pacman){
    int nMovTotais = ObtemNumeroAtualMovimentosPacman(pacman);
    int nMovPontuou = ObtemPontuacaoAtualPacman(pacman); 

    int nMovimentosSemPontuar = nMovTotais - nMovPontuou;

    return nMovimentosSemPontuar;
}

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