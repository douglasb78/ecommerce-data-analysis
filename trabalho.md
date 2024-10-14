1. Definição do contexto a ser explorado:

contexto dos dados é: comportamento no e-commerce de uma loja de vários departamentos (“eCommerce behavior data from multi category store”).

O dataset para trabalho de organização de arquivos está em: "https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-data-from-multi-category-store".

2. Montagem dos arquivo de dados

Você deve criar no mínimo dois arquivos com esses dados: arquivo de produtos, e arquivo de acesso ao e-commerce.
Como a organização de arquivos definida é sequencial, os arquivos devem estar ordenados por algum dos campos, preferencialmente o campo identificador (chave)

Assim, as seguintes tarefas deverão ser realizadas:

Escolher os arquivos que serão criados com o dataset fornecido.

Cada arquivo deve ter pelo menos 3 campos (colunas) de informações.)

Um dos campos com dados não repetidos (o campo da chave), e pelo menos um dos campos com informações repetidas

Definir duas ou três perguntas que se poderia fazer a esse conjunto de dados

Ordenar os dados do arquivo de dados pelo campo chave (que não tem dados repetidos).

É possível gerar as chaves pelo incremento de um número sequencial se não houver um identificador único.

Os arquivos de dados devem ser criados em modo binário (não textual).

Organização e registros do Arquivo de Dados:

Os registros dos arquivos de dados devem ser de tamanho fixo.

Inserir espaços em branco no fim se necessário.

Cada linha do arquivo é encerrada com com o caracterere '\n'

A implementação deve ser feita em uma linguagem de programação (C, C#, C++, Python, PHP, Java ...) que possua o comando seek ou similar.

Implementar, para cada arquivo de dados:

uma função para inserir os dados: explicar como os dados foram ordenados (se for
o caso) e inseridos.

uma função para mostrar os dados

uma função para realizar a pesquisa binária

uma função para consultar dados a partir da pesquisa binária


Deverão ser construídos 2 índices, um para cada arquivo (salvos em arquivo no final da execução de um programa, e carregados quando o programa for aberto).

Índices em arquivo:

Implemente um arquivo de índice parcial para o campo chave de cada arquivo de dados
de acordo com a descrição do índice de arquivo da organização sequencial-indexado.

Implemente uma função de consulta a partir deste índice usando a pesquisa binária
para pesquisar no arquivo de índice e, depois o comando seek para pesquisar no arquivo
de dados.

3. Inserção/remoção de dados em um dos arquivos de dados, e reconstrução do índice:

Como será gerenciada a inserção de um novo registro no arquivo de dados?

Como será gerenciada a remoção de um registro no arquivo de dados?

4. Postar no AVA

Descrição dos arquivos de dados e descrição dos arquivos de índices.

Link para o projeto no GiT Hub, onde deve estar: o código-fonte da implementação, os arquivos de dados, os arquivos de índices gerados para aqueles dados.

5. Avaliação

O trabalho vale 10 pontos e será avaliado conforme o cumprimento das atividades propostas e a utilização de boas práticas de programação.

Não é permitido o uso da memória RAM para armazenar todos (ou grande parte) dos registros do arquivo de dados para efetuar as buscas.

Devem ser trazidos para a memória apenas os dados necessários.

Todas as operações solicitadas devem ser executadas no arquivo de dados armazenado em memória secundária
