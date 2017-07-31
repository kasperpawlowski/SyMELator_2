# SyMELator_2
Instruments driver for SyMELator 2 flight simulator.

Author: Kasper Pawlowski   (kasperpawlowski@gmail.com)

Cooperation: Jan Nawrocki

Program przeznaczony jest do uruchomienia na platformie Arduino MEGA2560. 
Jego zadaniem jest odbiór chwilowych parametrów lotu szybowca, ich interpretacja i sterowanie silnikami krokowymi bipolarnymi (39BYGH405B) oraz serwomechanizmami (Tower Pro SG90) w celu odwzorowania parametrów lotu na tablicy przyrządów szybowca SZD-30 Pirat. Działa tylko z programem SyMELator 2 ze względu na konieczność dostarczania odpowiedniej ramki danych. 
Jeżeli chcesz uruchomić urządzenie nie posiadając programu SyMELator 2 na PC, skontaktuj się ze mną.

Zespół przyrządów pokładowych szybowca SZD-30 Pirat składa się z następujących elementów (w nawiasie uwzględniono jakiego rodzaju napęd 
został wykorzystany do sterowania danym elementem):
- wariometru do 5 m/s (silnik krokowy) - VARIO5
- prędkościomierza (silnik krokowy) - SPEED
- wysokościomierza (silnik krokowy dla wskazówki metrów oraz serwomechanizm dla wskazówki kilometrów) - ALT_M i ALT_KM
- wariometru do 30 m/s (silnik krokowy) - VARIO30
- busoli (silnik krokowy) - COMPASS
- zakrętomierza z chyłomierzem (dwa serwomechanizmy) - TURN i SLIP

Podsumowując, wykorzystywane jest 5 silników krokowych (20 linii sterujących) i 3 sewomechanizmy (3 linie sterujące).

Projekt został celowo wyeksportowany do githuba razem z plikami programu Atmel Studio, dzięki czemu posiadając ten program
na własnym komputerze, każdy użytkownik jest w stanie z łatwością go uruchomić i skompilować. 
W celu uruchomienia urządzenia należy wgrać program do pamięci mikrokontrolera (sposób flashowania mikrokontrolera AVR pozostawiam dla 
dociekliwych, choć polecam skorzystanie z zakładki Tools->External Tools programu Atmel Studio i avrdude). Następnie należy połączyć
Arduino z przygotowaną wcześniej płytką elektroniczną, zawierającą 5 podwójnych mostków H (np. L293D) i 3 wejścia dla serwomechanizmów.
Płytka musi mieć zasilanie silników krokowych i serwomechanizmów oddzielne od zasilania platformy Arduino! Oprócz przewodów sygnałowych
pamiętać należy o połączeniu mas (GND) Arduino i przygotowanej płytki elektronicznej! Następnie należy uruchomić program SyMELator 2
na PC oraz program symulacyjny (w chwili pisania README jest to CONDOR w wersji co najmniej obsługującej port UDP). Należy wybrać 
odpowiedni port COM, do którego podłączona jest płytka Arduino, rozpocząć transmisję danych i uruchomić symulację. 

WAŻNE:
Aby poprawnie zakończyć działanie urządzenia należy zakończyć transmisję danych oraz odczekać około 5 sekund na powrót wskazówek do
pozycji neutralnej i zapalenie czerwonej diody. Bezpośrednie odłączenie zasilania spowoduje, że przyrządy nie powrócą do pozycji neutralnej oraz konieczna będzie
ich kalibracja przy kolejnym uruchomieniu urządzenia. 
Nie należy również zbyt często celowo przerywać transmisji danych oraz restartować urządzenia. Każde działanie tego typu wymusza zapis
oraz odczyt pozycji neutralnej z pamięci nieulotnej, której żywotność producent ocenia na około 100 000 cykli, co daje około 6 000
cykli rozpoczęcia/zakończenia transmisji danych. Potem zajdzie konieczność wymiany platformy Arduino na nowy egzemplarz.

Kalibracja:
Urządzenie przechowuje stan kalibracji w pamięci nieulotnej, nie oznacza to jednak, że nie trzeba go w ogóle kalibrować. Do okresowej
kalibracji należy używać odpowiedniej zakładki w programie SyMELator 2 na PC, doprowadzając do neutralnego położenia (0) wskazówek
wszystkich przyrządów.

Sposób połączenia:
- GND płytki elektronicznej <-> GND Arduino
- USB PC <-> USB Arduino
- zasilanie płytki elektronicznej
- VARIO5:
  + INPUT1 <-> PA0 (22)
  + INPUT2 <-> PA1 (23)
  + INPUT3 <-> PA2 (24)
  + INPUT4 <-> PA3 (25)
- VARIO30:
  + INPUT1 <-> PA4 (26)
  + INPUT2 <-> PA5 (27)
  + INPUT3 <-> PA6 (28)
  + INPUT4 <-> PA7 (29)
- SPEED:
  + INPUT1 <-> PC4 (33)
  + INPUT2 <-> PC5 (32)
  + INPUT3 <-> PC6 (31)
  + INPUT4 <-> PC7 (30)
- ALT_M:
  + INPUT1 <-> PC0 (37)
  + INPUT2 <-> PC1 (36)
  + INPUT3 <-> PC2 (35)
  + INPUT4 <-> PC3 (34)
- ALT_KM:
  + SERVO_SIGNAL <-> PE3 (5)
- COMPASS:
  + INPUT1 <-> PL4 (45)
  + INPUT2 <-> PL5 (44)
  + INPUT3 <-> PL6 (43)
  + INPUT4 <-> PL7 (42)
- TURN:
  + SERVO_SIGNAL <-> PE4 (2)
- SLIP:
  + SERVO_SIGNAL <-> PE5 (3)
- POTENTIOMETER (opcjonalnie - do kalibracji wskazówki metrów wysokościomierza):
  + OUT <-> PF0 (A0)
- GREEN_LED (opcjonalnie - ułatwia diagnozowanie usterek, pamiętać o rezystorach):
  + LED_ANODE <-> PB0 (53)
- RED_LED (opcjonalnie - ułatwia diagnozowanie usterek, pamiętać o rezystorach):
  + LED_ANODE <-> PB1 (52)

Zakłada się, że każdy mostek H połączony jest z silnikiem krokowym w sposób następujący:
- OUTPUT1 <-> A (przewód biały)
- OUTPUT2 <-> A\ (przewód żółty)
- OUTPUT3 <-> B (przewód czerwony)
- OUTPUT4 <-> B\ (przewód niebieski)

Stan diod:
- GREEN_OFF, RED_OFF - urządzenie nie działa
- GREEN_ON, RED_OFF - poprawny odbiór danych
- GREEN_OFF, RED_ON - problem z transmisją danych, sprawdź połączenie PC <-> Arduino oraz poprawność działania programu SyMELator 2
- GREEN_ON, RED_ON - inicjalizacja urządzenia po nawiązaniu połączenia z PC
