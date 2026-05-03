// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NOME_SIZE 30
#define COR_SIZE 10
#define MAPA_SIZE 5
#define NUM_MISSOES 2;

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct Territorio {
    char nome[NOME_SIZE];
    char cor[COR_SIZE];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();

void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);
int sortearMissao();

// Funções de interface com o usuário:
void exibirMapa(const Territorio* mapa);

// Funções de lógica principal do jogo:

// Função utilitária:
void limparBufferEntrada();
void exibirTerritorio(const Territorio t);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    setlocale(LC_ALL, "Portuguese");
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    Territorio* mapa = alocarMapa();
    if (!mapa) return 1;
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapa);
    // - Define a cor do jogador e sorteia sua missão secreta.
    // char *cor_jogador = mapa[rand() % MAPA_SIZE].cor;
    // int missao = sortearMissao();


    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    exibirMapa(mapa);
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarMapa() {
    Territorio* mapa = calloc(MAPA_SIZE, sizeof(Territorio));
    if (!mapa) printf("Alocação falhou.\n");
    return mapa;
}


// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio* mapa) {
    printf("Cadastre 5 territórios:\n\n");
    for (unsigned char i = 0; i < MAPA_SIZE; i++) {
        int final = 0;

        printf("Território %d:\n", i + 1);
        printf("Insira o nome (até %d caracteres): ", NOME_SIZE);
        fgets(mapa[i].nome, NOME_SIZE, stdin);
        final = strlen(mapa[i].nome) - 1;
        if (mapa[i].nome[final] != '\n') limparBufferEntrada();
        mapa[i].nome[final] = '\0';

        printf("Insira a cor (até %d caracteres): ", COR_SIZE);
        fgets(mapa[i].cor, COR_SIZE, stdin);
        final = strlen(mapa[i].cor) - 1;
        if (mapa[i].cor[final] != '\n') limparBufferEntrada();
        mapa[i].cor[final] = '\0';

        while (mapa[i].tropas < 1) {
            printf("Insira o número de tropas (número inteiro 1-%d): ", INT_MAX);
            scanf("%d", &mapa[i].tropas);
            limparBufferEntrada();
            if (mapa[i].tropas < 1) printf("Número de tropas inválido. Deve ser pelo menos 1.\n");
        }
        printf("\n");
    }
    printf("Territórios cadastrados com sucesso!\nIniciando jogo...\n");
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("Seu turno! Escolha sua ação:\n");
    printf("1. Atacar.\n");
    printf("2. Checar condição de vitória.\n");
    printf("0. Sair.\n");
    printf("-> ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa) {
    printf("Id | Território | Cor | Tropas\n");
    for (unsigned char i = 0; i < MAPA_SIZE; i++) {
        printf("%d | ", i + 1);
        exibirTerritorio(mapa[i]);
    }
    printf("\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    if ((rand() & 1) != 0) {
        defensor->tropas = 0;
        atacante->tropas -= defensor->tropas;
    } else {
        atacante->tropas = 0;
        defensor->tropas -= atacante->tropas;
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirTerritorio(const Territorio t) {
    printf("%s | %s | %d\n", t.nome, t.cor, t.tropas);
}
