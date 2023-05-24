<?xml version="1.0" encoding="UTF-8"?>
<sqls namespace="R32Table">
    <define id="fields">id, serial_number, dateTime, sensor_id, software_version, adc_value, concentration</define>

    <sql id="CreatTable">
        CREATE TABLE r32table (
            id INT IDENTITY(1,1) PRIMARY KEY,
            serial_number VARCHAR(50),
            dateTime VARCHAR(50),
            sensor_id VARCHAR(50) UNIQUE,
            software_version VARCHAR(50),
            adc_value INT,
            concentration FLOAT
        );
    </sql>

    <sql id="CheckTable">
        SELECT COUNT(*) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = 'r32table'
    </sql>

    <sql id="insert">
        INSERT INTO r32table (serial_number, dateTime, sensor_id, software_version, adc_value, concentration)
        VALUES (:serial_number, :dateTime, :sensor_id, :software_version, :adc_value, :concentration)
    </sql>

    <sql id="update">
        UPDATE r32table SET adc_value = :adc_value, concentration = :concentration WHERE sensor_id = :sensor_id
    </sql>

    <sql id="countBySensorId">
        SELECT COUNT(*) FROM r32table WHERE sensor_id = '%1'
    </sql>

    <sql id="datasCount">
        SELECT count(*) FROM r32table WHERE dateTime &gt;= '%1' and dateTime &lt;= '%2'
    </sql>

    <sql id="findByDateTime">
        SELECT <include defineId="fields"/> FROM r32table WHERE dateTime &gt;= '%1' and dateTime &lt;= '%2' ORDER BY dateTime
    </sql>

    <sql id="deleteByDateTime">
        DELETE FROM r32table WHERE dateTime &lt;= '%1'
    </sql>
</sqls>