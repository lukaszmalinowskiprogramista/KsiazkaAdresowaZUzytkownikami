#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct Uzytkownik {
    int idUzytkownika;
    string nazwa, haslo;
};

struct Adresat {
    int idAdresata, idUzytkownika;
    string imie, nazwisko, numerTelefonu, email, adres;
};

void wczytajDaneUzytkownikowZPliku(vector <Uzytkownik> &uzytkownicy) {

    fstream plikUzytkownicy;
    Uzytkownik tymczasowyUzytkownik;
    string idUzytkownikaString;

    plikUzytkownicy.open("Uzytkownicy.txt",ios::in);
    if(plikUzytkownicy.good()==false) {
        cout<<"Brak Pliku z Danymi;";
        system("pause");

    } else {
        while(getline(plikUzytkownicy >> ws,idUzytkownikaString, '|')) {
            tymczasowyUzytkownik.idUzytkownika=stoi(idUzytkownikaString);
            getline(plikUzytkownicy,tymczasowyUzytkownik.nazwa, '|');
            getline(plikUzytkownicy,tymczasowyUzytkownik.haslo, '|');

            uzytkownicy.push_back(tymczasowyUzytkownik);
        }
    }
    plikUzytkownicy.close();
}

void zapiszDaneUzytkownikowDoPliku(vector <Uzytkownik> &uzytkownicy) {

    fstream plikUzytkownicy;
    plikUzytkownicy.open("Uzytkownicy.txt", ios::out);

    for(vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr<uzytkownicy.end(); itr++) {
        plikUzytkownicy<<(*itr).idUzytkownika   <<"|";
        plikUzytkownicy<<(*itr).nazwa           <<"|";
        plikUzytkownicy<<(*itr).haslo           <<"|"<<endl;;
    }
    plikUzytkownicy.close();
}

int wczytajDaneAdresatowZPliku(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika) {

    fstream plikAdresaci;
    Adresat tymczasowyAdresat;
    string idAdresataString, idUzytkownikaString;

    plikAdresaci.open("Adresaci.txt",ios::in);
    if(plikAdresaci.good()==false) {
        cout<<"Brak Pliku z Danymi;";
        system("pause");

    } else {
        while(getline(plikAdresaci >> ws,idAdresataString, '|')) {
            tymczasowyAdresat.idAdresata=stoi(idAdresataString);

            getline(plikAdresaci,idUzytkownikaString, '|');
            tymczasowyAdresat.idUzytkownika=stoi(idUzytkownikaString);

            getline(plikAdresaci,tymczasowyAdresat.imie, '|');
            getline(plikAdresaci,tymczasowyAdresat.nazwisko, '|');
            getline(plikAdresaci,tymczasowyAdresat.numerTelefonu, '|');
            getline(plikAdresaci,tymczasowyAdresat.email, '|');
            getline(plikAdresaci,tymczasowyAdresat.adres, '|');
            if(idZalogowanegoUzytkownika==tymczasowyAdresat.idUzytkownika)
                adresaci.push_back(tymczasowyAdresat);

        }
    }
    plikAdresaci.close();

    if(adresaci.empty())
        return 0;
    else
        return tymczasowyAdresat.idAdresata;
}

bool sprawdzCzyUzytkownikIstniejeJuzWBazie(vector <Uzytkownik> &uzytkownicy,string nazwaUzytkownikaDoSprawdzenia) {
    for(vector <Uzytkownik>::iterator itr=uzytkownicy.begin(); itr!=uzytkownicy.end(); itr++) {
        if((*itr).nazwa==nazwaUzytkownikaDoSprawdzenia)
            return 1;
    }
    return 0;
}

void  rejestracja(vector <Uzytkownik> &uzytkownicy) {

    Uzytkownik tymczasowyUzytkownik;

    cout << "Dodaj Uzytkownika" << endl;
    cout << "Podaj Nazwe: ";
    cin.ignore();
    getline(cin, tymczasowyUzytkownik.nazwa);

    cout << "Podaj Haslo: ";
    getline(cin, tymczasowyUzytkownik.haslo);

    if (uzytkownicy.empty())
        tymczasowyUzytkownik.idUzytkownika=1;
    else {
        tymczasowyUzytkownik.idUzytkownika= uzytkownicy.back().idUzytkownika+1;
        while(sprawdzCzyUzytkownikIstniejeJuzWBazie(uzytkownicy, tymczasowyUzytkownik.nazwa)) {
            cout << "Taki Uzytkownik Juz Istnieje. Podaj Inna nazwe: ";
            getline(cin, tymczasowyUzytkownik.nazwa);
        }
    }
    cout<<"Pomyslnie Dodano Uzytkownika"<<endl;
    system("pause");
    uzytkownicy.push_back(tymczasowyUzytkownik);
    zapiszDaneUzytkownikowDoPliku(uzytkownicy);
}

