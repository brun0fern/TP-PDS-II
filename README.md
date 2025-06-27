Twitter - Projeto PDS2
 - Visão Geral
Implementação em C++11 de um sistema de Twitter em linha de comando, desenvolvido para a disciplina de Programação e Desenvolvimento de Software 2. O projeto demonstra os princípios de orientação a objetos e boas práticas de programação em C++.

Funcionalidades Principais
Gerenciamento de Usuários
Criação de contas de usuário

Sistema de relacionamentos (seguir/deixar de seguir)

Perfis de usuário

Sistema de Publicações
Postagem de tweets (limite de 280 caracteres)

Retweets com comentários

Detecção automática de:

#hashtags

@menções

Sistema de Notificações contando com alertas para:

Novos seguidores

Menções em tweets

Interações

- Tecnologias e Padrões
Linguagem: C++11

Paradigma: Orientação a Objetos

Bibliotecas Principais:

Standard Template Library (STL)

<chrono> para manipulação de tempo

<memory> para smart pointers

 - Estrutura do Projeto
text
twitter-cli/
├── bin/            # Executáveis compilados
├── build/          # Objetos de compilação
├── include/        # Cabeçalhos
│   ├── User.hpp
│   ├── Tweet.hpp
│   ├── Notification/
│   │   ├── NotificationManager.hpp
│   │   └── ...
├── src/            # Implementação
│   ├── main.cpp
│   ├── User.cpp
│   ├── Tweet.cpp
│   └── ...
├── tests/          # Testes unitários
├── Makefile        # Build automation
└── README.md

-  Como Executar

Compilação
bash
make
Execução
bash
make run
Testes
bash
make test
- Documentação
Gere a documentação com:

bash
doxygen Doxyfile
Acesse docs/html/index.html para visualizar.

Obrigado Pela Atenção
