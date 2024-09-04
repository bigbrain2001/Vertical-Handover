# Vertical-Handover

Scopul acestei aplicații este de a simula și colecta date în urma unui algoritm de Vertical Handover.
Utilizatorul își va putea seta anumiți parametri în rețea pentru a experimenta cu diferite situații.

## **Cuprins** ##
- [Tehnologii folosite](#tehnologii-folosite)
- [Pornirea aplicației](#pornirea-aplicației)

## Tehnologii folosite  ##
[![C/C++](https://img.shields.io/badge/C/C++-blue.svg?style=flat&logo=c++)](https://visualstudio.microsoft.com/vs/features/cplusplus/)

[![OMNET++](https://img.shields.io/badge/OMNET++-orange.svg)](https://omnetpp.org/download/)

## C/C++  ##
Acest proiect folosește compilatorul de C/C++.
Nu este necesar de instalat separat de simulatorul de OMNET++.

## OMNET++  ##
Acest proiect folosește OMNET++ 6.0.1.
Proiectul nu va funcționa pentru o versiune mai veche de OMNET++, iar pentru o versiune mai nouă pot apărea erori.
Dacă nu aveți OMNET++ 6.0.1 instalat, îl puteți descărca la următoarea adresă, secțiunea 6.0.1:
```shell
https://omnetpp.org/download/old
```
Instalarea simulatorului OMNET++ are loc după documentația de pe linkul: 
```shell
https://doc.omnetpp.org/omnetpp/InstallGuide.pdf
```
## Pornirea aplicației ##
Pentru a putea porni este nevoie de următorii pași:
```
Pasul 1: Descărcați proiectul de pe GitHub.
Pasul 2: Deschideți simulatorul de OMNET++ și deschideți fișierul VHO ca proiect din File -> Open Projects from File System...
Pasul 3: Creeați o simulare OMNET++ nouă selectând VerticalHandover.ned, Run -> Run As -> Omnet++ Simulation
pentru a compila sistemul.
Pasul 4: Simulați sistemul și alegeți configurarea dorită.
SAU
Pasul 4: După compilarea sistemului, selectați meniul Run -> Run Configurations iar în datele pentru fișierul de simulare a sistemului,
în Main->Config Name, alegeți configurarea dorită, APPLY și RUN.
```