int logowanie(vector <Uzytkownik> &uzytkownicy) {

    string nazwa, haslo;
    cout<<"Podaj nazwe: ";
    cin.ignore();
    getline(cin, nazwa);

    for(vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr!=uzytkownicy.end(); itr++) {
        if ((*itr).nazwa==nazwa) {
            for(int proby=0; proby < 3; proby++) {
                cout<< "Podaj haslo: Pozostalo prob " << 3-proby<< ": ";
                getline(cin, haslo);
                if((*itr).haslo==haslo) {
                    cout<< "Zalogowales sie."<<endl;
                    system("pause");
                    return (*itr).idUzytkownika;
                }
            }
            cout<<"Podales 3 razy bledne haslo. Poczekaj 2 sekundy przed kolejna proba"<<endl;
            Sleep(3000);
            return 0;
        }
    }
    cout<<"Nie ma uzytkownika z takim loginem" <<endl;
    system("pause");
    return 0;
}

void wyszukajPoImieniuLubNazwisku(vector <Adresat> &adresaci, string wyborImieLubNazwisko) {
    string wyszukiwane;
    bool czyIstniejeTakiAdresat=0;
    if(wyborImieLubNazwisko=="imie")
        cout<<endl<< "Podaj Imie Do Wyszukania: ";
    else
        cout<<endl<< "Podaj Nazwisko Do Wyszukania: ";

    cin>>wyszukiwane;

    for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++) {
        if(((*itr).imie==wyszukiwane && wyborImieLubNazwisko=="imie") || ((*itr).nazwisko==wyszukiwane && wyborImieLubNazwisko=="nazwisko" )) {
            cout<< "Id: "               << (*itr).idAdresata       <<endl;
            cout<< "Imie: "             << (*itr).imie             <<endl;
            cout<< "Nazwisko: "         << (*itr).nazwisko         <<endl;
            cout<< "Numer Telefonu: "   << (*itr).numerTelefonu    <<endl;
            cout<< "email: "            << (*itr).email            <<endl;
            cout<< "Adres: "            << (*itr).adres            <<endl<<endl;
            czyIstniejeTakiAdresat=1;
        }
    }
    if(czyIstniejeTakiAdresat==0)
        cout<<"Taki Adresat Nie Wystepuje w bazie. Sprobuj Ponownie";
    system("pause");
}

void dodajAdresata(vector <Adresat> &adresaci,int idUzytkownika, int &idOstatniegoAdresata) {
    Adresat tymczasowyAdresat;

    idOstatniegoAdresata++;
    tymczasowyAdresat.idAdresata= idOstatniegoAdresata;
    tymczasowyAdresat.idUzytkownika=idUzytkownika;

    cout << "Dodaj Adresata" << endl;
    cout<< "Podaj Imie: ";
    cin.ignore();
    getline(cin, tymczasowyAdresat.imie);
    cout<< "Podaj Nazwisko: ";
    getline(cin,tymczasowyAdresat.nazwisko);
    cout<< "Podaj Numer Telefonu: ";
    getline(cin, tymczasowyAdresat.numerTelefonu);
    cout<< "Podaj email: ";
    getline(cin,tymczasowyAdresat.email);
    cout<< "Podaj Adres: ";
    getline(cin,tymczasowyAdresat.adres);

    adresaci.push_back(tymczasowyAdresat);
    fstream plikAdresaci;
    plikAdresaci.open("Adresaci.txt", ios::out| ios::app);
    plikAdresaci<<tymczasowyAdresat.idAdresata<<"|"<<tymczasowyAdresat.idUzytkownika<<"|"<<tymczasowyAdresat.imie <<"|"
                <<tymczasowyAdresat.nazwisko  <<"|"<<tymczasowyAdresat.numerTelefonu<<"|"<<tymczasowyAdresat.email<<"|"
                <<tymczasowyAdresat.adres     <<"|"<<endl;
    plikAdresaci.close();
    cout<<"Pomyslnie Dodano Adresata"<<endl;
    system("pause");
}

void wyswietlWszystkichAdresatow(vector <Adresat> &adresaci) {
    cout << "Wyswietl Wszystkich Adresatow" << endl;

    for(vector<Adresat>::iterator  itr=adresaci.begin(); itr!=adresaci.end(); itr++) {
        cout<< "Id: "               << (*itr).idAdresata       <<endl;
        cout<< "Imie: "             << (*itr).imie             <<endl;
        cout<< "Nazwisko: "         << (*itr).nazwisko         <<endl;
        cout<< "Numer Telefonu: "   << (*itr).numerTelefonu    <<endl;
        cout<< "email: "            << (*itr).email            <<endl;
        cout<< "Adres: "            << (*itr).adres            <<endl<<endl;
    }
    system("pause");
}

