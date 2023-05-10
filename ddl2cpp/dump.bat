pg_dump.exe -U postgres homeless > homeless.ddl
python3 ddl2cpp.py homeless.ddl homeless-db db