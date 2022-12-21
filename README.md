# Fundamentos de sistemas embarcados - TRABALHO 1

Repositório do TRABALHO 1 da disciplina de Fundamentos de Sistemas Embarcados (FGA-UnB).

## Servidor central

### Dependências
    python

Para executar o servidor central, utilize:

```bash
cd central_server
cd src
python central_server.py [IP] [PORTA]
```

## Servidor distribuido

### Dependências
    gcc 
    wiringPi
    make

```bash
cd distributed
make
bin/distributed
```

Assim que o servidor distribuido for conectado, vai pedir o caminho do arquivo de configuração json que deseja-se utilizar, nesse caso, é só colocar o caminho completo
para chegar no arquivo json:

'/home/dafnemoreira/distributed/src/configuracao_sala_01.json'

Pode-se inicializar os servidores em qualquer ordem, que eles estaram esperando para se conectar.


