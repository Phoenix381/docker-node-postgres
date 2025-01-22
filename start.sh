#!/bin/bash
echo "start.sh executing..."

cp /repo/postgres/src/tutorial/complex.so /var/lib/postgresql/scripts/complex.so

cd scripts
psql -U postgres -d postgres -f load-type.sql
# psql -U postgres -d postgres -a -f init-table.sql

psql -U postgres -d postgres -a -f edmonds-carp.sql
