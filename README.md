# arduino

Software für die Arduino Steuerung von

@tobiasmueller
@jukirsch

## Simple Parallel State Machines
Die Statemaschinen der 3 Motoren laufen unabhängig von einander innerhalb der loop.

```
void loop() 
{
	rotator.update();
	trolley.Update();
	winch.update();
}
```

## Testboard


## Befehle (Serielle Schnittstelle):
~~
Befehle können über die serielle Schnittstelle (9600 Baudrate) gesendet werden. Diese werden mit einem ";" terminiert.

MOVE-Befehl 
MOVE <id> <steps>;
Dieser Befehl sorgt dafür, dass im derzeitigen Testaufbau eine LED blinkt. 
Die ID ist zweistellig anzugeben (Derzeit 00, 01, 02).
Steps gibt an, wie oft eine bestimmte Tätigkeit ausgeführt werden soll, im Testaufbau gibt Steps die Anzahl an Blinkvorgängen an.

Beispiele:
MOVE 00 1000;
STOP;
~~