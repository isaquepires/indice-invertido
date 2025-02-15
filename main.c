#include <stdio.h>
#include <stdlib.h>

#define MAX_PALAVRA 50
#define MAX_PALAVRAS 1000
#define CONTEXTO 5

/* calcular o tamanho de uma string */
int calcula_tamanho(char *string)
{
  int tam = 0;
  while (string[tam] != '\0')
    tam++;
    
  return tam;
}

/* para comparar duas strings */
int comparar(char *uma_string, char *outra_string)
{
  while (*uma_string && (*uma_string == *outra_string))
  {
    uma_string++;
    outra_string++;
  }

  return *(unsigned char *)uma_string - *(unsigned char *)outra_string;
}

/* para copiar uma string */
void copiar_string(char *outra_string, char *uma_string)
{
  while (*uma_string)
  {
    *outra_string = *uma_string;
    outra_string++;
    uma_string++;
  }
  
  *outra_string = '\0';
}

/* para converter em minusculo */
char converter_caractere_minusculo(char caractere)
{
  if (caractere >= 'A' && caractere <= 'Z')
    return caractere + ('a' - 'A');

  return caractere;
}

/* para converter string em minusculo */
void converter_string_minusculo(char *string)
{
  for (int i = 0; string[i]; i++)
    string[i] = converter_caractere_minusculo(string[i]);
}

/* para verificar se é pontuação */
int verificar_pontuacao(char caractere)
{
  char pontuacoes[] = ".,;:!?()[]{}\"'";
  
  for (int i = 0; pontuacoes[i]; i++)
  {
    if (caractere == pontuacoes[i])
      return 1;
  }

  return 0;
}

// Função para remover pontuação no final de uma palavra
void remover_pontuacao(char *string)
{
  int tamanho = calcula_tamanho(string);
  
  while (tamanho > 0 && verificar_pontuacao(string[tamanho - 1]))
  {
    string[tamanho - 1] = '\0';
    tamanho--;
  }
}

/* struct de palavras */
typedef struct
{
  char palavra[MAX_PALAVRA];
  int ocorrencias[100]; // posição
  int qtd_ocorrencias;
} ArquivoInvertido;

/* encontrar uma palavra no índice */
int encontrar_palavra(ArquivoInvertido *indice, int tamanho, char *palavra)
{
  for (int i = 0; i < tamanho; i++)
  {
    if (comparar(indice[i].palavra, palavra) == 0)
      return i;
  }

  return -1;
}

FILE* ler_arquivo(char *texto)
{
  int i = 0;
  FILE* arquivo = fopen(texto, "r");
  
  if (!arquivo)
  {
    printf("erro ao abrir arquivo!");
    return NULL;
  }
  
  return arquivo;
}

/* cria arquivo invertido relacionando o vocabulário com as ocorrências */
int criar_arquivo_invertido(char *nome_arquivo, ArquivoInvertido *indice)
{
  FILE *arquivo = ler_arquivo(nome_arquivo);

  int total_palavras = 0;
  int posicao = 0;
  char palavra[MAX_PALAVRA];

  while (fscanf(arquivo, "%49s", palavra) != EOF)
  {
    remover_pontuacao(palavra);
    converter_string_minusculo(palavra);

    /* ignorar espaços */
    if (calcula_tamanho(palavra) == 0) continue;

    /* verifica se a palavra já está no índice */
    int index = encontrar_palavra(indice, total_palavras, palavra);
    if (index == -1)
    {
      /* copia nova palavra ao índice */
      copiar_string(indice[total_palavras].palavra, palavra);
      indice[total_palavras].ocorrencias[0] = posicao;
      indice[total_palavras].qtd_ocorrencias = 1;
      total_palavras++;
    } else
    {
      /* adiciona a nova ocorrência */
      int qtd = indice[index].qtd_ocorrencias;
      indice[index].ocorrencias[qtd] = posicao;
      indice[index].qtd_ocorrencias++;
    }
    
    posicao++;
  }

  fclose(arquivo);
    
  return total_palavras;
}

/* para exibir o índice invertido */
void exibir_invertido(ArquivoInvertido *indice, int total_palavras)
{
  for (int i = 0; i < total_palavras; i++)
  {
    printf("%s: ", indice[i].palavra);
    
    for (int j = 0; j < indice[i].qtd_ocorrencias; j++)
      printf("%d ", indice[i].ocorrencias[j]);
        
    printf("\n");
  }
}


