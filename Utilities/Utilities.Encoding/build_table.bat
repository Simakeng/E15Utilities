@echo off
echo Detecting Python..
for %%s in (python -V) do @SET PYTHON_VERSION=%%s
if %ERRORLEVEL% NEQ 0 exit
echo python installed!
echo version:
python -V
echo start generating
python %~dp0codetables\build_tables.py
echo generating finished
echo linking...