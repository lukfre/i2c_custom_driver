# i2c custom driver

Progetto di Sistemi Operativi *A.A. 2018/2019* del prof. Giorgio Grisetti

# What
Realizzazione driver `i2c` per la comunicazione *Master/Slave*  tra `Arduino` e `device i2c`.
In particolare sono state realizzate due versioni:
1. `i2c/basic`: primitive standard di comunicazione i2c.
    Per testare questi driver sono stati realizzati due main di prova *Master/Slave* tra due differenti ATMega
    *(codice disponibile nella cartella `examples/[master/slave])`*

2. `i2c/interrupt`: driver i2c realizzato con Interrupt ed utilizzato nel layer superiore `Comunication.h`
    per permettere la comunicazione **Multi-Slave**

Inoltre, è stato realizzato il driver per **device LCD** utilizzando il layer `comunication.h` sottostante.
Il driver permette:
- gestione diversi display (testati 16x2 e 20x4)
- Shifting dello schermo
- Creazione caratteri custom

Come esempio di funzionamento della comunicazione seriale e del driver i2c abbiamo realizzato un videogioco "liberamente ispirato" a Pacman.


| Master/Slave - 1 : 1| Master/Slave - 1 : n |
|--------------|---------------|
|              | my_lcd_i2c    |
|              | comunication  |
| i2c/basic    | i2c/interrupt |

![arduino](https://gitlab.com/I2Cteam/interrupt2catafano/raw/27e9d0c5b22a59b80bd93164a1ffe9bb2236cf44/img/i2c_videogame.jpg)

# How
Nella fase iniziale per comprendere a fondo il funzionamento del protocollo abbiamo realizzato la versione `Basic`.
Successivamente abbiamo realizzato la versione con interrupt e con il supporto al Multi-slave (quindi con separazione degli indirizzi sia a livello hardware che software).

Molte difficoltà *(e non solo)* sono nate per la realizzazione della comunicazione non standard, in particolare:

- Arduino: lettura disabilitando interrupt in modalità non bloccante 
- PC: Apertura UART in modo corretto e definizione primitive per la comunicazione
- Display: comprensione del funzionamento dello schermo a basso livello, in particolare dello shift

**N.B.** Lo schermo non viene refreshato ogni volta totalmente (per problemi di tempo) e viene fatto uno shift intero al display.
Il display però non mantiene un riferimento assoluto delle colonne, ma li cambia ad ogni shift. *(Ex. colonna 3 si muove ad ogni shift.)*


# How to Run
Per runnare è necessario compilare:
- `pc_server`
- `arduino client`
quindi:


```s
cd avr/arduino_client
make
make arduino_client.hex

cd ../../pc_server
make
./i2c_videogame 0 # 0 se device su ttyACMO, 1 se ttyACM1
```

Ora puoi divertirti :smirk:

## How the game works
Il core del gioco si trova su Arduino, mentre l'input dell'utente è gestito dalla tastiera del pc.
La comunicazione tra Arduino e PC viene effettuata tramite porta seriale.

Ecco i vari step del gioco:

1 - All'avvio il PC attende che Arduino sia pronto; una volta sincronizzati comparirà la scritta `start` su entrambi gli schermi.

2 - Tramite tastiera vengono inseriti i dati del Giocatore (*nome e difficoltà desiderata*).

3 - Arduino è indipendente dal pc, quindi il gioco prosegue anche se non si premono tasti sulla tastiera. A ogni step di refresh invia un messaggio di `life or death` al pc per comunicare lo stato del gioco.

4 - Quando il giocatore muore il punteggio viene stampato su entrambi gli schermi e Arduino rimane in attesa di una nuova partita.

# Authors 
- Francesco Colasante
- Luca Gioffrè
- Gianmarco D'Alessandro
- Alessandro Accardo
