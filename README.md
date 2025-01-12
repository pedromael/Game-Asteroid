# Game-Asteroid

## Descrição
Game-Asteroid é um jogo de tiro espacial onde o jogador controla uma nave e deve destruir inimigos e meteoros enquanto coleta pacotes de itens.

## Funcionalidades
- Controle da nave do jogador usando o teclado.
- Criação e movimentação de inimigos e meteoros.
- Sistema de colisão entre objetos.
- Desenho de elementos na tela usando SDL2.
- Sistema de armas e tiros.
- Implementação de escudo para a nave do jogador.

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
  - `control.c`: Implementa o controle da nave do jogador.
  - `desenhar.c`: Implementa as funções de desenho dos elementos na tela.
  - `novos.c`: Implementa a criação de novos inimigos, meteoros e pacotes.
  - `verificador.c`: Implementa as funções de verificação e atualização do estado do jogo.
- `header/`: Contém os arquivos de cabeçalho com as declarações das funções e estruturas.
- `files/`: Contém os arquivos de recursos como imagens e fontes.
- `makefile`: Script para compilar o projeto.

## Controles do Jogo
- `Seta para cima`: Move a nave para cima.
- `Seta para baixo`: Move a nave para baixo.
- `Seta para esquerda`: Move a nave para a esquerda.
- `Seta para direita`: Move a nave para a direita.
- `Espaço`: Dispara a arma.
- `C`: Ativa o escudo.

## Contribuição
Se você deseja contribuir para o projeto, sinta-se à vontade para abrir um pull request ou relatar problemas na seção de issues.

## Licença
Este projeto está licenciado sob a [MIT License](LICENSE).
