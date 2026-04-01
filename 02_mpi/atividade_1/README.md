# Atividade 1 de MPI: Soma de quadrados com validação

## Objetivo

Neste exercício, você irá implementar um programa em **C com MPI** que calcula a **soma dos quadrados dos números de 1 a 40** de forma **paralela**. Ao final, o programa deverá comparar o resultado obtido com o resultado **sequencial esperado**, validando a execução paralela.

---

## Enunciado

Implemente um programa MPI que siga os seguintes passos:

1. O processo **root (rank 0)** deve criar um vetor contendo os inteiros de `1` a `N`, onde `N = 40`.

2. Esse vetor deverá ser **dividido igualmente entre todos os processos** com `MPI_Scatter`.  
   > Exemplo: se `N = 40` e há `4` processos, cada um receberá `10` elementos.

3. Cada processo deve calcular a **soma dos quadrados** dos elementos recebidos.
   > Exemplo: se recebeu `[3, 4, 5]`, calcular `3² + 4² + 5² = 50`.

4. Com `MPI_Reduce`, envie todas as somas locais para o processo root, que deve calcular a **soma total dos quadrados**.

5. O processo root também deve calcular a **soma sequencial** dos quadrados (1² + 2² + ... + 40²) e comparar com o resultado paralelo.

6. Exibir na tela:
   - O vetor local de cada processo (para verificação)
   - O resultado da soma paralela
   - O resultado da soma sequencial
   - Se os valores **coincidem ou não**

---

## Regras e restrições

- O vetor global deve ser dividido igualmente — use um número de processos que divida 40 exatamente.
- Use **apenas** `MPI_Scatter` e `MPI_Reduce`.
- **Não utilize** `MPI_Gather`, `MPI_Probe`, `MPI_Cancel` nem `MPI_Allgather`.
- O programa deve funcionar corretamente com **4 processos**.

---

## Dica

Você pode usar a fórmula da soma dos quadrados para validar o resultado no processo root:

```math
\sum_{i=1}^{N} i^2 = \frac{N(N+1)(2N+1)}{6}
```

Para `N = 40`, essa fórmula gera o valor **esperado** da soma dos quadrados.

---

## Exemplo de Saída Esperada (sugestão)

```bash
Processo 0 recebeu: 1 2 3 4 5 6 7 8 9 10
Processo 1 recebeu: 11 12 13 14 15 16 17 18 19 20
Processo 2 recebeu: 21 22 23 24 25 26 27 28 29 30
Processo 3 recebeu: 31 32 33 34 35 36 37 38 39 40

Processo 0: soma local dos quadrados = 385
Processo 1: soma local dos quadrados = 1540
Processo 2: soma local dos quadrados = 3555
Processo 3: soma local dos quadrados = 9660

Processo 0: soma paralela dos quadrados = 22140
Processo 0: soma sequencial esperada    = 22140

✅ Os valores conferem!
```

ou...


```bash
[DEBUG] Processo 0 - Quadrados locais: 1^2 + 2^2 + ... + 10^2 = 385
[DEBUG] Processo 1 - Quadrados locais: 11^2 + ... + 20^2 = 1540
[DEBUG] Processo 2 - Quadrados locais: 21^2 + ... + 30^2 = 3555
[DEBUG] Processo 3 - Quadrados locais: 31^2 + ... + 40^2 = 9660

[RESULTADO] Processo root (0):
  Soma global (via MPI_Reduce) = 22140
  Soma esperada (via fórmula)  = 22140
  ✅ Resultado validado com sucesso.
```
---

## Teste o programa com:

```bash
mpicc -o soma_quadrados soma_quadrados.c
mpirun -np 4 ./soma_quadrados
```

---

## Envio da atividade

Enviar a atividade em PDF contendo os testes em prints (execução) e código-fonte (pode ser um link para um repositório). Mantenha organizado!

Não esqueça de incluir o nome de todos os integrantes no PDF!
