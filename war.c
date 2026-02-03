#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// Definição constantes globais
#define NAME_STR_SIZE 30
#define COLOR_STR_SIZE 10
#define TERRITORY_NAMES_LENGTH 10
#define COLOR_NAMES_LENGTH 10
#define MISSION_TYPES_LENGTH 2

char const COLOR_NAMES[COLOR_NAMES_LENGTH][COLOR_STR_SIZE] = {"Branco", "Preto", "Vermelho", "Amarelo", "Verde", "Azul", "Roxo", "Laranja", "Cinza", "Ciano"};
char const TERRITORY_NAMES[TERRITORY_NAMES_LENGTH][NAME_STR_SIZE] = {"EUA", "Brasil", "Russia", "Alemanha", "China", "India", "Japao", "Franca", "Mongolia", "Africa do Sul"};
int const MISSION_TYPES[MISSION_TYPES_LENGTH] = { 0, 1 };

// Definindo a struct de territorio
struct Territory {
  char color[COLOR_STR_SIZE];
  char name[NAME_STR_SIZE];
  int numberOfTroops;
};

// Definindo a struct de missão
struct Mission {
  char desc[100];
  char destructionGoal[COLOR_STR_SIZE]; // usado apenas se o tipo da missão for 0 (destruir exercito x)
  char playerOriginalColor[COLOR_STR_SIZE];
  int conquestGoal; // usado apenas se o tipo da missão for 1 (conquistar n territorios)
  int type;
};

bool isGameLost(struct Mission* mission, struct Territory* list, int listSize);
bool isGameWon(struct Mission* mission, struct Territory* list, int listSize);
bool strContains(char list[][NAME_STR_SIZE], int listSize, char* item, int itemIndex);
int getTerritoryCap();
struct Territory* getPlayer(struct Territory* list, int listSize);
void attack(struct Territory* attacker, struct Territory* defender);
void cleanInputBuffer();
void convertTerritory(struct Territory *converted, struct Territory *converter);
void fillTerritoryList(struct Territory* list, int listSize);
void freeMemory(struct Territory* territoryList, struct Mission* mission);
void printActionsMenu(int* option);
void printAttackMenu(int *attackerOption, int *defenderOption, int territoryCap);
void printDefeatScreen();
void printMissionMenu(struct Mission* mission);
void printTerritoryMenu(struct Territory* list, int listSize);
void printVictoryScreen(struct Mission* mission);
void sortMission(struct Mission* mission, struct Territory* player, struct Territory* list, int listSize);
void sortNumberOfTroops(struct Territory* list, int listSize);
void sortTerritoryColors(struct Territory* list, int territoryCap);
void sortTerritoryNames(struct Territory* list, int listSize);

int main() {
  // Definindo a "semente" aleatória que a função rand() 
  // vai utilizar para basear a sua "aleatoriadade"
  srand(time(NULL));

  struct Territory *territoryList;
  struct Mission *mission;
  struct Territory *player;

  int territoryCap; 
  int attackerIndex, defenderIndex;
  int attackerOption, defenderOption, actionOption;

  territoryCap = getTerritoryCap();
  territoryList = (struct Territory *) malloc(territoryCap * sizeof(struct Territory));
  mission =  (struct Mission *) malloc(sizeof(struct Mission));

  if (territoryList == NULL || mission == NULL) {
    printf("Falha na alocação de memória.");
    return 1;
  }
  
  fillTerritoryList(territoryList, territoryCap);
  player = getPlayer(territoryList, territoryCap);
  sortMission(mission, player, territoryList, territoryCap);

  do {
    system("pause");
    printTerritoryMenu(territoryList, territoryCap);
    printActionsMenu(&actionOption);


    switch(actionOption) {
      case 1:
        printAttackMenu(&attackerOption, &defenderOption, territoryCap);
        
        if (attackerOption < 1 || attackerOption > territoryCap || defenderOption < 1 || defenderOption > territoryCap) {
          printf("Por favor, digite um numero valido para o territorio atacante e/ou defensor!\n");
          continue;
        } else {
          attackerIndex = attackerOption - 1;
          defenderIndex = defenderOption - 1;
    
          if (strcmp(territoryList[attackerIndex].color, territoryList[defenderIndex].color) == 0) {
            printf("Impossivel atacar o proprio territorio!\n");
            printf("Escolha 2 territorios de exercitos distintos!\n");
            continue;
          } 
          
          attack(&territoryList[attackerIndex], &territoryList[defenderIndex]);
        }
        break;
      case 2:
        printMissionMenu(mission);
        break;
      case 0: 
        printf("\nSaindo do programa...\n\n");
        break;
      default:
        if (actionOption < 0 || actionOption > 2) {
          printf("\n\nPor favor, digite uma opcao valida!\n");
        }
    }

    if (isGameWon(mission, territoryList, territoryCap)) {
      printVictoryScreen(mission);
      break;
    } else if (isGameLost(mission, territoryList, territoryCap)) {
      printDefeatScreen();
      break;
    }
  } while (actionOption != 0);
      
  freeMemory(territoryList, mission);
  
  system("pause");
  return 0;
}

