"C:\Program Files\MariaDB\bin\mysqldump" -u root -p homeless --default-character-set=utf8 > "homeless.sql"
python3 ddl2cpp.py homeless.sql homeless-db db