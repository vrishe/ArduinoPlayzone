@echo off

for /r %%f in (*.o) do avr-objdump -gdSl -m avr %%f > %%~nf.s