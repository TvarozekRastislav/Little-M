# **Little M.**
<div style="text-align:center">
    <img src="presentation/little_m_logo_no_bg.png"/>

Bakalársky projekt študenta na FEI STU Bratislava.<br> Jedná sa o návrh malého autonómneho robotického vozdila s orientáciou v interiery.
</div>

---

## **Princíp**
Základom projektu je myšlienka že takmer všade okolo nás sa už nachdáza wifi sieť. Pomocou určitých techník vieme zmerať silu wifi signálu na danom mieste.
Zisťovanie v akej miestnosti sa vozdilo nachádza je založené na zbere dát o rôznych silách signálov, rôznych wifi sietí(v projekte troch) a priradovania týmto wifi sietam aj "id" miestnosti. Následne sú dáta spracované a je nimi natrénovaný model s SVM kalsifikátorom teda ide o tzv. učenie s učiteľom. Pomocou openSource knižnice je následne tento model prevedený do jazyka C aby bol použiteľný vo vozidle ktoré vie pomocou tohoto modelu a sily signálu zistiť v akej miestnosti sa nachdáza.

Projekt je zložený z troch častí: 
- ### zariadenie na zber dát

- ### modelová časť
- ### vozidlo
---
### **Zariadenie na zber dát**
Skladá sa z USB Adaptéra pre ESP-01, ESP8266 ESP-01 a tlačidla ktoré je používané na sprístupnenie programovancieho módu.\
V kóde je potrebné zvoliť počet miestností, počet wifi sietí, ich mená, a koľko meraní má byť v každej miestnosti vykonaných.\
Kód je možné loggovať pomocou putty alebo iného softwaru.\
Pri nahrávaní kódu uvedieme modul do programovacieho módu tak že pri vkladaní do USB portu budeme držať tlačidlo.\
Pri programovaní musíme dbať na správne nastavenie výšky boaudov.\
**súčiastky**:
- Adaptér pre ESP-01
- ESP8266 ESP-01
- tlačidlo

**schéma**: 
- [schemas/zber_dat_png](https://github.com/TvarozekRastislav/Little-M./blob/main/schemas/zber_dat_png.png)


**kód**:
- [code_base/data_collection.ino](https://github.com/TvarozekRastislav/Little-M./blob/main/code_base/data_collection/data_collection.ino)

**fotka**:
- [hardware_pics/signal_collector.ino](https://github.com/TvarozekRastislav/Little-M./blob/main/hardware_pics/signal_collector.png)

---
### **Modelová časť**

---
### **Vozidlo**
