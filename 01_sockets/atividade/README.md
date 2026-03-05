# ⚔️ Batalha de palavras em rede

Jogo multiplayer baseado em texto, implementado em C com **Sockets TCP** e **pthreads**.

---

## Descrição do jogo

Dois jogadores se conectam ao servidor. A cada rodada:

1. O servidor envia uma **letra aleatória**
2. Os jogadores têm **10 segundos** para digitar uma palavra que:
   - Comece com a letra indicada
   - Tenha no mínimo **5 caracteres**
   - Contenha apenas letras (sem números, espaços ou símbolos)
3. O servidor valida e pontua:
   - **Palavra válida** → +1 ponto
   - **Palavra inválida ou tempo esgotado** → 0 pontos
   - **Palavras repetidas** (iguais entre os dois jogadores) → ninguém pontua

O jogo tem **5 rodadas**. Ganha quem tiver mais pontos.

---

## Estrutura sugerida para o Projeto

```
atividade/
├── protocolo.h      ← definições do protocolo de comunicação (compartilhado)
├── jogo.h           ← interface da lógica do jogo
├── jogo.c           ← implementação: validação, geração de letras, comunicação
├── servidor.c       ← servidor concorrente (pthreads)
├── cliente.c        ← cliente interativo com select()
├── Makefile         ← compilação automatizada
└── README.md        ← este arquivo
```

### Responsabilidade de cada arquivo

| Arquivo | Responsabilidade |
|---------|-----------------|
| **protocolo.h** | Constantes do jogo (porta, rodadss, tempo) e prefixos das mensagens do protocolo. Compartilhado entre cliente e servidor. |
| **jogo.h / jogo.c** | Lógica pura do jogo: validação de palavras, geração de letras, funções de envio/recebimento formatadas pelo protocolo. |
| **servidor.c** | Aceita conexões, forma pares de jogadores, cria threads por partida, controla rodadas e placar. |
| **cliente.c** | Conecta ao servidor, interpreta mensagens do protocolo, exibe interface formatada, lê input com timeout via `select()`. |

---

## Protocolo de comunicação

Todas as mensagens são **strings de texto** com campos separados por `|` (pipe) e terminadas por `\n`.

### Servidor → Cliente

| Tipo | Formato | Descrição |
|------|---------|-----------|
| `MSG` | `MSG\|texto` | Mensagem genérica para exibição |
| `NOME` | `NOME\|` | Solicita o nome do jogador |
| `AGUARDE` | `AGUARDE\|texto` | Pede para aguardar (ex: segundo jogador) |
| `RODADA` | `RODADA\|num\|letra\|tempo` | Início de rodada com número, letra e tempo em segundos |
| `RESULTADO` | `RESULTADO\|texto` | Resultado individual da rodada |
| `PLACAR` | `PLACAR\|nome1\|pts1\|nome2\|pts2` | Placar atualizado |
| `FIM` | `FIM\|texto` | Fim do jogo com resultado final |

### Cliente → Servidor

| Tipo | Formato | Descrição |
|------|---------|-----------|
| `NOME` | `NOME\|nome_do_jogador` | Resposta com o nome |
| `PALAVRA` | `PALAVRA\|palavra_digitada` | Palavra da rodada |
| `TIMEOUT` | `TIMEOUT\|` | Tempo esgotado (não respondeu a tempo) |

---

## Diagrama de fluxo de mensagens

```
   CLIENTE A                SERVIDOR                 CLIENTE B
      │                        │                        │
      │──── connect ──────────▶│                        │
      │◀──── NOME| ────────────│                        │
      │──── NOME|Alice ───────▶│                        │
      │◀── AGUARDE|Esperando...│                        │
      │                        │◀────── connect ────────│
      │                        │──── NOME| ────────────▶│
      │                        │◀── NOME|Bob ───────────│
      │                        │                        │
      │◀── MSG|Alice vs Bob ───│── MSG|Alice vs Bob ───▶│
      │                        │                        │
      ├────────────── RODADA 1 ─────────────────────────┤
      │                        │                        │
      │◀── RODADA|1|M|10 ─────│── RODADA|1|M|10 ─────▶│
      │                        │                        │
      │── PALAVRA|manga ──────▶│                        │
      │                        │◀── PALAVRA|morango ────│
      │                        │                        │
      │                     [VALIDA]                    │
      │                        │                        │
      │◀── RESULTADO|+1 ──────│── RESULTADO|+1 ───────▶│
      │◀── PLACAR|Alice|1|... ─│── PLACAR|...|Bob|1 ──▶│
      │                        │                        │
      ├────────── RODADAS 2-5 (repete) ─────────────────┤
      │                        │                        │
      │◀──── FIM|Empate! ─────│──── FIM|Empate! ──────▶│
      │                        │                        │
```

