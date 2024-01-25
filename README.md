# Projekt Współdzielonego Kalendarza Sieciowego (serwer)
##Temat Zadania
Projekt współdzielonego kalendarza sieciowego polega na stworzeniu systemu umożliwiającego użytkownikom zarządzanie wydarzeniami w kalendarzu poprzez interfejs graficzny (GUI), przy jednoczesnej synchronizacji danych z serwerem.

##Opis Protokołu Komunikacyjnego
Protokół komunikacyjny pomiędzy klientem a serwerem opiera się na modelu klient-serwer, przy użyciu technologii sieciowych. Klient GUI JavaFX komunikuje się z serwerem C++ za pomocą połączeń TCP/IP, wymieniając dane dotyczące wydarzeń kalendarzowych. Został zdefiniowany prosty protokół komunikacyjny w formie tekstowej. Żądania to proste komunikaty oddzielane spacjami, gdzie pierwszy łańcuch znaków powinien być tokenem autoryzacyjnym (wyjątkiem jest żądanie LOGIN, które odpowiada za wygenerowanie tokena), drugi to typ komunikatu, a kolejne to w zależności od komunikatu parametry. Odpowiedzi serwera są skonstruowane w ten sam sposób. Przykładowe komunikaty:
* LOGIN \<username> \<password> - zalogowanie użytkownika
* \<token> LOGOUT - wylogowanie użytkownika
* \<token> ADD_EVENT \<event_title> \<event_id> \<event_start_time> \<event_end_time>
* ...

##Opis Implementacji
Serwer C++ (kaledarz_siec)\
DbLogic.cpp, DbLogic.h: Klasy odpowiedzialne za operacje na bazie danych, w tym dodawanie, aktualizowanie i usuwanie wydarzeń.\
server.cpp: Główny moduł serwera, który obsługuje połączenia sieciowe, zarządza żądaniami od klientów i komunikuje się z bazą danych.\
Sposób Kompilacji, Uruchomienia i Obsługi Programów Projektu\
Obsługa Programów\
Klient GUI JavaFX oferuje graficzny interfejs użytkownika do logowania, przeglądania kalendarza i zarządzania wydarzeniami.\
Serwer C++ nasłuchuje na połączenia od klientów i zarządza wymianą danych między bazą danych a klientami.\
Uwagi Końcowe\
Należy dostosować parametry konfiguracyjne serwera i klienta zgodnie z potrzebami środowiska uruchomieniowego. Wymagane mogą być również dodatkowe kroki, takie jak konfiguracja bazy danych.\

## Zagadnienia techniczne
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
