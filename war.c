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
#define NUM_MISSOES 2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct Territorio {
    char nome[NOME_SIZE];
    char cor[COR_SIZE];
    int tropas;
} Territorio;

typedef struct Jogador {
    char *cor;
    int missao;
} Jogador;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();

int sortearMissao();

void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);

// Funções de interface com o usuário:
int faseDeAtaque(Territorio *mapa);

void exibirMapa(const Territorio* mapa);
void exibirMenuPrincipal(const Jogador j);
void exibirMissao(const int id);

// Funções de lógica principal do jogo:
int verificarVitoria(const Territorio *mapa, const Jogador *jogador);

void simularAtaque(Territorio *atacante, Territorio *defensor);

// Função utilitária:
void limparBufferEntrada();
void exibirTerritorio(const Territorio t);
void proximoTurno(void);

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
    Jogador jogador;

    jogador.cor = mapa[rand() % MAPA_SIZE].cor;
    jogador.missao = sortearMissao();

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    int escolha;

    do {
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    escolha = -1;
    exibirMapa(mapa);
    exibirMissao(jogador.missao);
    exibirMenuPrincipal(jogador);
    scanf("%d", &escolha);
    limparBufferEntrada();
    switch (escolha) {
        case 1:
            escolha = faseDeAtaque(mapa);
            break;
        case 2:
            if (verificarVitoria(mapa, &jogador)) {
                printf("Você completou a missão!\n");
                escolha = 0;
            }
            else {
                printf("Missão não completada.\n");
                escolha = 100;
            }
            break;
        case 0: break;
        default:
            printf("Opção inválida.\n");
            escolha = -1;
            break;
    }
    if (escolha > 0) proximoTurno();
    printf("\n");
    } while (escolha != 0);

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
void exibirMenuPrincipal(const Jogador j) {
    printf("====================\n");
    printf("------- Menu -------\n");
    printf("====================\n");
    printf("Seu turno(%s)! Escolha sua ação:\n", j.cor);
    printf("1. Atacar.\n");
    printf("2. Checar condição de vitória.\n");
    printf("0. Sair.\n");
    printf("-> ");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio* mapa) {
    printf("======= Placar =======\n");
    printf("ID | Território | Cor | Tropas\n");
    for (unsigned char i = 0; i < MAPA_SIZE; i++) {
        printf("%d | ", i + 1);
        exibirTerritorio(mapa[i]);
    }
    printf("\n");
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(const int id) {
    printf("======= Missão =======\n");
    if (id & 1) printf("Destrua todos os exércitos\n");
    else printf("Domine 3 territórios\n");
    printf("\n");
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
int faseDeAtaque(Territorio *mapa) {
    while (1) {
        int atacante = -1;
        int defensor = -1;

        while (atacante < 0 || atacante > MAPA_SIZE) {
            printf("Escolha o atacante (selecione o ID ou 0 pra sair): ");
            scanf("%d", &atacante);
            limparBufferEntrada();
            if (atacante < 0 || atacante > MAPA_SIZE) printf("Escolha inválida. Tente novamente.\n");
        }
        if (atacante == 0) return 0;
        atacante -= 1;
        while (defensor < 1 || defensor > MAPA_SIZE) {
            printf("Escolha o defensor: ");
            scanf("%d", &defensor);
            limparBufferEntrada();
            if (defensor < 1 || defensor > MAPA_SIZE || defensor == atacante) printf("Escolha inválida. Tente novamente.\n");
        }
        defensor -= 1;
        printf("\n");
        if (!strcmp(mapa[atacante].cor, mapa[defensor].cor)) {
            printf("Você não pode atacar um território da mesma cor.\n");
            continue;
        }
        simularAtaque(mapa + atacante, mapa + defensor);
        break;
    }
    return 1;
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio *atacante, Territorio *defensor) {
    int resultado_a = rand() % 6 + 1;
    int resultado_d = rand() % 6 + 1;

    printf("%s ataca %s: %dx%d\n", atacante->nome, defensor->nome, resultado_a, resultado_d);
    switch (resultado_a - resultado_d) {
        case 0:
            printf("Empate! %s(Atacante) perde 1 tropa.\n", atacante->nome);
            --atacante->tropas;
            break;

        case 1: case 2: case 3: case 4: case 5:
            printf("O atacante venceu! %s(Defensor) perde 1 tropa.\n", defensor->nome);
            --defensor->tropas;
            break;

        case -1: case -2: case -3: case -4: case -5:
            printf("O defensor venceu! %s(Atacante) perde 1 tropa.\n", atacante->nome);
            --atacante->tropas;
            break;
    }
    if (atacante->tropas == 0) {
        printf("%s(%s) foi derrotado! E agora pertence a %s.\n", atacante->nome, atacante->cor, defensor->cor);
        strcpy(atacante->cor, defensor->cor);
        atacante->tropas = 1;
    } else if (defensor->tropas == 0) {
        printf("%s(%s) foi derrotado! E agora pertence a %s.\n", defensor->nome, defensor->cor, atacante->cor);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = 1;
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
int verificarVitoria(const Territorio *mapa, const Jogador *jogador) {
    int territoriosConquistados = 0;

    switch (jogador->missao) {
        case 1:
            for (unsigned short i = 0; i < MAPA_SIZE; i++)
                if (strcmp(mapa[i].cor, jogador->cor) != 0) return 0;
            return 1;
            break;

        case 0:
            for (unsigned short i = 0; i < MAPA_SIZE; i++)
                if (strcmp(mapa[i].cor, jogador->cor) == 0) territoriosConquistados++;
            if (territoriosConquistados < 3) return 0;
            break;
    }
    return 0;
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirTerritorio(const Territorio t) {
    printf("%s | %s | %d\n", t.nome, t.cor, t.tropas);
}

void proximoTurno(void) {
    printf("Pressione ENTER para continuar...");
    (void)getchar();
}
