# Trabalho 1 - Montador

Este trabalho consiste em implementar um montador para a linguagem assembly simplificada especificada em sala de aula. O montador é capaz de realizar as fases de análise e síntese de códigos escritos na linguagem assembly inventada.

O montador produz um arquivo pre-processado (extensão .pre) e um arquivo objeto (extensão .obj), caso a montagem seja bem sucedida, ou mostra os erros detectados de montagem, caso ela não seja bem sucedida.

# Autores

Giovanni M Guidini - 16/0122660
Thiago Veras Machado - 16/0146682

# Compilando o projeto

Recomendamos utilizar o sistema operacional Linux para rodar o programa.

## Compilando o tradutor
O arquivo principal do projeto (onde está a `main`) é o arquivo `tradutor.cpp`, na pasta `src`. Assumindo que você esteja no root da pasta do projeto, para compilar (e ligar) o montador utilize o comando abaixo.

``` bash
$ g++ -std=c++11 -o tradutor src/tradutor.cpp
```

Isso deverá gerar o executável `tradutor`. Assumindo que você esteja usando o sistema Linux, para executar o programa utilize o comando abaixo.

``` bash
$ ./tradutor /path/to/file.asm
```

## Compilando os testes automáticos
Foram feitos diversos testes unitários para testar as partes do programa, conforme foram sendo implementadas. Esses testes estão na pasta `test/unit_tests`. Para compilar e executar todos os testes, siga os passos abaixo.

``` bash
$ cd test/unit_tests
$ g++ -std=c++11 -o all_tests test_all.cpp
$ ./all_tests
```