# Rodeo VM - Mechanical Bull Control Language

Uma linguagem de domínio específico (DSL) para controle de touros mecânicos, implementada com Flex, Bison e uma VM personalizada.

## Visão Geral

Rodeo VM é uma linguagem de programação projetada especificamente para controlar sistemas de touros mecânicos. A linguagem oferece comandos de alto nível para controle de movimento, leitura de sensores e implementação de lógicas de segurança.

## Comandos Principais

| Comando | Descrição | Exemplo |
|---------|-----------|---------|
| `speed(n)` | Define velocidade (0-100%) | `speed(75);` |
| `torque(n)` | Define torque (0-100%) | `torque(80);` |
| `yaw(n)` | Define rotação (graus/passo) | `yaw(10);` |
| `brake(n)` | Liga/desliga freio (0/1) | `brake(1);` |
| `wait(n)` | Aguarda n milissegundos | `wait(1000);` |
| `pattern(P)` | Define padrão de movimento | `pattern(CALM);` |

## Exemplos Disponíveis

1. **test_basic.rodeo** - Comandos básicos
2. **test_arithmetic.rodeo** - Operações matemáticas
3. **test_if_else.rodeo** - Condicionais
4. **test_while.rodeo** - Loops
5. **test_sensors.rodeo** - Leitura de sensores
6. **test_patterns.rodeo** - Padrões de movimento
7. **test_safety.rodeo** - Sistema de segurança

## Comandos Make

```bash
make          # Compila o projeto
make clean    # Remove arquivos gerados
make test     # Executa test.rodeo
```

## Estrutura do Projeto

```
rodeo-vm/
│
├── Implementação
│   ├── lexer.l                ✓ Analisador léxico (Flex)
│   ├── parser.y               ✓ Analisador sintático (Bison)
│   ├── ast.h / ast.c          ✓ Abstract Syntax Tree
│   ├── vm.h / vm.c            ✓ Virtual Machine
│   └── Makefile               ✓ Automação de build
│
├──  Testes
│   ├── test.rodeo             ✓ Teste principal
│   ├── run_all_tests.sh       ✓ Suite de testes
│   └── examples/              ✓ 7 exemplos demonstrativos
│       ├── test_basic.rodeo
│       ├── test_arithmetic.rodeo
│       ├── test_if_else.rodeo
│       ├── test_while.rodeo
│       ├── test_sensors.rodeo
│       ├── test_patterns.rodeo
│       └── test_safety.rodeo
│
└── 🔨 Build Artifacts
    └── rodeo-vm               ✓ Executável compilado
```


rodar os testes:

```bash
bash run_all_tests.sh
```
