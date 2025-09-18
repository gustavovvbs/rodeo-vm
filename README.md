# rodeo-vm
vm de um touro mecanico aeeeepaaa


EBNF:

```
Program = { Statement } ;


Statement = Assignment | IfStmt | WhileStmt | Command ;


Assignment = Identifier "=" Expression ";" ;


IfStmt = "if" "(" Condition ")" "{" { Statement } "}"
[ "else" "{" { Statement } "}" ] ;


WhileStmt = "while" "(" Condition ")" "{" { Statement } "}" ;


Command = SpeedCmd ";" | TorqueCmd ";" | YawCmd ";" | BrakeCmd ";"
| WaitCmd ";" | PatternCmd ";" | SensorCmd ";" ;


SpeedCmd = "speed" "(" Expression ")" ; (* 0..100 *)
TorqueCmd = "torque" "(" Expression ")" ; (* 0..100 *)
YawCmd = "yaw" "(" Expression ")" ; (* graus por step; + eh sentido horario *)
BrakeCmd = "brake" "(" Expression ")" ; (* 0 off, 1 on *)
WaitCmd = "wait" "(" Expression ")" ; (* ms *)
PatternCmd = "pattern" "(" Mode ")" ; (* preset de movimento *)


SensorCmd = "read" "(" Sensor ")" "->" Identifier ; (* assign sensor value to var *)


Expression = Term { ("+" | "-" | "*" | "/") Term } ;
Term = Number | Identifier | "(" Expression ")" ;


Condition = Expression RelOp Expression ;
RelOp = "==" | "!=" | ">" | "<" | ">=" | "<=" ;


Mode = "CALM" | "SWIRL" | "AGGRESSIVE" ;
Sensor = "rider" | "tilt" | "rpm" | "emergency" | "time_ms" ;

Identifier = Letter { Letter | Digit | "_" } ;
Number = Digit { Digit } ;
Letter = "a" | ... | "z" | "A" | ... | "Z" ;
Digit = "0" | ... | "9" ;
```

exemplo nessa EBNF:

```
pattern(CALM);
speed(0);
brake(1);


read(rider) -> r;
if (r == 1) {
brake(0);
s = 0;


while (s < 80) {
s = s + 5;
speed(s);
torque(70);
yaw(3);
wait(200);


read(tilt) -> tilt;
if (tilt > 25) {
brake(1);
speed(0);
wait(300);
brake(0);
}


read(emergency) -> em;
if (em == 1) {
  s = 80; // force loop exit path
  speed(0);
  brake(1);
    }
  }
}


speed(0);
brake(1);
```
