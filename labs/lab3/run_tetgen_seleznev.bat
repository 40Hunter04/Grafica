@echo off
chcp 1251 > nul
echo [TETGEN 1.5] Генерация 3D сеток Делоне и Вороного для Селезнева М...
tetgen_cli\tetgen.exe -fenv data\stars_seleznev.node
echo [TETGEN 1.5] Готово! Созданы файлы .1.node, .1.edge, .1.face, .1.ele, .1.v.node
