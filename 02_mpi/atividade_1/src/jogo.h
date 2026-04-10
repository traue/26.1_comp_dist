/*
 * jogo.h — Interface da lógica do jogo "Batalha de Palavras"
 *
 * Contém as estruturas de dados e declarações de funções
 * que controlam o estado da partida.
 */

#ifndef JOGO_H
#define JOGO_H

#include "protocolo.h"

/* ============================================================
 * Estruturas de dados
 * ============================================================ */

/* Representa um jogador na partida */
typedef struct {
    int   fd;                    /* file descriptor do socket */
    char  nome[NOME_SIZE];       /* nome do jogador */
    int   pontos;                /* pontuação acumulada */
    char  palavra[BUFFER_SIZE];  /* palavra enviada na rodada atual */
    int   respondeu;             /* 1 se já respondeu nesta rodada */
    int   valida;                /* 1 se a palavra é válida nesta rodada */
} Jogador;

/* Representa o estado de uma partida */
typedef struct {
    Jogador jogadores[MAX_JOGADORES];
    int     num_jogadores;
    int     rodada_atual;        /* 1 a TOTAL_RODADAS */
    char    letra_atual;         /* letra da rodada */
} Partida;

/* ============================================================
 * Funções — Lógica do jogo
 * ============================================================ */

/* Inicializa uma partida com valores zerados */
void partida_inicializar(Partida *p);

/* Gera uma letra aleatória maiúscula (A-Z) para a rodada */
char gerar_letra(void);

/* Converte uma string para minúsculas (in-place) */
void str_to_lower(char *s);

/* Remove espaços e \n do final da string (in-place) */
void str_trim(char *s);

/*
 * Valida a palavra de um jogador:
 *   - Começa com a letra da rodada?
 *   - Tem no mínimo MIN_CARACTERES caracteres?
 *
 * Retorna 1 se válida, 0 se inválida.
 * Preenche 'motivo' com a razão da invalidez.
 */
int validar_palavra(const char *palavra, char letra, char *motivo, size_t motivo_size);

/*
 * Verifica se dois jogadores enviaram a mesma palavra.
 * Retorna 1 se são iguais, 0 se diferentes.
 */
int palavras_iguais(const char *p1, const char *p2);

/* ============================================================
 * Funções — Comunicação (wrappers do protocolo)
 * ============================================================ */

/* Envia uma mensagem formatada pelo protocolo ao jogador */
void enviar_msg(int fd, const char *texto);

/* Envia solicitação de nome */
void enviar_pedir_nome(int fd);

/* Envia mensagem de "aguarde" */
void enviar_aguarde(int fd, const char *texto);

/* Envia início de rodada: número, letra e tempo */
void enviar_rodada(int fd, int num_rodada, char letra, int tempo);

/* Envia resultado da rodada */
void enviar_resultado(int fd, const char *texto);

/* Envia o placar atual */
void enviar_placar(int fd, Partida *p);

/* Envia mensagem de fim de jogo */
void enviar_fim(int fd, const char *texto);

/*
 * Recebe dados do socket com timeout.
 * Retorna número de bytes lidos, 0 se timeout, -1 se erro/desconexão.
 */
ssize_t receber_com_timeout(int fd, char *buf, size_t buf_size, int timeout_seg);

#endif /* JOGO_H */
