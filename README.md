# ️C++ Arcade

![C++](https://img.shields.io/badge/C++-20-blue.svg) ![Raylib](https://img.shields.io/badge/Raylib-5.0-red.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

## Sobre o Projeto

O **C++ Arcade** é um projeto de estudo focado na implementação de mecânicas essenciais de *game development*. O objetivo não é apenas clonar jogos, mas entender os algoritmos e estruturas de dados que fazem cada um deles funcionar.

Este repositório serve como um "laboratório" onde cada jogo introduz um novo nível de complexidade, desde a movimentação simples até inteligência artificial e manipulação de matrizes.

---

## Os Jogos e Conceitos Aprendidos

Cada jogo nesta coleção foi escolhido para abordar um desafio técnico específico.

### 1. Tic-Tac-Toe (Jogo da Velha)
O clássico jogo de estratégia.
- **Principal Conceito:** **Algoritmo Minimax & Recursividade**.
- **Explicação:** A implementação da dificuldade "Difícil" utiliza o algoritmo Minimax. A IA simula todas as jogadas futuras possíveis recursivamente para escolher o movimento perfeito, tornando-a invencível. Também introduz o conceito de **Máquina de Estados** para gerenciar menus e telas de jogo.

### 2. Pong
O avô dos videogames.
- **Principal Conceito:** **Física Vetorial Básica & Colisão AABB**.
- **Explicação:** Introduz o loop de jogo fundamental e a física de rebatimento simples (inversão de velocidade nos eixos X/Y). Utiliza colisão entre Círculo (Bola) e Retângulo (Raquete) e matemática simples para calcular o ângulo de rebote baseado no ponto de impacto.

### 3. Snake (O Jogo da Cobra)
Um teste de reflexo e gerenciamento de espaço.
- **Principal Conceito:** **Estruturas de Dados (Deque) & Filas**.
- **Explicação:** Utiliza a estrutura `std::deque` (fila de duas pontas) para gerenciar o corpo da cobra. O movimento é uma ilusão criada ao adicionar uma nova cabeça na direção do movimento e remover a cauda (exceto quando ela come), demonstrando manipulação eficiente de contêineres dinâmicos.

### 4. Breakout
A evolução do Pong com elementos destrutíveis.
- **Principal Conceito:** **Gerenciamento de Entidades & Vetores Dinâmicos**.
- **Explicação:** Foca no gerenciamento de memória de múltiplos objetos. Os tijolos são armazenados em um `std::vector`, sendo iterados a cada frame para checagem de colisão e removidos da memória dinamicamente quando destruídos.

### 5. Space Invaders
Gerenciando o caos e múltiplos projéteis.
- **Principal Conceito:** **Comportamento de Enxame (Swarm Logic) & Timers**.
- **Explicação:** O desafio aqui é controlar um grupo de inimigos como uma unidade coesa. Se um alien toca a borda, todos descem e invertem a direção sincronizadamente. Também lida com o ciclo de vida de múltiplos projéteis simultâneos (do jogador e dos inimigos).

### 6. Tetris
O desafio final de lógica.
- **Principal Conceito:** **Matrizes 2D, Rotação & Lógica de Grid**.
- **Explicação:** O jogo mais complexo do pacote. Envolve a manipulação matemática de matrizes 4x4 para rotacionar as peças, verificação de colisão prévia (antes de mover) e o conceito de "Locking", onde uma entidade dinâmica se torna parte do cenário estático (o Grid).

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++ (Standard 20)
* **Biblioteca Gráfica:** [Raylib](https://www.raylib.com/) - Pela sua simplicidade e foco em codificação pura sem a necessidade de editores visuais pesados.
* **Build System:** CMake

---

## 🚀 Como Rodar o Projeto

### Pré-requisitos
* Compilador C++ compatível com C++20 (GCC, Clang ou MSVC).
* CMake (versão 3.20 ou superior).

### Instalação

1.  **Clone o repositório:**
    ```bash
    git clone [https://github.com/seu-usuario/cpp-arcade.git](https://github.com/seu-usuario/cpp-arcade.git)
    cd cpp-arcade
    ```

2.  **Gere os arquivos de build (o CMake baixará a Raylib automaticamente):**
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  **Compile:**
    ```bash
    cmake --build .
    ```

4.  **Execute:**
    * No Linux/Mac: `./cpp_arcade`
    * No Windows: `Debug/cpp_arcade.exe`

---

## ⌨️ Controles Gerais

O Arcade foi desenhado para ser intuitivo, mas aqui estão os controles padrão:

* **Menu Principal:** Use o teclado numérico ou digite o número para selecionar o jogo.
* **Movimentação:** `WASD` ou `Setas Direcionais`.
* **Ação (Tiro/Rotação):** `Espaço` ou `Cima`.
* **Pausa:** `P`.
* **Reiniciar:** `R` (nas telas de Game Over).
* **Voltar ao Menu:** `M` (nas telas de Game Over).

---

## 📄 Licença

Este projeto está sob a licença MIT. Sinta-se livre para usar o código para seus próprios estudos!