@echo off
chcp 1251 > nul
title Пакетный расчёт TetGen 1.5 - Селезнев Михаил (гр. 12002531)
color 0A
cls
cd /d "%~dp0"
echo ===============================================================================
echo   Пакетный расчёт 3D сеток Делоне и диаграмм Вороного (TetGen 1.5)
echo   Каталог HYG 4.0 (R ^<= 500 св. лет). Студент: Селезнев Михаил (гр. 12002531)
echo ===============================================================================
echo.
echo d:\ForWorkStudy\Projects\bsu\02_Компьютерное_моделирование_Васильев\students\seleznev\labs\lab4\data^> tetgen.exe -fenv O500ly.node
tetgen.exe -fenv O500ly.node
echo.
echo d:\ForWorkStudy\Projects\bsu\02_Компьютерное_моделирование_Васильев\students\seleznev\labs\lab4\data^> tetgen.exe -fenv B500ly.node
tetgen.exe -fenv B500ly.node
echo.
echo d:\ForWorkStudy\Projects\bsu\02_Компьютерное_моделирование_Васильев\students\seleznev\labs\lab4\data^> tetgen.exe -fenv A500ly.node
tetgen.exe -fenv A500ly.node
echo.
echo ===============================================================================
echo   Расчёт сеток Делоне и Вороного для классов завершён успешно!
echo ===============================================================================
pause