void usunAdresata(vector <Adresat> &adresaci) {
    int idDoUsuniecia=0;
    Adresat tymczasowyAdresat;
    string idAdresataString,idUzytkownikaString;

    wyswietlWszystkichAdresatow(adresaci);
    if (!adresaci.empty()) {
        fstream plikAdresaci, plikTymczasowy;

        cout<<"Podaj Id Adresata Do Usuniecia: ";
        cin>>idDoUsuniecia;

        for(vector<Adresat>::iterator itr=adresaci.begin(); itr<adresaci.end(); itr++) {
            if((*itr).idAdresata==idDoUsuniecia)
                adresaci.erase(itr);
        }

        plikAdresaci.open("Adresaci.txt", ios::in);
        plikTymczasowy.open("Tymczasowy.txt", ios::out);

        while(getline(plikAdresaci >> ws,idAdresataString, '|')) {
            tymczasowyAdresat.idAdresata=stoi(idAdresataString);

            getline(plikAdresaci,idUzytkownikaString, '|');
            tymczasowyAdresat.idUzytkownika=stoi(idUzytkownikaString);

            getline(plikAdresaci,tymczasowyAdresat.imie, '|');
            getline(plikAdresaci,tymczasowyAdresat.nazwisko, '|');
            getline(plikAdresaci,tymczasowyAdresat.numerTelefonu, '|');
            getline(plikAdresaci,tymczasowyAdresat.email, '|');
            getline(plikAdresaci,tymczasowyAdresat.adres, '|');

            if (tymczasowyAdresat.idAdresata!=idDoUsuniecia) {
                plikTymczasowy<<tymczasowyAdresat.idAdresata<<"|"<<tymczasowyAdresat.idUzytkownika<<"|"<<tymczasowyAdresat.imie <<"|"
                              <<tymczasowyAdresat.nazwisko  <<"|"<<tymczasowyAdresat.numerTelefonu<<"|"<<tymczasowyAdresat.email<<"|"
                              <<tymczasowyAdresat.adres     <<"|"<<endl;
            }
        }
        plikAdresaci.close();
        plikTymczasowy.close();

        remove("Adresaci.txt");
        rename("Tymczasowy.txt", "Adresaci.txt");

        cout<<"Pomyslnie Ukonczono Usuwanie"<<endl;
        system("pause");
    }
}

void edytujAdresata(vector <Adresat> &adresaci) {
    int wybor=0, idDoZmiany;
    string idAdresataString, idUzytkownikaString;
    vector<Adresat>::iterator itr;
    Adresat tymczasowyAdresat, adresatDoZamiany;

    if (adresaci.empty())
        return;

    wyswietlWszystkichAdresatow(adresaci);
    cout << "Podaj Id Adresata, ktorego dane maja zostac zmienione" << endl;
    cin >> idDoZmiany;

    for(vector<Adresat>::iterator itr=adresaci.begin(); itr<adresaci.end(); itr++) {
        while(wybor!=6 &&(*itr).idAdresata==idDoZmiany) {
            cout << "Wybierz Dane do zmiany:" << endl;
            cout<<"1 - imie"<<endl<<"2 - nazwisko"<<endl<<"3 - numer telefonu"<<endl<<"4 - email"<<endl<<"5 - adres"<<endl<<"6 - powrot do menu"<<endl;
            cin>> wybor;
            cin.ignore();
            if(wybor== 1) {
                cout<<"Podaj nowe imie: ";
                getline(cin,(*itr).imie);
            }
            if(wybor==  2) {
                cout<<"Podaj nowe nazwisko: ";
                getline(cin,(*itr).nazwisko);
            }
            if(wybor==  3) {
                cout<<"Podaj nowy numer telefonu: ";
                getline(cin,(*itr).numerTelefonu);
            }
            if(wybor==  4) {
                cout<<"Podaj nowy email: ";
                getline(cin,(*itr).email);
            }
            if(wybor==  5) {
                cout<<"Podaj nowy adres: ";
                getline(cin,(*itr).adres);
            }
            if(wybor==  6) {
                break;
            }
            adresatDoZamiany=(*itr);
        }
    }

    fstream plikAdresaci, plikTymczasowy;
    plikAdresaci.open("Adresaci.txt", ios::in);
    plikTymczasowy.open("Tymczasowy.txt", ios::out);

    while(getline(plikAdresaci >> ws,idAdresataString, '|')) {
        tymczasowyAdresat.idAdresata=stoi(idAdresataString);

        getline(plikAdresaci,idUzytkownikaString, '|');
        tymczasowyAdresat.idUzytkownika=stoi(idUzytkownikaString);

        getline(plikAdresaci,tymczasowyAdresat.imie, '|');
        getline(plikAdresaci,tymczasowyAdresat.nazwisko, '|');
        getline(plikAdresaci,tymczasowyAdresat.numerTelefonu, '|');
        getline(plikAdresaci,tymczasowyAdresat.email, '|');
        getline(plikAdresaci,tymczasowyAdresat.adres, '|');

        if (tymczasowyAdresat.idAdresata==idDoZmiany) {
            plikTymczasowy<<adresatDoZamiany.idAdresata<<"|"<<adresatDoZamiany.idUzytkownika<<"|"<<adresatDoZamiany.imie <<"|"
                          <<adresatDoZamiany.nazwisko  <<"|"<<adresatDoZamiany.numerTelefonu<<"|"<<adresatDoZamiany.email<<"|"
                          <<adresatDoZamiany.adres     <<"|"<<endl;
        } else {
            plikTymczasowy<<tymczasowyAdresat.idAdresata<<"|"<<tymczasowyAdresat.idUzytkownika<<"|"<<tymczasowyAdresat.imie <<"|"
                          <<tymczasowyAdresat.nazwisko  <<"|"<<tymczasowyAdresat.numerTelefonu<<"|"<<tymczasowyAdresat.email<<"|"
                          <<tymczasowyAdresat.adres     <<"|"<<endl;
        }
    }
    plikAdresaci.close();
    plikTymczasowy.close();

    remove("Adresaci.txt");
    rename("Tymczasowy.txt", "Adresaci.txt");

    cout<<"Pomyslnie Ukonczono Zmiane"<<endl;
    system("pause");
}

