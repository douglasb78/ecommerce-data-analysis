1. Defini��o do contexto a ser explorado:

contexto dos dados �: comportamento no e-commerce de uma loja de v�rios departamentos (�eCommerce behavior data from multi category store�).

O dataset para trabalho de organiza��o de arquivos est� em: "https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-data-from-multi-category-store".

2. Montagem dos arquivo de dados

Voc� deve criar no m�nimo dois arquivos com esses dados: arquivo de produtos, e arquivo de acesso ao e-commerce.
Como a organiza��o de arquivos definida � sequencial, os arquivos devem estar ordenados por algum dos campos, preferencialmente o campo identificador (chave)

Assim, as seguintes tarefas dever�o ser realizadas:

Escolher os arquivos que ser�o criados com o dataset fornecido.

Cada arquivo deve ter pelo menos 3 campos (colunas) de informa��es.)

Um dos campos com dados n�o repetidos (o campo da chave), e pelo menos um dos campos com informa��es repetidas

Definir duas ou tr�s perguntas que se poderia fazer a esse conjunto de dados

Ordenar os dados do arquivo de dados pelo campo chave (que n�o tem dados repetidos).

� poss�vel gerar as chaves pelo incremento de um n�mero sequencial se n�o houver um identificador �nico.

Os arquivos de dados devem ser criados em modo bin�rio (n�o textual).

Organiza��o e registros do Arquivo de Dados:

Os registros dos arquivos de dados devem ser de tamanho fixo.

Inserir espa�os em branco no fim se necess�rio.

Cada linha do arquivo � encerrada com com o caracterere '\n'

A implementa��o deve ser feita em uma linguagem de programa��o (C, C#, C++, Python, PHP, Java ...) que possua o comando seek ou similar.

Implementar, para cada arquivo de dados:

uma fun��o para inserir os dados: explicar como os dados foram ordenados (se for
o caso) e inseridos.

uma fun��o para mostrar os dados

uma fun��o para realizar a pesquisa bin�ria

uma fun��o para consultar dados a partir da pesquisa bin�ria


Dever�o ser constru�dos 2 �ndices, um para cada arquivo (salvos em arquivo no final da execu��o de um programa, e carregados quando o programa for aberto).

�ndices em arquivo:

Implemente um arquivo de �ndice parcial para o campo chave de cada arquivo de dados
de acordo com a descri��o do �ndice de arquivo da organiza��o sequencial-indexado.

Implemente uma fun��o de consulta a partir deste �ndice usando a pesquisa bin�ria
para pesquisar no arquivo de �ndice e, depois o comando seek para pesquisar no arquivo
de dados.

3. Inser��o/remo��o de dados em um dos arquivos de dados, e reconstru��o do �ndice:

Como ser� gerenciada a inser��o de um novo registro no arquivo de dados?

Como ser� gerenciada a remo��o de um registro no arquivo de dados?

4. Postar no AVA

Descri��o dos arquivos de dados e descri��o dos arquivos de �ndices.

Link para o projeto no GiT Hub, onde deve estar: o c�digo-fonte da implementa��o, os arquivos de dados, os arquivos de �ndices gerados para aqueles dados.

5. Avalia��o

O trabalho vale 10 pontos e ser� avaliado conforme o cumprimento das atividades propostas e a utiliza��o de boas pr�ticas de programa��o.

N�o � permitido o uso da mem�ria RAM para armazenar todos (ou grande parte) dos registros do arquivo de dados para efetuar as buscas.

Devem ser trazidos para a mem�ria apenas os dados necess�rios.

Todas as opera��es solicitadas devem ser executadas no arquivo de dados armazenado em mem�ria secund�ria
