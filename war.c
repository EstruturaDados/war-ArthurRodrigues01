#include <stdio.h>
#include <stdlib.h>

// Definição constantes globais, nesse caso apenas o número máximo de territórios possíveis
#define MAX_TERR 5

// Definindo a struct de territorio
struct Territory {
  char name[30];
  char color[10];
  int numberOfTroops;
};

// Função para limpar o buffer de strings, 
// evitando que acidentalmente acabe pulando alguma pergunta do usuário
void cleanInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c !=  EOF);
}

int main() {
  // Definição de um array de territórios: "territoryArray"
  struct Territory territoryArray[MAX_TERR];

  // Loop para ler os dados dos territórios digitados pelo usuário
  for (int i = 0; i < MAX_TERR; i++) {
    printf("\n\n=========== INSERIR TERRITORIO %d ===========\n", i + 1);
    
    printf("Digite o nome do territorio: ");
    fgets(territoryArray[i].name, sizeof(territoryArray[i].name), stdin);
    printf("Digite a cor do exercito no territorio: ");
    fgets(territoryArray[i].color, sizeof(territoryArray[i].color), stdin);
    printf("Digite o numero de tropas no territorio: ");
    scanf("%d", &territoryArray[i].numberOfTroops);

    //Limpar buffer
    cleanInputBuffer();
  }
  
  // Loop para exibir os dados dos territórios digitados pelo usuário
  printf("\n\n=========== MAPA DO MUNDO ===========");
  for (int i = 0; i < MAX_TERR; i++) {
    printf("\n\n=== Territorio %d ===\n", i + 1);

    printf("Nome do territorio: %s", territoryArray[i].name);
    printf("Cor do exercito: %s", territoryArray[i].color);
    printf("Numero de tropas: %d", territoryArray[i].numberOfTroops);
  }

  return 0;
}