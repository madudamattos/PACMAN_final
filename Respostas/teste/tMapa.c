#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tTunel.h"
#include "tMapa.h"

/**
 * Dado o arquivo de configurações, cria o mapa dinamicamente e 
 * retorna um ponteiro para o tipo tMapa.
 * Caso o arquivo de configurações não exista, retorna NULL.
 * \param caminho caminho do arquivo com as configurações do mapa
 */
tMapa* CriaMapa(const char* caminhoConfig){
   tMapa* mapa = (tMapa*)calloc(1, sizeof(tMapa));
   
   if (mapa == NULL) {
      printf("Erro na alocação de mapa\n");
      exit(1);
   }

   char caminhoMapa[100];
   FILE *pMapa = NULL;
   char caractere = '\0';

   sprintf(caminhoMapa, "%s/mapa.txt", caminhoConfig);

   pMapa = fopen(caminhoMapa, "r");

   if (pMapa == NULL) {
      printf("Erro na abertura do arquivo de mapa\n");
      exit(1);
   }

   fscanf(pMapa, "%d%*c", &mapa->nMaximoMovimentos);

   mapa->grid = (char**)calloc(1,sizeof(char*));

   if (mapa->grid == NULL) {
     printf("Erro na alocação do grid\n");
     exit(1);
   }

   int qtdLinhas = 0, qtdColunas = 0;

   mapa->grid[qtdLinhas] = (char*)calloc(1,sizeof(char));

   if (mapa->grid[qtdLinhas] == NULL) {
     printf("Erro na alocação do grid\n");
     exit(1);
   }

   while (1) {

     fscanf(pMapa, "%c", &caractere);

     while (caractere != '\n' && caractere != EOF) {
        mapa->grid[qtdLinhas][qtdColunas] = caractere;

        qtdColunas++;

        mapa->grid[qtdLinhas] = (char*)realloc(mapa->grid[qtdLinhas], (qtdColunas + 1) * sizeof(char));

        if (mapa->grid[qtdLinhas] == NULL) {
           printf("Erro na alocação das colunas do mapa\n");
           exit(1);
        }

        fscanf(pMapa, "%c", &caractere);
     }

     if(caractere == '\n') qtdLinhas++;

     if (caractere == EOF) {
        mapa->nColunas = qtdColunas;
        mapa->nLinhas = qtdLinhas; // Incrementado para contar a última linha.
        break;
     }

     qtdColunas = 0;

     mapa->grid = (char**)realloc(mapa->grid, (qtdLinhas + 1) * sizeof(char*));
      
     if (mapa->grid == NULL) {
        printf("Erro na alocação das linhas do mapa\n");
        exit(1);
     }

   }

   // int i, j;

   // //printf("%d", mapa->nMaximoMovimentos);
   // //conta qtd de linhas e coluans primeiro   
   // while(1){

   //    printf("%d", mapa->nMaximoMovimentos);
      
   //    fscanf(pMapa, "%c", &caractere);
      
   //    //printf("1");

   //    printf("%c", caractere);

   //    if(caractere == EOF) break;

   //    if(caractere != '\n'){
   //       qtdColunas++;
   //    }

   //    if(caractere == '\n'){
   //       qtdLinhas++;
   //    }

   //    printf("2");
   // }

   // qtdColunas = qtdColunas/qtdLinhas;

   // mapa->grid = calloc(1, qtdLinhas*qtdColunas*sizeof(char));

   // if(mapa->grid == NULL){
   //    printf("Erro na alocacao do grid\n");
   //    exit(1);
   // } 

   // //fscanf(pMapa, "%d%*c", &mapa->nMaximoMovimentos);
   
   // for(i=0; i<qtdLinhas; i++){
   //    for(j=0; j<qtdColunas; j++){
   //       fscanf(pMapa, "%c", &caractere);
   //       mapa->grid[i][j] = caractere;
   //    }
   // }

   fclose(pMapa);

   return mapa;
}


/**
 * Obtem a posição de um item do mapa.
 * Dado um item do tipo char, o mapa deverá ser varrido até encontrar o item.
 * Caso o item seja encontrado cria um tipo tPosição e retorna.
 * Caso o item não seja encontrado, retorna NULL.
 * \param mapa mapa
 * \param item item a ser procurado no mapa
 */
tPosicao* ObtemPosicaoItemMapa(tMapa* mapa, char item){
   tPosicao* posicao = NULL;
   int i,j;

   for(i=0; i< mapa->nLinhas; i++){
      for(j=0; j<mapa->nColunas; j++){
         if(mapa->grid[i][j] == item){
            posicao->linha = i;
            posicao->coluna = j;
         }
      }
   }

   return posicao;
}

