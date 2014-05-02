for i in {0..99}
do
    db_no=`printf '%02d' $i`
mysql -uroot -p1987922 -e "
CREATE DATABASE IF NOT EXISTS game_db_${db_no}"
    for j in {0..99}
    do
        table_no=`printf '%02d' $j`
        mysql -uroot -p1987922 -e "
SET NAMES UTF8;
CREATE TABLE IF NOT EXISTS game_db_${db_no}.base_info_table_${table_no} 
(
    userid  INT UNSIGNED NOT NULL PRIMARY KEY,
    nick    CHAR(16) NOT NULL DEFAULT ''
) ENGINE = INNODB CHARSET = UTF8;"

    echo $db_no.$table_no ok
    done
done
