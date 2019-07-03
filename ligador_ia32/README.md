# Projeto 2
## Parte 2 - Tradutor + Ligador

Este projeto traduz o assembly IA-32 para código de máquina e o formata em um arquivo ELF executável. A saída é um arquivo executável em sistemas Linux (note que pode haver diferença entre os sistemas Linux, então não há garantias que funcionará no seu computador). Utiliza a biblioteca ELFIO.

# Autores

Giovanni M Guidini - 16/0122660

Thiago Veras Machado - 16/0146682

# Compilando o projeto

Assumindo que você esteja na pasta `ligador_ia32` (root do projeto), use o comando abaixo para compilar (**lembre de colocar a path certa para o arquivo**):

```bash
$ g++ -std=c++11 -I/path/to/dir/ligador_ia32/lib/elfio/ -o ligador_ia32 src/ligador_ia32.cpp
```

E para executar o arquivo, utilize:
```bash
$ ./ligador_ia32 assembly_file.s
```

A saída gerada será um arquivo de mesmo nome que o da entrada, mas no formato elf.
