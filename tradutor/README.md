# Projeto 2
## Parte 1 - Tradutor

Este projeto é o tradutor de Assembly inventado para Assembly IA-32. A entrada do sistema deve ser um arquivo em assembly inventado extendido (com as instruções de input e output para hexadecimais, caracteres e strings), **sem macros**.

# Autores

Giovanni M Guidini - 16/0122660

Thiago Veras Machado - 16/0146682

# Compilando o projeto

Assumindo que você esteja na pasta `tradutor` (root do projeto), use o comando abaixo para compilar:

```bash
$ g++ -std=c++11 -o tradutor src/tradutor.cpp
```

E para executar o arquivo, utilize:
```bash
$ ./tradutor assembly_file.asm
```

A saída gerada será um arquivo de mesmo nome que o da entrada, porém com extensão `.s`. 
