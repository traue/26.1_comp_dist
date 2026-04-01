
# Atividade 3: Transformação paralela de dados com MPI

## Descrição

Nesta atividade, você irá desenvolver um programa paralelo usando a biblioteca **MPI (Message Passing Interface)** com o objetivo de aplicar uma **transformação matemática em um grande conjunto de dados**, de forma eficiente e distribuída.

O cenário simula uma situação comum em sistemas de processamento paralelo: um grande volume de dados que deve passar por uma transformação, sendo inicialmente **distribuído entre diferentes processos MPI**, que aplicam a transformação localmente. Após isso, os resultados locais são **reunidos** para formar o conjunto de dados final transformado.

---

## Objetivo

O desafio é:

1. Inicializar um vetor de inteiros de tamanho fixo (`DATA_SIZE = 100`) contendo valores de `1` a `100`.
2. Distribuir esse vetor igualmente entre **5 processos MPI**.
3. Cada processo aplica a **função de transformação** localmente:  
   - \( f(x) = x \cdot x \) (ou seja, o quadrado de cada elemento).
4. Após a transformaação, reunir os resultados com `MPI_Gather` para reconstruir o vetor final transformado no processo mestre (rank 0).
5. O processo mestre exibe o vetor final transformado.

---

## Parâmetros Fixos

- **DATA_SIZE = 100**: tamanho total do vetor de dados.
- **Número de processos = 5**
- Cada processo ficará responsável por exatamente **20 elementos**.
- Os dados são números inteiros no intervalo `[1, 100]`.

---

## Requisitos

1. O programa deve ser executado com exatamente 5 processos.
2. O vetor original deve ser criado **somente no processo de rank 0**, contendo os valores de 1 a 100.
3. Utilizar `MPI_Scatter` para dividir o vetor entre os processos.
4. Cada processo aplica localmente a transformação:  
   - \( x 
ightarrow x^2 \)
5. Os resultados devem ser reunidos no processo 0 com `MPI_Gather`.
6. O processo 0 deve imprimir:
   - O vetor original (antes da transformação)
   - O vetor transformado (com os quadrados dos valores)

---

## Dica

- Use `MPI_Scatter` para enviar partes iguais do vetor original a cada processo.
- Após a transformação local, use `MPI_Gather` para reunir os pedaços no processo mestre.
- Lembre-se de alocar os buffers corretamente em cada processo para a parte que ele receberá.

---

## Exemplo de Saída Esperada (simplificado)

```
[Processo 0] Vetor original: [1, 2, 3, ..., 100]
[Processo 0] Vetor transformado: [1, 4, 9, ..., 10000]
```

---

## Extensões sugeridas (opcional)

- Permitir que o `DATA_SIZE` e o número de processos sejam definidos pela linha de comando (desde que `DATA_SIZE` seja divisível pelo número de processos).
- Adicionar uma transformação mais complexa, como uma função polinomial ou exponencial.
- Medir o tempo de execução com `MPI_Wtime()` para analisar a performance.

---

## Desafio Extra

Implemente uma versão onde o vetor transformado **não seja armazenado no processo 0**, mas sim em **todos os processos** usando `MPI_Allgather`.


---

## Envio da atividade

Enviar a atividade em PDF contendo os testes em prints (execução) e código-fonte (pode ser um link para um repositório). Mantenha organizado!

Não esqueça de incluir o nome de todos os integrantes no PDF!