void procurar_palavra(char *nome_arquivo, ArquivoInvertido *indice, int total_palavras, char *palavra_busca, int max_ocorrencias)
{
  remover_pontuacao(palavra_busca);  // remove pontuação

  int index = encontrar_palavra(indice, total_palavras, palavra_busca);
  
  if (index == -1)
  {
    printf("\n\"%s\" não foi encontrada.\n", palavra_busca);  
    return;
  }

  if (!max_ocorrencias)
  {
    if (indice[index].qtd_ocorrencias > 1)
      printf("\n\"%s\" foi encontrada %d vezes\n", palavra_busca, indice[index].qtd_ocorrencias);
    else
      printf("\n\"%s\" foi encontrada %d vez\n", palavra_busca, indice[index].qtd_ocorrencias);
  }

  max_ocorrencias++;

  FILE *arquivo = fopen(nome_arquivo, "r");
  if (!arquivo)
  {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  char palavras_no_arquivo[MAX_PALAVRAS][MAX_PALAVRA]; // Armazena todas as palavras do arquivo
  int posicao = 0;
    
  /* para ler todas as palavras do arquivo e remover pontuação */
  while (fscanf(arquivo, "%49s", palavras_no_arquivo[posicao]) != EOF && posicao < MAX_PALAVRAS)
    posicao++;

  for (int i = 0; i < indice[index].qtd_ocorrencias && i < max_ocorrencias; i++)
  {
    int pos = indice[index].ocorrencias[i];
    printf("\nPosição %d: ", pos);

    /* para exibir palavras antes da palavra encontrada */
    int inicio = (pos - CONTEXTO > 0) ? pos - CONTEXTO : 0;
    for (int j = inicio; j < pos; j++)
      printf("%s ", palavras_no_arquivo[j]);

    /* exibindo a palavra encontrada em destaque */
    printf("[%s] ", palavras_no_arquivo[pos]);

    /* exibindo palavras depois da palavra encontrada */
    int fim = (pos + CONTEXTO < posicao) ? pos + CONTEXTO : posicao - 1;
    for (int j = pos + 1; j <= fim; j++)
      printf("%s ", palavras_no_arquivo[j]);
  }

  printf("\n");
}

void funcoes_texto()
{
  system("clear");
  printf(" 1) Ler um arquivo texto\n");
  printf(" 2) Apresentar o arquivo invertido\n");
  printf(" 3) Procurar uma palavra no arquivo invertido\n");
  printf(" 4) Procurar as próximas ocorrências\n");
  printf(" 5) Sair do sistema\n");
}


void voltar()
{
  getchar();
  printf("\nAperte qualquer tecla para voltar ao menu principal.\n");
  getchar();
  system("clear");
}

int main()
{
  int ocorrencia = 0;
  char *nome_arquivo = "Historia.txt";
  char palavra_busca[MAX_PALAVRA];
  ArquivoInvertido indice[MAX_PALAVRAS];
  int total_palavras = criar_arquivo_invertido(nome_arquivo, indice);
  int opcoes;
  int index;
  funcoes_texto();
  printf("\n Digite a opção desejada:\n");
  scanf("%d", &opcoes);
  int max_ocorrencias = 0;

  switch (opcoes)
  {
    case 1:
      system("clear");
      fclose(ler_arquivo(nome_arquivo));
      printf(" arquivo lido com sucesso!\n");
      voltar();
      main();
      break;
    case 2:
      system("clear");
      exibir_invertido(indice, total_palavras);
      voltar();
      main();
      break;
    case 3:
      system("clear");
      printf("Digite a palavra que deseja buscar: ");
      scanf("%s", palavra_busca);
      index = indice[encontrar_palavra(indice, total_palavras, palavra_busca)].qtd_ocorrencias;
      procurar_palavra(nome_arquivo, indice, total_palavras, palavra_busca, 0);
      voltar();
      main();
      break;
    case 4:
      system("clear");
      printf("Digite a palavra que deseja buscar: ");
      scanf("%s", palavra_busca);
      index = indice[encontrar_palavra(indice, total_palavras, palavra_busca)].qtd_ocorrencias;
      procurar_palavra(nome_arquivo, indice, total_palavras, palavra_busca, index);
      voltar();
      main();
      break;
    case 5:
      printf("saindo...\n");
      exit(0);
      break;
    default:
      printf("opção inválida\n");
      exit(1);
      break;
  }

  return 0;
}
