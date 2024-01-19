# Serwer cpp

### Kompilacja

```
//g++ server.cpp -o server -pthread
g++ -o server server.cpp DbLogic.cpp -pthread -lpqxx -lpq
```
### Wymagane zależności

```
sudo apt install postgresql postgresql-contrib
sudo apt-get install libpqxx-dev
```

### Instalowanie skryptu bazy danych

```
sudo passwd postgres
sudo su postgres
psql -U postgres -q cal < ./database.sql
```

### Połączenie z bazą z aplikacji

```
CREATE ROLE garry WITH LOGIN PASSWORD 'mypassword';
GRANT ALL PRIVILEGES ON DATABASE cal TO garry;

sudo vim /etc/postgresql/12/main/pg_hba.conf
// zmien dla polaczen local z peer na md5
// restart service postgresql
```
