<Qucs Schematic 0.0.18>
<Properties>
  <View=-7,-37,980,753,0.75,0,3>
  <Grid=10,10,1>
  <DataSet=fancy_lights.dat>
  <DataDisplay=fancy_lights.dpl>
  <OpenDisplay=1>
  <Script=fancy_lights.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
  <.PortSym 40 20 5 0>
  <.PortSym 40 60 4 0>
  <.PortSym 40 100 1 0>
  <.PortSym 40 140 2 0>
  <.PortSym 40 180 3 0>
  <.PortSym 40 220 6 0>
  <.PortSym 40 260 7 0>
  <.PortSym 40 300 8 0>
</Symbol>
<Components>
  <Lib D1 1 250 80 -26 13 0 0 "LEDs" 0 "red" 0>
  <Lib D2 1 250 170 -26 13 0 0 "LEDs" 0 "yellow" 0>
  <Lib D3 1 250 260 -26 13 0 0 "LEDs" 0 "green" 0>
  <R R1 1 140 80 -26 15 0 0 "220 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R2 1 140 170 -26 15 0 0 "220 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R3 1 140 260 -26 15 0 0 "220 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <GND * 1 280 30 0 -16 1 0>
  <C C1 1 420 90 17 -26 0 1 "1 pF" 1 "" 0 "neutral" 0>
  <R R4 1 480 180 15 -26 0 1 "220 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <GND * 1 390 120 -16 0 1 1>
  <Port d2 1 480 250 12 4 0 1 "5" 0 "out" 0>
  <Port Vcc1 1 540 30 12 -23 1 3 "4" 0 "analog" 0>
  <Port D5 1 60 80 -23 12 0 0 "1" 0 "in" 0>
  <Port D6 1 60 170 -23 12 0 0 "2" 0 "in" 0>
  <Port D9 1 60 260 -23 12 0 0 "3" 0 "in" 0>
  <Port Vcc2 1 380 500 -23 12 0 0 "6" 0 "analog" 0>
  <R R5 1 490 500 -26 15 0 0 "1-251 KOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R6 1 790 400 15 -26 0 1 "10 KOhm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R7 1 720 590 -26 15 0 0 "300 KOhm" 0 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <_BJT KT315г 1 790 500 8 -26 0 0 "npn" 0 "1e-16" 0 "1" 0 "1" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "1.5" 0 "0" 0 "2" 0 "100" 0 "1" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0" 0 "0.75" 0 "0.33" 0 "0" 0 "0.75" 0 "0.33" 0 "1.0" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "0.0" 0 "26.85" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "1.0" 0 "1.0" 0 "0.0" 0 "0.0" 0 "3.0" 0 "1.11" 0 "26.85" 0 "1.0" 0>
  <GND * 1 790 650 0 0 0 0>
  <Port a1 1 680 650 12 4 0 1 "7" 0 "out" 0>
  <Port d3 1 900 650 12 4 0 1 "8" 0 "out" 0>
</Components>
<Wires>
  <170 80 220 80 "" 0 0 0 "">
  <170 170 220 170 "" 0 0 0 "">
  <170 260 220 260 "" 0 0 0 "">
  <280 170 280 260 "" 0 0 0 "">
  <280 80 280 170 "" 0 0 0 "">
  <60 80 110 80 "" 0 0 0 "">
  <60 170 110 170 "" 0 0 0 "">
  <60 260 110 260 "" 0 0 0 "">
  <280 30 280 80 "" 0 0 0 "">
  <540 30 540 60 "" 0 0 0 "">
  <420 60 540 60 "" 0 0 0 "">
  <540 110 570 110 "" 0 0 0 "">
  <540 60 540 110 "" 0 0 0 "">
  <420 120 570 120 "" 0 0 0 "">
  <570 110 570 120 "" 0 0 0 "">
  <480 130 570 130 "" 0 0 0 "">
  <480 130 480 150 "" 0 0 0 "">
  <480 210 480 250 "" 0 0 0 "">
  <570 120 570 130 "" 0 0 0 "">
  <390 120 420 120 "" 0 0 0 "">
  <380 500 430 500 "" 0 0 0 "">
  <430 350 430 500 "" 0 0 0 "">
  <430 500 460 500 "" 0 0 0 "">
  <520 500 680 500 "" 0 0 0 "">
  <430 350 790 350 "" 0 0 0 "">
  <790 350 790 370 "" 0 0 0 "">
  <680 590 690 590 "" 0 0 0 "">
  <750 590 790 590 "" 0 0 0 "">
  <680 500 680 590 "" 0 0 0 "">
  <790 530 790 590 "" 0 0 0 "">
  <680 500 760 500 "" 0 0 0 "">
  <790 430 790 450 "" 0 0 0 "">
  <790 450 790 470 "" 0 0 0 "">
  <790 450 900 450 "" 0 0 0 "">
  <900 450 900 650 "" 0 0 0 "">
  <680 590 680 650 "" 0 0 0 "">
  <790 590 790 650 "" 0 0 0 "">
  <900 650 900 660 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
  <Rectangle 80 40 230 290 #000000 1 2 #c0c0c0 1 0>
  <Text 210 310 12 #000000 0 "Light Emitter">
  <Rectangle 570 100 40 40 #00007f 2 1 #00007f 1 0>
  <Rectangle 400 40 230 190 #000000 0 2 #c0c0c0 1 0>
  <Text 540 210 12 #000000 0 "IR Receiver">
  <Rectangle 400 330 540 300 #000000 0 2 #c0c0c0 1 0>
  <Arrow 480 520 20 -40 8 3 #00007f 2 1 1>
  <Arrow 700 550 10 20 12 3 #00007f 2 1 1>
  <Arrow 710 550 10 20 12 3 #00007f 2 1 1>
  <Text 800 340 12 #000000 0 "Darkness Detector\n">
</Paintings>
