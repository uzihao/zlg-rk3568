"CREATE TABLE IF NOT EXISTS `alarm_info` ("
                   "`id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                   "`model` VARCHAR(128) DEFAULT NULL,"     // 型号
                   "`seq` VARCHAR(128) DEFAULT NULL,"       // 编号
                   "`rssi` TINYINT DEFAULT NULL,"           // RSSI(dbm)
                   "`freq` FLOAT DEFAULT NULL,"             // 频点
                   "`longtitude` FLOAT DEFAULT NULL,"       // 经度
                   "`latitude` FLOAT DEFAULT NULL,"         // 纬度
                   "`distance` FLOAT DEFAULT NULL,"         // 距离
                   "`height` FLOAT DEFAULT NULL,"           // 高度
                   "`altitude` FLOAT DEFAULT NULL,"         // 海拔
                   "`angle` FLOAT DEFAULT NULL,"            // 角度
                   "`elangle` FLOAT DEFAULT NULL,"          // 俯仰角
                   "`datetime` DATETIME DEFAULT NULL)");    // 时间



```bash
CREATE TABLE IF NOT EXISTS `alarm_info` (
				 `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, # INTEGER整数, PRIMARY KEY 主键列, AUTOINCREMENT 自动递增, NOT NULL不可为空。当插入新记录时不用显式为该列赋值，数据库会自己添加一个递增的ID
                   `model` VARCHAR(128) DEFAULT NULL, # VARCHAR(128)最大长度为128个字符的可变长度字符串, DEFAULT NULL 表示该列的默认值为空（即NULL），当插入新记录时如果没有显式为该列赋值，则会使用该默认值
                   `seq` VARCHAR(128) DEFAULT NULL,
                   `rssi` TINYINT DEFAULT NULL, # TINYINT 占用1字节存储空间的整数列
                   `freq` FLOAT DEFAULT NULL, 
                   `longtitude` FLOAT DEFAULT NULL,
                   `latitude` FLOAT DEFAULT NULL,
                   `distance` FLOAT DEFAULT NULL,
                   `height` FLOAT DEFAULT NULL,
                   `altitude` FLOAT DEFAULT NULL,
                   `angle` FLOAT DEFAULT NULL,
                   `elangle` FLOAT DEFAULT NULL,
                   `datetime` DATETIME DEFAULT NULL)
```

```bash
SELECT count(*) FROM alarm_info # 读数目
```

```bash
INSERT INTO alarm_info (model, seq, rssi, freq, longtitude, latitude, distance, height, altitude, angle, elangle, datetime) VALUES('%1', '%2', %3, %4, %5, %6, %7, %8, %9, %10, %11, '%12') # 插入数据
```

