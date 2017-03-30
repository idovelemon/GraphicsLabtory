@echo off
cd d:\
cd python
cd python27

set input=
set /p input=Please input file's name:

:main
python cpplint.py %~dp0%input%
set wait=
set /p wait=press any key to re-check
cls
goto main
