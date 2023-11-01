#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tPosicao.h"
#include "tTunel.h"
#include "tMapa.h"

tMapa* CriaMapa(const char* caminhoConfig) {
   tMapa* mapa = (tMapa*)calloc(1, sizeof(tMapa));

   if (mapa == NULL) {
      printf("Erro na alocação de mapa\n");
       exit(1);
   }

   char caminhoMapa[100];
   FILE* pMapa = NULL;
   char caractere = '\0';

   sprintf(caminhoMapa, "%s/mapa.txt", caminhoConfig);

   pMapa = fopen(caminhoMapa, "r");

   if (pMapa == NULL) {
      printf("Erro na abertura do arquivo de mapa\n");
      exit(1);
   }

   fscanf(pMapa, "%d%*c", &mapa->nMaximoMovimentos);
 
   int i=0, j=0, qtdLinhas = 0, qtdColunas = 0;

   while(!feof(pMapa)){

      fscanf(pMapa, "%c", &caractere);

      while(caractere != '\n'){
         
         if(qtdLinhas<1) qtdColunas++;
         
         if(feof(pMapa)) break; 

         fscanf(pMapa, "%c", &caractere);
         
         
      }

      qtdLinhas++;
      caractere = '\0';
   }

   qtdLinhas--;


   //printf("%d %d\n", qtdLinhas, qtdColunas);

   mapa->nColunas = qtdColunas;
   mapa->nLinhas = qtdLinhas;

   //alocacao do grid

   mapa->grid = (char**) malloc(qtdLinhas * sizeof(char*));

   if(mapa->grid == NULL){
      printf("Erro na alocacao do grid");
      exit(1);
   }

   for(i=0; i<qtdLinhas; i++){
      mapa->grid[i] = malloc(qtdColunas * sizeof(char));  
   }

   fclose(pMapa);

   pMapa = fopen(caminhoMapa, "r");

   fscanf(pMapa, "%*d%*c");

   //leitura do grid;

   for(i=0; i<qtdLinhas; i++){
      for(j=0; j<qtdColunas;j++){
         fscanf(pMapa, "%c", &mapa->grid[i][j]);
      }
      fscanf(pMapa, "%*c");
   }
   
   //teste pra printar o mapa
   // for(i=0; i<qtdLinhas; i++){
   //     for(j=0; j<qtdColunas; j++){
   //         printf("%c", mapa->grid[i][j]);
   //     }
   //     printf("\n");
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
   int L = ObtemNumeroLinhasMapa(mapa);
   int C = ObtemNumeroColunasMapa(mapa);

   if(mapa == NULL) return NULL;

   for(i=0; i< L; i++){
      for(j=0; j<C; j++){
         if(mapa->grid[i][j] == item){
            posicao = CriaPosicao(i,j);
            return posicao;
         }
      }
   }

   return NULL;
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
   int i, j;

   if(mapa == NULL) return;

   int L= ObtemNumeroLinhasMapa(mapa);

   for(i=0; i<L; i++){
      free(mapa->grid[i]);  
   }
   free(mapa->grid);

   DesalocaTunel(mapa->tunel);

   free(mapa);
}