# Índice Invertido em C

Programa em C que lê um arquivo de texto, constrói um **arquivo/índice invertido** (mapeamento palavra → posições de ocorrência) e permite buscar palavras exibindo seu contexto (palavras vizinhas) no texto original.

## Funcionalidades

- **Leitura de arquivo texto** (padrão: `Historia.txt`, ou outro nome informado pelo usuário).
- **Construção do índice invertido**: cada palavra distinta do texto é associada à lista de posições (índices) em que ocorre.
- **Normalização de palavras**: remoção de pontuação no início/fim e conversão para minúsculas antes de indexar.
- **Exibição do índice invertido** completo (palavra + lista de posições).
- **Busca de uma palavra** no índice, mostrando:
  - Quantas vezes ela ocorre no texto;
  - O contexto de até 5 palavras antes e depois de cada ocorrência (janela definida por `CONTEXTO`).
- **Busca das próximas ocorrências**: repete a busca avançando para as ocorrências seguintes da mesma palavra.
- **Menu interativo** no terminal para acessar todas as opções.

## Estrutura de dados

```c
typedef struct {
    char palavra[MAX_PALAVRA];     // a palavra normalizada
    int  ocorrencias[100];         // posições (índices) onde a palavra aparece
    int  qtd_ocorrencias;          // quantidade de ocorrências registradas
} ArquivoInvertido;
```

### Constantes

| Constante      | Valor | Significado                                         |
|----------------|-------|------------------------------------------------------|
| `MAX_PALAVRA`  | 50    | Tamanho máximo de uma palavra                        |
| `MAX_PALAVRAS` | 1000  | Número máximo de palavras lidas do arquivo           |
| `CONTEXTO`     | 5     | Quantidade de palavras de contexto exibidas na busca |

## Principais funções

| Função | Descrição |
|---|---|
| `calcula_tamanho` | Calcula o tamanho de uma string (equivalente a `strlen`). |
| `comparar` | Compara duas strings caractere a caractere. |
| `copiar_string` | Copia o conteúdo de uma string para outra. |
| `converter_caractere_minusculo` / `converter_string_minusculo` | Convertem caractere/string para minúsculas. |
| `verificar_pontuacao` | Verifica se um caractere é um sinal de pontuação. |
| `remover_pontuacao` | Remove pontuação do final de uma palavra. |
| `encontrar_palavra` | Procura uma palavra dentro do índice já construído. |
| `ler_arquivo` | Abre um arquivo para leitura, tratando erro de abertura. |
| `criar_arquivo_invertido` | Lê o arquivo texto e monta o índice invertido. |
| `exibir_invertido` | Imprime todas as palavras do índice com suas posições. |
| `procurar_palavra` | Busca uma palavra específica e exibe o contexto das ocorrências. |
| `funcoes_texto` | Exibe o menu principal. |
| `die` | Valida se o arquivo configurado pode ser lido; encerra o programa caso contrário. |
| `main` | Loop principal do menu interativo. |

## Compilação

```bash
gcc -o indice_invertido main.c
```

## Uso

```bash
./indice_invertido
```

O programa apresenta o seguinte menu:

```
1) Ler um arquivo texto
2) Apresentar o arquivo invertido
3) Procurar uma palavra no arquivo invertido
4) Procurar as próximas ocorrências
5) Sair do sistema
```

1. **Opção 1** — informa o nome do arquivo de texto a ser usado (padrão inicial: `Historia.txt`).
2. **Opção 2** — reconstrói o índice a partir do arquivo atual e imprime cada palavra com suas posições.
3. **Opção 3** — solicita uma palavra e mostra quantas vezes ela ocorre, exibindo o contexto (até 5 palavras antes/depois) de cada ocorrência.
4. **Opção 4** — repete a busca da opção 3, mas avançando o número de ocorrências já exibidas (útil para "ver mais resultados").
5. **Opção 5** — encerra o programa.

## Observações e limitações

- O arquivo precisa existir e ser legível; caso contrário, as opções 2–4 exibem uma mensagem de erro (`die`).
- O índice é reconstruído a cada chamada das opções 2, 3 e 4 (não há cache entre buscas).
- `MAX_PALAVRAS` limita a 1000 o total de palavras lidas do arquivo; arquivos maiores serão truncados.
- Cada palavra pode ter no máximo 100 ocorrências registradas (`ocorrencias[100]`).
- A pontuação considerada inclui: `. , ; : ! ? ( ) [ ] { } " '`.
- Sistema pensado para uso via terminal em ambiente Linux/Unix (usa `system("clear")`).

## Possíveis melhorias futuras

- Alocação dinâmica para `ocorrencias` e para o número de palavras, removendo os limites fixos.
- Tratamento de pontuação no início da palavra (atualmente só remove no final).
- Persistência do índice em disco para evitar reprocessamento a cada operação.
- Suporte a acentuação e caracteres especiais de forma mais robusta (atualmente a conversão para minúsculas cobre apenas `A-Z`).