//Checa se o jogador deixou de possuir territorios de sua cor
bool isGameLost(struct Mission* mission, struct Territory* list, int listSize) {
  for (int i = 0; i < listSize; i++) {
    if (strcmp(list[i].color, mission->playerOriginalColor) == 0) return false;
  }

  return true;
}

// Checa se o jogador completou sua missão
bool isGameWon(struct Mission* mission, struct Territory* list, int listSize) {
  int count = 0;

  switch (mission->type) {
    case 0: // destruir exercito x
      for (int i = 0; i < listSize; i++) {
        if (strcmp(list[i].color, mission->destructionGoal) == 0) return false;
      }
    case 1: // consquistar n territorios
      for (int i = 0; i < listSize; i++) {
        if (strcmp(list[i].color, mission->playerOriginalColor) == 0) count++;
      }

      if (count - 1 < mission->conquestGoal) return false;
      break;
  }

  return true;
}

// Função que checa se uma list de strings tem uma certa string
bool strContains(char list[][NAME_STR_SIZE], int listSize, char* item, int itemOriginalIndex) {
  for (int i = 0; i < listSize; i++) {
    if (i == itemOriginalIndex) continue;
    if (strcmp(list[i], item) == 0) return true;
  }
  
  return false;
}

// Função que gera um valor aleatório entre 2 e 5 para o número de territórios
int getTerritoryCap() {
  int territoryCap = rand() % 4 + 2;

  return territoryCap;
}

// Função para escolher aleatoriamente um territorio para ser o "jogador", todas as missões vão ser baseadas na cor do exercito desse territorio
struct Territory* getPlayer(struct Territory* list, int listSize) {
  int playerIndex = rand() % listSize;

  return &list[playerIndex];
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

  printf("\n");
}

// Função para limpar o buffer de strings, 
// evitando que acidentalmente acabe pulando alguma pergunta do usuário
void cleanInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c !=  EOF);
}

// muda o atributo "color" de um território pelo atributo color de outro território
// além disso, pega metade das tropas do territorio conversor (se for maior que 1) e a 
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

// Função que preenche a lista de teritorios com valores aleatorios definidos previamente
void fillTerritoryList(struct Territory* list, int listSize) {
  sortTerritoryNames(list, listSize);
  sortTerritoryColors(list, listSize);
  sortNumberOfTroops(list, listSize);  
}

// Função que libera a memória alocada para as structs de territorios e missão
void freeMemory(struct Territory* territoryList, struct Mission* mission) {
  free(mission);
  free(territoryList);
  printf("Memoria liberada.\n");
}

// Função que imprime o menu de ataque na tela
void printAttackMenu(int *attackerOption, int *defenderOption, int territoryCap) {
  printf("\n\n---------- FASE DE ATAQUE ----------\n");
  printf("Escolha o territorio atacante(1 a %d): ",  territoryCap);
  scanf("%d", attackerOption);
  printf("Escolha o territorio defensor(1 a %d): ",  territoryCap);
  scanf("%d", defenderOption);
  cleanInputBuffer();
  printf("\n");
}

// Função que imprime os territórios atuais e seus respectivos valores na tela
void printTerritoryMenu(struct Territory* list, int listSize) {
  printf("=============== MAPA DE TERRITORIOS - ESTADO ATUAL ===============\n");
  
  for (int i = 0; i < listSize; i++) {
    printf("%d - %s: (Exercito %s, Quantidade de Tropas: %d)\n", i + 1, list[i].name, list[i].color, list[i].numberOfTroops);
  }
}

//Função que imprime a tela de derrota
void printDefeatScreen() {
  printf("\n\n=================================================\n");  
  printf("================= FIM DE JOGO ===================\n");  
  printf("=================================================\n");
  
  printf("\n\nO jogador perdeu todo o seu exercito!\n\n");
}

