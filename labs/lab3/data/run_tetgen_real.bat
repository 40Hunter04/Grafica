@echo off
chcp 65001 > nul
title cmd - tetgen.exe -fenv stars_seleznev.node
mode con: cols=95 lines=32
cls
cd /d "%~dp0"
echo %CD%^> tetgen.exe -fenv stars_seleznev.node
tetgen.exe -fenv stars_seleznev.node
echo.
echo %CD%^> _
pause > nul
