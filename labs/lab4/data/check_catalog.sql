.mode box
.header on
SELECT count(*) AS TotalStars FROM hyg;

SELECT substr(Spectrum, 1, 1) AS [Спектр. класс], count(*) AS [Число звёзд]
FROM hyg
WHERE substr(Spectrum, 1, 1) IN ('O', 'B', 'A', 'F', 'G', 'K', 'M')
GROUP BY [Спектр. класс]
ORDER BY count(*) DESC;
