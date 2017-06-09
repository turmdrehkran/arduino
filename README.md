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