//Função que imprime a tela de vitória
void printVictoryScreen(struct Mission* mission) {
  printf("\n\n=================================================\n");  
  printf("================= FIM DE JOGO ===================\n");  
  printf("=================================================\n");

  printf("\nMissao concluida!\n");
  
  printf("\n---------- MISSAO ----------\n");
  printf("Descricao: %s\n\n", mission->desc);
  printf("Vitoria do Exercito %s!\n\n", mission->playerOriginalColor);
}

// Função que imprime o menu de ações na tela
void printActionsMenu(int* option) {
  printf("\n\n---------- MENU DE ACOES ----------\n");
  printf("1 - Atacar\n");
  printf("2 - Verificar Missao\n");
  printf("0 - Sair\n");
  printf("Escolha sua acao: ");
  scanf("%d", option);
  cleanInputBuffer();
}

// Função que imprime o menu de missões na tela
void printMissionMenu(struct Mission* mission) {
  char s[25];

  switch (mission->type) {
    case 0:
      strcpy(s, "Destruicao de Exercito");
      break;
    case 1: 
      strcpy(s, "Conquista de Territorios");
      break;
  }
  
  printf("\n\n---------- MISSAO ----------\n");
  printf("Missao de: %s\n", mission->playerOriginalColor);
  printf("Tipo: %d (%s)\n", mission->type, s);
  printf("Descricao: %s\n\n", mission->desc);
}

// Função que gera uma missão para o jogador
void sortMission(struct Mission* mission, struct Territory* player, struct Territory* list, int listSize) {
  int missionType = MISSION_TYPES[rand() % 2];
  mission->type = missionType;
  strcpy(mission->playerOriginalColor, player->color);

  switch (missionType) {
    case 0: {
      struct Territory *target;
      
      do {
        target = &list[rand() % listSize];
      } while (target == player);
      
      strcpy(mission->destructionGoal, target->color);
      mission->conquestGoal = 0;
      sprintf(mission->desc, "Destruir exercito %s", target->color);
      break;
    }
    case 1: {
      int numberOfTerritories = rand() % listSize + 1;
      
      if (numberOfTerritories == listSize) numberOfTerritories--;
      
      strcpy(mission->destructionGoal, "Nenhum");
      mission->conquestGoal = numberOfTerritories;
      sprintf(mission->desc, "Consquistar %d territorios", numberOfTerritories);
      break;
    }
  }
}

// Função que atribuí um valor aleatório entre 1 e 7 para o número de tropas de cada território
void sortNumberOfTroops(struct Territory* list, int listSize) {
  for (int i = 0; i < listSize; i++) {
    int numberOfTroops = rand() % 7 + 1;

    list[i].numberOfTroops = numberOfTroops;
  }
}

// Função que sorteia cores aleatórias definidas previamente, cada território tem que pertencer a um exército distinto no começo do jogo
void sortTerritoryColors(struct Territory* list, int listSize) {
  char usedColors[listSize][NAME_STR_SIZE];
  
  for (int i = 0; i < listSize; i++) {
    char colorName[NAME_STR_SIZE];
    do {
      strcpy(colorName, COLOR_NAMES[rand() % COLOR_NAMES_LENGTH]);
            
      if (!strContains(usedColors, listSize, colorName, i)) strcpy(usedColors[i], colorName);
    } while (strContains(usedColors, listSize, colorName, i));
  }

  for (int i = 0; i < listSize; i++) {
    strcpy(list[i].color, usedColors[i]);
  }
}

// Função que sorteia nomes de território aleatórios definidos previamente, cada território tem que ter um nome único
void sortTerritoryNames(struct Territory* list, int listSize) {
  char usedTerritories[listSize][NAME_STR_SIZE];
  
  for (int i = 0; i < listSize; i++) {
    char territoryName[NAME_STR_SIZE];
    
    do {
      strcpy(territoryName, TERRITORY_NAMES[rand() % TERRITORY_NAMES_LENGTH]);
            
      if (!strContains(usedTerritories, listSize, territoryName, i)) strcpy(usedTerritories[i], territoryName);
    } while (strContains(usedTerritories, listSize, territoryName, i));
  }

  for (int i = 0; i < listSize; i++) {
    strcpy(list[i].name, usedTerritories[i]);
  }
}