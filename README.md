# Trabalho Prático 1 – Algoritmo do Banqueiro

## Descrição
Este projeto consiste na implementação do algoritmo do banqueiro em linguagem C, com o objetivo de simular o gerenciamento de recursos em um ambiente concorrente.

O programa representa múltiplos clientes que solicitam e liberam recursos simultaneamente, garantindo que o sistema permaneça sempre em um estado seguro.

## Tecnologias utilizadas
- Linguagem C
- Biblioteca Pthreads

## Compilação
Para compilar o programa, utilize o seguinte comando:

gcc -pthread main.c -o banqueiro

## Execução
Para executar o programa, utilize:

./banqueiro 10 5 7

Onde:
10 representa a quantidade do primeiro recurso
5 representa a quantidade do segundo recurso
7 representa a quantidade do terceiro recurso

## Funcionamento
O programa simula múltiplos clientes (threads) que realizam solicitações e liberações de recursos de forma concorrente.

Cada cliente solicita recursos de maneira aleatória, respeitando sua necessidade máxima. Antes de conceder qualquer solicitação, o algoritmo do banqueiro verifica se o sistema continuará em um estado seguro.

Caso a solicitação leve o sistema a um estado inseguro, ela é negada.

Além disso, o acesso às estruturas compartilhadas é controlado por mutex, evitando condições de corrida e garantindo a consistência dos dados durante toda a execução.

Durante a execução, o estado do sistema é exibido no terminal, permitindo acompanhar a alocação de recursos e o comportamento das threads.