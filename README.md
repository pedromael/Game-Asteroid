# Game-Asteroid

## Descrição
Game-Asteroid é um jogo de nave espacial onde o jogador controla uma nave e deve destruir inimigos e meteoros enquanto coleta pacotes de itens.

## Funcionalidades
- Controle da nave do jogador usando o teclado.
- Criação e movimentação de inimigos e meteoros.
- Sistema de colisão entre objetos.
- Desenho de elementos na tela usando SDL2.
- Sistema de armas e balas.
- Implementação de escudo para a nave do jogador.
- Modo construtivo para criar defesas.
- Troca de armas durante o jogo.
- Robô metralhadora auxiliar.
- Disparo de bola de raio.

## Dependências
- SDL2
- SDL2_ttf
- SDL2_image
- SDL2_mixer

## Compilação e Execução
Para compilar e executar o projeto, siga os passos abaixo:

1. Certifique-se de ter as dependências instaladas.
2. Execute o comando `make` no terminal para compilar o projeto.
3. Execute o comando `./Asteroid` para iniciar o jogo.

## Estrutura do Projeto
- `main.c`: Arquivo principal que inicializa o jogo e contém o loop principal.
- `src/`: Contém os arquivos de implementação das funcionalidades do jogo.
  - [`control.c`](src/control.c): Implementa o controle da nave do jogador.
  - [`desenhar.c`](src/desenhar.c): Implementa as funções de desenho dos elementos na tela.
  - [`novos.c`](src/novos.c): Implementa a criação de novos inimigos, meteoros e pacotes.
  - [`verificador.c`](src/verificador.c): Implementa as funções de verificação e atualização do estado do jogo.
  - [`inicializador.c`](src/inicializador.c): Implementa a inicialização do jogo e suas variáveis.
- `header/`: Contém os arquivos de cabeçalho com as declarações das funções e estruturas.
  - [`control.h`](header/control.h)
  - [`desenhar.h`](header/desenhar.h)
  - [`novos.h`](header/novos.h)
  - [`verificador.h`](header/verificador.h)
  - [`inicializador.h`](header/inicializador.h)
  - [`index.h`](header/index.h)
  - [`quadTree.h`](header/quadTree.h)
- `files/`: Contém os arquivos de recursos como imagens e fontes.
  - `audio/`: Contém os arquivos de áudio.
  - `img/`: Contém os arquivos de imagem.
  - `Roboto-Light.ttf`: Fonte utilizada no jogo.
- `makefile`: Script para compilar o projeto.

## Controles do Jogo
- `Seta para cima`: Move a nave para cima.
- `Seta para baixo`: Move a nave para baixo.
- `Seta para esquerda`: Move a nave para a esquerda.
- `Seta para direita`: Move a nave para a direita.
- `Espaço`: Dispara a arma.
- `C`: Ativa o escudo.
- `V`: Cria uma parede defensiva (modo construtivo ativado).
- `B`: Alterna o modo construtivo.
- `N`: Cria um robô metralhadora.
- `M`: Dispara uma bola de raio.
- `TAB`: Troca de arma.

## Contribuição
Se você deseja contribuir para o projeto, sinta-se à vontade para abrir um pull request ou relatar problemas na seção de issues.

## Licença
Este projeto está licenciado sob a [MIT License](LICENSE).

