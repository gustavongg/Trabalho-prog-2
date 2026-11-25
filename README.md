# Jogo do Milhao (FP2 - UTFPR)

Equipe:
- Gustavo Nicole Geraldo
- Joao Pedro Rodrigues Ribeiro
- Joao Pedro Damiao

Tema: Futebol, com um minigame de pescaria para pescar as perguntas.

## Como rodar

Precisa da [raylib](https://www.raylib.com) instalada (parte grafica).

No terminal, dentro desta pasta (no VS Code: Terminal > New Terminal):

```
make
make run
```

Para limpar: `make clean`. No Windows, use o gcc/make do w64devkit que fica em
`C:\raylib\w64devkit\bin` (no PowerShell: `$env:PATH = "C:\raylib\w64devkit\bin;" + $env:PATH`),
com a raylib em `C:\raylib\raylib\src`.
Rode sempre da raiz do projeto, porque o programa procura os arquivos em `dados/`.

### Instalar a raylib no Linux (Ubuntu/Debian)

```
sudo apt install build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev
git clone --depth 1 https://github.com/raysan5/raylib.git
cd raylib/src && make PLATFORM=PLATFORM_DESKTOP && sudo make install
```

## Arquivos

- `src/pergunta.c` - lista dinamica de perguntas (inserir, listar, pesquisar, alterar, excluir)
- `src/arquivo.c` - leitura e gravacao em CSV e binario
- `src/interface.c` - cores e sons do terminal
- `src/jogo.c` - regras da partida (niveis, sorteio, premios)
- `src/janela.c` - parte grafica (raylib): pescaria, CR7 e perguntas no estilo Genio Quiz
- `src/ranking.c` - ranking das partidas
- `dados/perguntas.csv` - 80 perguntas iniciais
