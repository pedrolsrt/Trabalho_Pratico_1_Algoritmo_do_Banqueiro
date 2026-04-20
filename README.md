# Trabalho Prático 1 - Algoritmo do Banqueiro

## Autor
Pedro Lucas

## Disciplina
Sistemas Operacionais

## Professor
Lucas Bragança

---

## Descrição

Este projeto implementa o Algoritmo do Banqueiro em linguagem C utilizando múltiplas threads com a biblioteca Pthreads.

O objetivo é simular clientes que solicitam e liberam recursos concorrentemente, permitindo recursos apenas quando o sistema permanece em estado seguro, evitando deadlocks.

---

## Compilação

gcc -pthread main.c -o banqueiro

---

## Execução

./banqueiro 10 5 7

Os valores informados representam a quantidade inicial de recursos disponíveis no sistema.

---

## Funcionamento

Cada thread representa um cliente.

Os clientes realizam solicitações e liberações de recursos continuamente. Antes de conceder qualquer solicitação, o algoritmo verifica se o sistema continuará em estado seguro.

Caso contrário, a solicitação é negada.

---

## Sincronização

Foi utilizado mutex para controlar o acesso às estruturas compartilhadas, evitando condições de corrida e garantindo consistência dos dados.

---

## Estrutura do Projeto

Algoritmo_do_Banqueiro/
│── main.c
│── README.md
│── Trabalho_Pratico_1.pdf

---

## Considerações Finais

O projeto aplica conceitos importantes de Sistemas Operacionais, como concorrência, sincronização e prevenção de deadlocks.