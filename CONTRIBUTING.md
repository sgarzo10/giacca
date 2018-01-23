# Istruzioni per i collaboratori
All'interno della caratella Sketch_Giacca è contenuto il file sketch_giacca.ino nel quale è contenuto il codice che viene caricato sull'Arduino Nano. Questo skecth recupera la temperatura e l'umidità attraverso il sensore DHT11, legge i dati che arrivano in Bluetooth e in base alla stringa ricevuta esegue una determinata funzione. Tutte le informazioni recuperate sia dal Bluetooth che dal sensore DHT11 vengono mostrate in output su un display OLED. Di seguito vengono spiegate le singole funzioni presenti nello sketch e il significato delle varibili utilizzate:
* # setup() 
  Inizializzazione delle varibili utilizzate, mostra una pagina di avvio sul display OLED.
* # loop()
  Esegue la read sul Bluetooth, aggiorna la data, mostra la pagina sul display, ogni x secondi legge i valori dalla DHT11.
* # AGGIORNA_DATA()
  Aggiorna la data e l'ora che vengono mostrate sull OLED, utilizza le variabili di sistema di Arduino (day, month, year, ecc.).
* # read_DHT11()
  Esegue la read dalla DTH11.
* # initPage(int cursorX, int cursorY)
  Esegue le operazioni comuni a tutte le pagine: pulizia display, set text size, set text color, set cursor. I due parametri vengono passati alla funzione setCursor.
* # PageStart()
  Mostra la pagina di avvio dell'Arduino.
* # PageOra()
  Mostra la pagina con la data e l'ora.
* # PageHum()
  Mostra la pagina con l'umidità.
* # PageTemp()
  Mostra la pagina con la temperatura.
* # PageGPS()
  Mostra la pagina con la i dati del GPS ricevuti dal telefono in bluetooth.
* # PageNota()
  Mostra una nota che può essere settata attraverso il bluetooth.
* # DisplayPage()
  In base al valore dell'indice decide che pagina mostrare.
* # BLUETOOTH_READ()
  Legge i dati che arrivano dal bluetooth e li mette in un buffer, riscrive in bluettoth il messaggio ricevuto.
* # uint8_t castMonth(String s)
  Esegue la conversione del mese da stringa a intero.
* # set_Posizione(String s)
  Setta le variabili della posizione con i valori ricevuti dal bluettoth.
* # set_DataOra(String s)
  Setta le variabili della data e dell'ora con i valori ricevuti dal bluetooth, una volta settate l'Arduino le manterrà aggiornate senza necessitare di una risincronizzazione.
* # BLUETOOTH_COMMAND()
  In base alla stringa ricevuta in input dal bluetooth decide quale funzione richiamare.
