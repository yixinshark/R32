<?xml version="1.0" encoding="UTF-8"?>
<sqls namespace="ConsistencyLog">
    <define id="fields">id, dateTime, systemId, fileName, type, processMeans</define>

    <sql id="CreatTable">
                CREATE TABLE IF not EXISTS `consistencylog` (
                `id` int(11) NOT NULL AUTO_INCREMENT,
                `dateTime` int(11) NOT NULL,
                `systemId` varchar(255) NOT NULL,
                `fileName` varchar(255) NOT NULL,
                `type` varchar(255) DEFAULT NULL,
                `processMeans` varchar(255) DEFAULT NULL,
                PRIMARY KEY (`id`)
                )ENGINE=InnoDB DEFAULT CHARSET=utf8;
    </sql>

    <sql id="datasCount">
        SELECT count(*) FROM consistencylog WHERE dateTime &gt;= %1 and dateTime &lt;= %2
    </sql>

    <sql id="findAll">
        SELECT <include defineId="fields"/> FROM consistencylog
    </sql>
	
    <sql id="findByDateTime">
        SELECT <include defineId="fields"/> FROM consistencylog WHERE dateTime &gt;= %1 and dateTime &lt;= %2 order by dateTime limit %3, %4
    </sql>

    <sql id="insert">
        INSERT INTO consistencylog (dateTime, systemId, fileName, type, processMeans)
        VALUES (:dateTime, :systemId, :fileName, :type, :processMeans)
    </sql>

    <sql id="deleteByDateTime">
        DELETE FROM consistencylog WHERE dateTime &lt;= %1
    </sql>
</sqls>
