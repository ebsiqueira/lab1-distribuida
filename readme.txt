Autor: Eduardo Borges Siqueira

Para compilar, basta rodar:
gcc cliente.c -o cliente && gcc servidor.c -o servidor -lpthread

Deve-se escolher entre os 3 possíveis comandos:
put, get e quit

put -> vai ser solicitado via terminal a chave e o valor
get -> vai ser solicitado via terminal a chave e retornado o valor
quit -> desconecta o cliente

Alguns padrões de projeto adotados:

Máximo de 10 chaves armazenadas, chaves de até 5 caracteres, valores de até 10 caracteres