---

## Compilação

```bash
make              # compila servidor e cliente
make clean        # remove binários
```

### Manualmente

```bash
# Servidor (precisa de -lpthread)
gcc -Wall -Wextra -pedantic -std=c11 -o servidor servidor.c jogo.c -lpthread

# Cliente
gcc -Wall -Wextra -pedantic -std=c11 -o cliente cliente.c jogo.c
```

---

## Execução

### 1. Iniciar o servidor

```bash
./servidor              # porta padrão 7070
./servidor 9000         # porta customizada
```

### 2. Conectar os clientes (em terminais separados)

```bash
./cliente                       # conecta em 127.0.0.1:7070
./cliente 127.0.0.1 9000        # IP e porta customizados
./cliente 192.168.1.10 7070     # conectar em outra máquina da rede
```

### 3. Jogar!

- O servidor aguarda **2 jogadores** antes de iniciar
- Cada jogador informa seu nome
- A cada rodada, ambos digitam uma palavra que começa com a letra indicada
- Após 5 rodadas, o resultado final é exibido

---

## Exemplo de Sessão

### Terminal do servidor:
```
╔══════════════════════════════════════════════╗
║      BATALHA DE PALAVRAS — Servidor         ║
║  Porta: 7070                                 ║
║  Aguardando jogadores (pares de 2)...       ║
╚══════════════════════════════════════════════╝

[+] Jogador conectou: 127.0.0.1:52341 (fd=4)
[*] Aguardando mais 1 jogador(es)...
[+] Jogador conectou: 127.0.0.1:52342 (fd=5)
[Partida #1] Jogadores: Alice vs Bob
  [Rodada 1] Letra: M
  [Rodada 1] Alice="manga"(ok) | Bob="morango"(ok) | Placar: 1 x 1
  [Rodada 2] Letra: C
  ...
[Partida #1] 🏆 Alice venceu! Placar final: Alice 3 x 2 Bob
```

### Terminal do cliente:
```
╔══════════════════════════════════════╗
║     BATALHA DE PALAVRAS — Cliente    ║
╚══════════════════════════════════════╝
  Conectando a 127.0.0.1:7070...
  Conectado!

  Digite seu nome: Alice
  Bem-vindo, Alice!

  ⏳ Conectado! Aguardando outro jogador para iniciar...

  🎮 Batalha de Palavras! Alice vs Bob — 5 rodadas. Boa sorte!

  ╔══════════════════════════════════╗
  ║        RODADA 1 de 5             ║
  ║  Letra: [M]   Tempo: 10 seg     ║
  ║  Mínimo: 5 caracteres           ║
  ╚══════════════════════════════════╝
  Sua palavra: manga
  Enviado: "manga" — aguardando resultado...
  📋 Palavra "manga" válida! +1 ponto. [Bob enviou: "morango"]
  ┌─────────────────────────────┐
  │  PLACAR: Alice    1  x  1 Bob      │
  └─────────────────────────────┘
```

---

## Conceitos Técnicos Demonstrados

| Conceito | Onde |
|----------|------|
| **Socket TCP** (socket, bind, listen, accept, connect, send, recv) | servidor.c, cliente.c |
| **Servidor concorrente** com `pthread_create`/`pthread_detach` | servidor.c |
| **Protocolo de aplicação** com mensagens estruturadas | protocolo.h, jogo.c |
| **Timeout com `select()`** | jogo.c (`receber_com_timeout`), cliente.c |
| **Validação de entrada** | jogo.c (`validar_palavra`) |
| **Sincronização de turnos** | servidor.c (`executar_rodada`) |
| **Modularização** em múltiplos arquivos .c e .h | todos os arquivos |
| **Tratamento de erros** | todos os arquivos |
| **Tratamento de sinais** (SIGPIPE, SIGINT) | servidor.c, cliente.c |

---

## Regras de validação

Uma palavra é **válida** se:

- [ ] Começa com a letra da rodada (case insensitive)
- [ ] Tem no mínimo 5 caracteres
- [ ] Contém apenas letras (a-z, A-Z)
- [ ] Não foi a mesma palavra que o oponente enviou

---

## Desafios extras (Opcionais)

- [ ] Permitir mais de dois jogadores por partida
- [ ] Criar ranking persistente em arquivo
- [ ] Modo "relâmpago" com tempo decrescente a cada rodada
- [ ] Sistema de salas (jogadores escolhem em qual sala entrar)
- [ ] Implementar versão UDP
