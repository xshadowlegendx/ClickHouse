SET enable_analyzer = 1;

SELECT toUInt64(NULL) AS x FROM (SELECT 1) HAVING x IN
    (SELECT NULL FROM (SELECT x IN (SELECT x IN (SELECT 1), x IN (SELECT 1) FROM (SELECT 1 WHERE x IN (SELECT NULL FROM (SELECT NULL))))));
