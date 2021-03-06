SELECT TOP 4 VERSION_ID 
  FROM DATA_SO
 WHERE VERSION_ID >= 'IPI-' + CONVERT(CHAR(6), DATEADD(DAY, -20, GETDATE()),12) + '-M-001'
 GROUP BY VERSION_ID
 ORDER BY VERSION_ID DESC


SELECT MACHINE_CD 
  FROM DATA_SO 
 WHERE VERSION_ID = 'IPI-170211-M-001'
   AND ZONE_CD = 'Z003'
 GROUP BY MACHINE_CD 
 ORDER BY MACHINE_CD


/*
UPDATE DATA_SO
   SET OSND_CNT_QTY = 2
 WHERE SO_ID = 1778

GO
*/


SELECT A.SO_ID, A.VERSION_ID, A.RESOURCE_CD, A.ZONE_CD, A.MACHINE_CD, A.SORT_KEY, 
       A.START_DATE, A.END_DATE, A.MOLD_ID, 
       A.CNT_QTY, A.OSND_CNT_QTY, 
       B.RST_CNT, B.RST_OSND_CNT,
       B.RST_START_DATE, B.RST_END_DATE
  FROM DATA_SO A
       LEFT  OUTER JOIN (
                        SELECT SO_ID, 
                               SUM(CASE REASON WHEN '00' THEN CNT_QTY ELSE 0 END) AS RST_CNT, 
                               SUM(CASE REASON WHEN '00' THEN 0 ELSE CNT_QTY END) AS RST_OSND_CNT,
                               MIN(START_DATE) AS RST_START_DATE,
                               MAX(END_DATE) AS RST_END_DATE
                          FROM DATA_RST
                         WHERE VERSION_ID = 'IPI-170211-M-001'
                           AND ZONE_CD = 'Z003'
                           AND MACHINE_CD IN ('MCA16','MCA17','MCA18')
                         GROUP BY SO_ID
                        ) B   ON A.SO_ID = B.SO_ID
 WHERE A.VERSION_ID = 'IPI-170211-M-001'
   AND A.ZONE_CD    = 'Z003'
   AND A.MACHINE_CD IN ('MCA16','MCA17','MCA18')
 ORDER BY A.VERSION_ID, A.RESOURCE_CD, A.SORT_KEY
