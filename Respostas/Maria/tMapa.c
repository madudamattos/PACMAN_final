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

   fclose(pMapa);

   //trata tunel e comida

   tPosicao* comida = NULL;
   mapa->nFrutasAtual = 0;
   
   mapa->tunel = NULL;
   int linhaAcesso1 = -1, colunaAcesso1 = -1, linhaAcesso2 = -1, colunaAcesso2 = -1;
   int tunel = 0;

   for(i=0; i<qtdLinhas; i++){
      for(j=0; j<qtdColunas; j++){
         comida = CriaPosicao(i,j);
         if(EncontrouComidaMapa(mapa, comida)){
            mapa->nFrutasAtual++;
         }
         else if(mapa->grid[i][j] == '@' && tunel == 0){
            linhaAcesso1 = i;
            colunaAcesso1 = j;
            tunel = 1;
         }
         else if(mapa->grid[i][j] == '@' && tunel == 1){
            linhaAcesso2 = i;
            colunaAcesso2 = j;
            tunel = 2;
         }
         DesalocaPosicao(comida);
      }

   }

   if(tunel == 2){
      mapa->tunel = CriaTunel(linhaAcesso1, colunaAcesso1, linhaAcesso2, colunaAcesso2);
   } 

   return mapa;
}

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

tTunel* ObtemTunelMapa(tMapa* mapa){
   return mapa->tunel;
}

char ObtemItemMapa(tMapa* mapa, tPosicao* posicao){
   char item = '\0';
   int Lp = ObtemLinhaPosicao(posicao);
   int Cp = ObtemColunaPosicao(posicao);
   int L = ObtemNumeroLinhasMapa(mapa);
   int C = ObtemNumeroColunasMapa(mapa);

   if(mapa->grid == NULL || Cp >= C || Lp >= L){
      item = '\0';
   }
   else{
      item = mapa->grid[Lp][Cp];
   }

   return item;
}

int ObtemNumeroLinhasMapa(tMapa* mapa){
   return mapa->nLinhas;
}

int ObtemNumeroColunasMapa(tMapa* mapa){
   return mapa->nColunas;
}

int ObtemQuantidadeFrutasIniciaisMapa(tMapa* mapa){
   return mapa->nFrutasAtual;
}

int ObtemNumeroMaximoMovimentosMapa(tMapa* mapa){
   return mapa->nMaximoMovimentos;
}

bool EncontrouComidaMapa(tMapa* mapa, tPosicao* posicao){
   int Lp = ObtemLinhaPosicao(posicao);
   int Cp = ObtemColunaPosicao(posicao);
   int L = ObtemNumeroLinhasMapa(mapa);
   int C = ObtemNumeroColunasMapa(mapa);

   if(mapa->grid == NULL || Cp >= C || Lp >= L){
      return false;
   }
   else if(mapa->grid[Lp][Cp] == '*'){
      return true;
   }
   else{
      return false;
   }
}

bool EncontrouParedeMapa(tMapa* mapa, tPosicao* posicao){
   int Lp = ObtemLinhaPosicao(posicao);
   int Cp = ObtemColunaPosicao(posicao);
   int L = ObtemNumeroLinhasMapa(mapa);
   int C = ObtemNumeroColunasMapa(mapa);

   if(mapa->grid == NULL || Cp >= C || Lp >= L){
      return false;
   }
   else if(mapa->grid[Lp][Cp] == '#'){
      return true;
   }
   else{
      return false;
   }
   return false;
}

bool AtualizaItemMapa(tMapa* mapa, tPosicao* posicao, char item){
   int Lp = ObtemLinhaPosicao(posicao);
   int Cp = ObtemColunaPosicao(posicao);
   int L = ObtemNumeroLinhasMapa(mapa);
   int C = ObtemNumeroColunasMapa(mapa);

   if(mapa == NULL || mapa->grid == NULL || Cp >= C || Lp >= L){
      return false;
   }
   else{
      mapa->grid[Lp][Cp] = item;
      return true;
   }

   return false;
}

bool PossuiTunelMapa(tMapa* mapa){
   if(mapa->tunel != NULL) return true;
   else return false;
}

bool AcessouTunelMapa(tMapa* mapa, tPosicao* posicao){
   if(EntrouTunel(mapa->tunel, posicao)){
      return true;
   }
   else{
      return false;
   }
}

void EntraTunelMapa(tMapa* mapa, tPosicao* posicao){
   LevaFinalTunel(mapa->tunel, posicao);
}

void DesalocaMapa(tMapa* mapa){
   int i;
   int L= ObtemNumeroLinhasMapa(mapa);
   
   if(mapa!= NULL){
      if(mapa->grid != NULL){
         for(i=0; i<L; i++){
            free(mapa->grid[i]); 
         }

         free(mapa->grid);
      } 

      DesalocaTunel(mapa->tunel);

      free(mapa);
   }
}