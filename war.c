#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição constantes globais
#define NAME_STR_SIZE 30
#define COLOR_STR_SIZE 10

// Definindo a struct de territorio
struct Territory {
  char name[NAME_STR_SIZE];
  char color[COLOR_STR_SIZE];
  int numberOfTroops;
};

// Função para limpar o buffer de strings, 
// evitando que acidentalmente acabe pulando alguma pergunta do usuário
void cleanInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c !=  EOF);
}

// muda o atributo "color" de um território pelo atributo color de outro território
// além disso, subtrai uma tropa do território conversor (se for maior que 1) e a 
// adiciona ao território à ser convertido
void convertTerritory(struct Territory *converted, struct Territory *converter) {
  strcpy(converted->color, converter->color);
  
  if (converter->numberOfTroops > 1) {
    converted->numberOfTroops += converter->numberOfTroops / 2;
    converter->numberOfTroops -= converter->numberOfTroops / 2 ;
  } else {
    converted->numberOfTroops++;
  }
}

// Função que simula o ataque
void attack(struct Territory* attacker, struct Territory* defender) {
  int attackerDice = rand() % 6 + 1;
  int defenderDice = rand() % 6 + 1;
  
  printf("\n---------- RESULTADO DA BATALHA ----------\n");
  printf("Atacante %s, Resultado da rolagem de dado: %d\n", attacker->name, attackerDice);
  printf("Defensor %s, Resultado da rolagem de dado: %d\n\n", defender->name, defenderDice);

  if (attackerDice >= defenderDice) {
    if (attackerDice == defenderDice) {
      printf("EMPATE! Vantagem do atacante!\n");
    }
    printf("VITORIA DO ATACANTE! O defensor perdeu 1 tropa.\n");
    defender->numberOfTroops--;

    if (defender->numberOfTroops == 0) {
      printf("CONQUISTA! O territorio %s foi dominado pelo exercito %s!\n", defender->name, attacker->color);
      convertTerritory(defender, attacker);
    }
  } else if (attackerDice < defenderDice) {
    printf("VITORIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
    attacker->numberOfTroops--;

    if (attacker->numberOfTroops == 0) {
      printf("CONQUISTA! O territorio %s foi dominado pelo exercito %s!\n", attacker->name, defender->color);
      convertTerritory(attacker, defender);
    }
  }
}

// Checa se o jogo já foi ganho, utilizado para encerrar o jogo automáticamente e exibir
// a tela de fim de jogo
int checkIfGameWon(struct Territory* list, int listSize) {
  char anyTerritoryColor[COLOR_STR_SIZE];
  
  strcpy(anyTerritoryColor, list[0].color);

  for (int i = 0; i < listSize; i++) {
    // compara as strings do campo "color" de cada item da lista,
    // se alguma delas for diferente da cor escolhida de algum território aleatório
    // o jogo ainda não foi completamente dominado por um exército apenas
    if (strcmp(anyTerritoryColor, list[i].color) != 0) return 0; 
  }

  // retorna 1 se todas os items tiverem o mesmo valor para o campo color, ou seja, todos
  // do mesmo exército, jogo já foi decidido
  return 1;
}

// Função que faz a leitura dos territórios e insere eles na lista
void fillTerritoryList(struct Territory* list, int listSize) {
  char tempName[NAME_STR_SIZE];
  char tempColor[COLOR_STR_SIZE];
  int tempNumberOfTroops;
  int i = 0;

  while (i < listSize) {
    printf("=============== Insert Territorio %d ===============\n", i + 1);

    printf("Digite um nome para o territorio: ");
    fgets(tempName, sizeof(tempName), stdin);
    tempName[strlen(tempName) - 1] = '\0'; //remove o \n do final da string
    
    printf("Digite uma cor para o territorio: ");
    fgets(tempColor, sizeof(tempColor), stdin);
    tempColor[strlen(tempColor) - 1] = '\0'; // remove o \n do final da string
    
    printf("Digite o numero de tropas presentes no territorio: ");
    scanf("%d", &tempNumberOfTroops);
    cleanInputBuffer();

    if (tempNumberOfTroops < 1) {
      printf("Por favor, digite um numero maior que 0!\n");
      continue;
    }

    strcpy(list[i].name, tempName);
    strcpy(list[i].color, tempColor);
    list[i].numberOfTroops = tempNumberOfTroops;
    i++;
    printf("\n\n");
  }
  
}

// Função que imprime os territórios atuais e seus respectivos valores na tela
void printTerritoryMenu(struct Territory* list, int listSize) {
  printf("=============== MAPA DE TERRITORIOS - ESTADO ATUAL ===============\n");
  
  for (int i = 0; i < listSize; i++)
  {
    printf("%d - %s: (Exercito %s, Quantidade de Tropas: %d)\n", i + 1, list[i].name, list[i].color, list[i].numberOfTroops);
  }
}

int main() {
  // Definindo a "semente" aleatória que a função rand() 
  // vai utilizar para basear a sua "aleatoriadade"
  srand(time(NULL));

  // Definição de um ponteiro de territórios: "territoryList"
  struct Territory *territoryList;
  int territoryCap, attackerIndex, defenderIndex, attackerOption, defenderOption;

  do {
    printf("Digite o numero de territorios: ");
    scanf("%d", &territoryCap);
    cleanInputBuffer();
    
    if (territoryCap < 2) {
      printf("O numero de territorios deve ser pelo menos 2\n");
      printf("Por favor, digite um numero maior que 1!\n\n");
    }
  } while (territoryCap < 2);
  
  // Alocando memória com malloc para o ponteiro territoryList
  territoryList = (struct Territory *) malloc(territoryCap * sizeof(struct Territory));

  if (territoryList == NULL) {
    printf("Falha na alocação de memória.");
    return 1;
  }
  
  //Chamando função que irá fazer o loop de inserção dos territórios
  fillTerritoryList(territoryList, territoryCap);

  do {
    system("pause");
    //Chamando função que irá fazer o loop de impressão do mapa atual
    printTerritoryMenu(territoryList, territoryCap);

    printf("\n\n---------- FASE DE ATAQUE ----------\n");
    printf("Escolha o territorio atacante(1 a %d, ou 0 para sair): ",  territoryCap);
    scanf("%d", &attackerOption);
    printf("Escolha o territorio defensor(1 a %d, ou 0 para sair): ",  territoryCap);
    scanf("%d", &defenderOption);
    cleanInputBuffer();
    printf("\n");

    if (attackerOption == 0 || defenderOption == 0) {
      printf("Saindo do programa...\n");
    } else if (attackerOption < 0 || attackerOption > territoryCap || defenderOption < 0 || defenderOption > territoryCap) {
      printf("Por favor, digite um número válido para o território atacante e/ou defensor!\n");
      continue;
    } else {
      attackerIndex = attackerOption - 1;
      defenderIndex = defenderOption - 1;

      if (strcmp(territoryList[attackerIndex].color, territoryList[defenderIndex].color) == 0) {
        printf("Impossível atacar o proprio territorio!\n");
        printf("Escolha 2 territorios de exercitos distintos!\n");
        continue;
      } 
      
      attack(&territoryList[attackerIndex], &territoryList[defenderIndex]);

      if (checkIfGameWon(territoryList, territoryCap) == 1) {
        printf("\n\n=================================================\n");  
        printf("================= FIM DE JOGO ===================\n");  
        printf("=================================================\n");
        
        printf("\n\nVITORIA DO EXERCITO %s!\n", territoryList[0].color);
        break;
      }
    }

    printf("\n");
  } while (attackerOption != 0 && defenderOption != 0);

  free(territoryList);
  
  system("pause");
  return 0;
}