void zmianaHasla(vector <Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika) {
    string haslo;

    cout<<"Podaj nowe haslo: ";
    cin.ignore();
    getline(cin, haslo);

    for(vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr<uzytkownicy.end(); itr++)
        if ((*itr).idUzytkownika==idZalogowanegoUzytkownika)
            (*itr).haslo=haslo;

    zapiszDaneUzytkownikowDoPliku(uzytkownicy);
    cout<<"Zmieniono Haslo"<<endl;
    system("pause");
}

int main() {
    vector<Adresat> adresaci;
    vector<Uzytkownik> uzytkownicy;
    int wybranaOpcja, idOstatniegoAdresata;
    int idZalogowanegoUzytkownika = 0;

    wczytajDaneUzytkownikowZPliku(uzytkownicy);

    while(1) {
        if(idZalogowanegoUzytkownika == 0 ) {

            system("cls");
            cout << "KSIAZKA ADRESOWA"  <<endl;
            cout<<"1. Rejestracja"      <<endl;
            cout<<"2. Logowanie"        <<endl;
            cout<<"9. Zakoncz program"  <<endl;
            cout << "TWOJA wybrana Opcja: ";

            cin >> wybranaOpcja;

            if ( wybranaOpcja == 1)
                rejestracja(uzytkownicy);
            else if (wybranaOpcja == 2) {
                idZalogowanegoUzytkownika=logowanie(uzytkownicy);
                idOstatniegoAdresata=wczytajDaneAdresatowZPliku(adresaci, idZalogowanegoUzytkownika);
            } else if (wybranaOpcja == 9)
                exit(0);
        } else {

            system("cls");

            cout << "KSIAZKA ADRESOWA"                  << endl;
            cout << "1. Dodaj adresata"                 << endl;
            cout << "2. Wyszukaj po imieniu"            << endl;
            cout << "3. Wyszukaj po nazwisku"           << endl;
            cout << "4. Wyswietl wszystkich adresatow"  << endl;
            cout << "5. Usun adresata"                  << endl;
            cout << "6. Edytuj adresata"                << endl;
            cout << "7. Zmiana hasla"                   << endl;
            cout << "8. Wylogowanie"                    << endl;
            cout << "TWOJA wybrana Opcja: ";

            cin >> wybranaOpcja;

            if (wybranaOpcja == 1){
                dodajAdresata(adresaci, idZalogowanegoUzytkownika, idOstatniegoAdresata);}
            else if (wybranaOpcja == 2)
                wyszukajPoImieniuLubNazwisku(adresaci, "imie");
            else if (wybranaOpcja == 3)
                wyszukajPoImieniuLubNazwisku(adresaci, "nazwisko");
            else if (wybranaOpcja == 4)
                wyswietlWszystkichAdresatow(adresaci);
            else if (wybranaOpcja == 5)
                usunAdresata(adresaci);
            else if (wybranaOpcja == 6)
                edytujAdresata(adresaci);
            else if ( wybranaOpcja == 7)
                zmianaHasla(uzytkownicy,idZalogowanegoUzytkownika);
            else  if (wybranaOpcja == 8) {
                idZalogowanegoUzytkownika=0;
                adresaci.clear();
            }
        }
    }
    return 0;
}