/**
 * Retorna um ponteiro para o túnel da estrutura tMapa.
 * \param mapa mapa
 */
tTunel* ObtemTunelMapa(tMapa* mapa){
   tTunel* tunel = NULL;
   int i, j;
   
   tunel->acesso1 = NULL;
   tunel->acesso2 = NULL;

   for(i=0; i<mapa->nLinhas; i++){
      for(j=0; j<mapa->nColunas; j++){
         if(mapa->grid[i][j] == '@' && tunel->acesso1 == NULL ){
            tunel->acesso1->linha = i;
            tunel->acesso1->coluna = j;
         }
         else if(mapa->grid[i][j] == '@' && tunel->acesso2 == NULL){
            tunel->acesso2->linha = i;
            tunel->acesso2->coluna = j;
         }
      }
   }

   return tunel;
}

char ObtemItemMapa(tMapa* mapa, tPosicao* posicao){
   char item = '\0';

   if(mapa->grid == NULL || posicao->coluna >= mapa->nColunas || posicao->linha >= mapa->nLinhas){
      item = '\0';
   }
   else{
      item = mapa->grid[posicao->linha][posicao->coluna];
   }

   return item;
}

int ObtemNumeroLinhasMapa(tMapa* mapa){
   return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa* mapa){
   return mapa->nColunas;
}

/**
 * Retorna a quantidade de frutas iniciais do mapa
 * \param mapa mapa
 */
int ObtemQuantidadeFrutasIniciaisMapa(tMapa* mapa){
   int i, j;
   tPosicao* comida = NULL;
   int totalComida = 0;

   for (i=0; i< mapa->nLinhas;i++){
      for(j=0;j<mapa->nColunas; j++){
         comida->linha = i;
         comida->coluna = j;
         if(EncontrouComidaMapa(mapa, comida)){
            totalComida++;
         }
      }
   }

   return totalComida;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa* mapa){
   return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa* mapa, tPosicao* posicao){
   if(mapa->grid == NULL || posicao->coluna >= mapa->nColunas || posicao->linha >= mapa->nLinhas){
      return false;
   }
   else if(mapa->grid[posicao->linha][posicao->coluna] == '*'){
      return true;
   }
   else{
      return false;
   }

   return false;
}

bool EncontrouParedeMapa(tMapa* mapa, tPosicao* posicao){
   if(mapa->grid == NULL || posicao->coluna >= mapa->nColunas || posicao->linha >= mapa->nLinhas){
      return false;
   }
   else if(mapa->grid[posicao->linha][posicao->coluna] == '#'){
      return true;
   }
   else{
      return false;
   }
   return false;
}

/**
 * Dado o mapa, uma posição e um item, atualiza a posição do mapa com aquele item.
 * Caso o mapa ou o grid do mapa seja NULL, ou a posição esteja fora dos limites do mapa,
 * retorna falso. 
 * Se conseguiu atualizar o item com sucesso, retorna verdadeiro.
 * \param mapa mapa
 * \param posicao posicao do item
 * \param item posicao item que vai atualizar o mapa
 */
bool AtualizaItemMapa(tMapa* mapa, tPosicao* posicao, char item){
   if(mapa->grid == NULL || posicao->coluna >= mapa->nColunas || posicao->linha >= mapa->nLinhas){
      return false;
   }
   else{
      mapa->grid[posicao->linha][posicao->coluna] = item;
      return true;
   }

   return false;
}

bool PossuiTunelMapa(tMapa* mapa){
   int i, j;
   int tunel = 0;

   for (i=0; i< mapa->nLinhas;i++){
      for(j=0;j<mapa->nColunas; j++){
         if(mapa->grid[i][j] == '@'){
            tunel = 1;
            return true;
         }
      }
   }

   if(tunel == 0){
      return false;
   }

   return false;
}

/**
 * Apenas chama a função EntrouTunel na estrutura tTunel.h
 * \param mapa mapa
 * \param posicao posicao a ser verificada
 */
bool AcessouTunelMapa(tMapa* mapa, tPosicao* posicao){
   if(EntrouTunel(mapa->tunel, posicao)){
      return true;
   }
   else{
      return false;
   }
}

/**
 * Apenas chama a função LevaFinalTunel na estrutura tTunel.h
 * \param mapa mapa
 * \param posicao posicao que vai entrar no túnel
 */
void EntraTunelMapa(tMapa* mapa, tPosicao* posicao){
   LevaFinalTunel(mapa->tunel, posicao);
}

/**
 * Libera o espaço alocado para a estrutura tMapa.
 * Apenas desaloca o mapa caso o mapa seja diferente de NULL.
 * \param mapa mapa
 */
void DesalocaMapa(tMapa* mapa){

}