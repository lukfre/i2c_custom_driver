# Arduino client
Bozza per la sincronizzazione fra pc_client e scheda arduino.

## utils
Per debuggare è abilitata la **printf()**, sia per *stdout*, che per *stderr*

## test
Per testare la sincronizzazione, flashare su arduino il file **arduino_client.c**, tramite il *Makefile* (assicurarsi che la scheda sia letta su `/dev/ttyACM0`):
```shell
$ make arduino_client.hex
```

e lanciare il **main**, in `/test UART/pc_server`.
