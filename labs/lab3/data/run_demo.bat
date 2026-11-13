@echo off
chcp 65001 > nul
title TetGen 1.5 CLI - [Селезнев Михаил, гр. 12002531]
color 0B
cd /d "%~dp0"
echo ===============================================================================
echo   TetGen 1.5 - Quality Tetrahedral Mesh Generator and 3D Delaunay Triangulator
echo   Лабораторная работа №3. Студент: Селезнев Михаил (гр. 12002531)
echo ===============================================================================
echo.
echo [COMMAND]: tetgen.exe -fenv stars_seleznev.node
echo.
..\tetgen_cli\Win32\Debug\tetgen.exe -fenv stars_seleznev.node
echo.
echo ===============================================================================
echo   Генерация завершена успешно! Файлы сеток Делоне и Вороного созданы.
echo ===============================================================================
pause
