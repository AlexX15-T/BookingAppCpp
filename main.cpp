#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
#include <ctime>

using namespace std;
#define _CRT_SECURE_NO_WARNINGS

ofstream log("log.txt");

#define pret_masa 20

enum STATUS
{
    LIBERA, OCUPATA
};

enum STATUS_REZERVARE
{
    IN_ASTEPTARE, APROBATA
};

enum EVENIMENT
{
    EROARE, ALERTA, REZERVARE
};

typedef struct dt
{
    int an, luna, zi;
} data;

typedef struct rez
{
    string numeJudet;
    string numeHotel;
    int nrCamera;
    int nrZile;
    int nr_mese;
    data dataCazare;
    STATUS_REZERVARE status;

} rezervare;

void writeError(EVENIMENT a, string descriere)
{
    time_t now = time(0);
    string x = ctime(&now);
    string aux;

    if(a == EROARE)
        aux = "ERROR";

    if(a == ALERTA)
        aux = "ALERT";

    if(a == REZERVARE)
        aux = "REZERVATION";

    x = x + aux + " " + descriere;

    log << x << endl;

}

class Camera
{
private:
    int numar;
    double pretPerNoapte;
    double pretCazare;
    STATUS status;

public:

    Camera()
    {
        numar = 0;
        pretPerNoapte = 0;
        status = LIBERA;
    }

    Camera(int nr, double pret)
    {
        numar = nr;
        pret = pretPerNoapte;
        status = LIBERA;
    }

    void setNumar(int nr)
    {
        numar = nr;
    }

    void setPret(double pret)
    {
        pretPerNoapte = pret;
    }

    int getNumar()
    {
        return numar;
    }

    double getPret()
    {
        return pretPerNoapte;
    }

    double getCazare()
    {
        return pretCazare;
    }

    void setCazare(double x)
    {
        pretCazare = x;
    }

    STATUS getStatus()
    {
        return status;
    }

    virtual double calcCazare() = 0;
    virtual int getCapacitate() = 0;

};


class CameraDubla : public Camera
{

private:
    int capacitate;

public:

    CameraDubla() : Camera()
    {
        capacitate = 2;
    }


    CameraDubla(int nr, double pret) : Camera(nr, pret)
    {
        capacitate = 2;
    }


    double calcCazare()
    {
        setCazare(getPret() * 1.4);
        return getCazare();
    }

    int getCapacitate()
    {
        return capacitate;
    }


};

class CameraTripla : public Camera
{
private:
    int capacitate;

public:

    CameraTripla() : Camera()
    {
        capacitate = 3;
    }


    CameraTripla(int nr, double pret) : Camera(nr, pret)
    {
        capacitate = 3;
    }


    double calcCazare()
    {
        setCazare(this->getPret() * 1.8);
        return getCazare();
    }

    int getCapacitate()
    {
        return capacitate;
    }


};

class Camera4 : public Camera
{

private:
    int capacitate;

public:


    Camera4() : Camera()
    {
        capacitate = 4;
    }


    Camera4(int nr, double pret) : Camera(nr, pret)
    {
        capacitate = 4;
    }


    double calcCazare()
    {
        setCazare(this->getPret() * 2);
        return getCazare();
    }

    int getCapacitate()
    {
        return capacitate;
    }


};


//clasa abstracta hotel
class Hotel
{
private:
    string denumire;
    int clasificare;
    int spatii_de_cazare;
    vector < string > recenzii;
    vector < Camera* > camere;

public:
    //constructors
    Hotel()
    {
        denumire = "Unknown";
        clasificare = 0;
        spatii_de_cazare = 0;
    }

    Hotel(string nume)
    {
        denumire = nume;
        clasificare = 0;
        spatii_de_cazare = 0;
    }

    Hotel(const string denumire,const int clasificare, const int spatii_de_cazare)
    {

        this->denumire = denumire;

        if (clasificare && spatii_de_cazare >= 0)
        {
            this->clasificare = clasificare;
            this->spatii_de_cazare = spatii_de_cazare;
        }

        else
        {
            cout << "Clasificarea si numarul spatiilor de cazare nu pot fi negative";
        }


    }

    Hotel(const Hotel& old)
    {
        denumire = old.denumire;
        clasificare = old.clasificare;
        spatii_de_cazare = old.spatii_de_cazare;
    }

    //setters and getters

    string getDenumire()
    {
        return denumire;
    }

    void setDenumire(const string denumire)
    {
        this->denumire = denumire;
    }

    void setClasificare(const int clasificare)
    {
        if (clasificare >= 0)
        {
            this->clasificare = clasificare;
        }
        else
        {
            cout << "Clasificarea nu poate fi negativa";
        }
    }

    int getClasificare()
    {
        return clasificare;
    }

    int getCazari()
    {
        return spatii_de_cazare;
    }

    void setCazare(const int cazari)
    {
        if (cazari >= 0)
        {
            this->spatii_de_cazare = cazari;
        }
        else
        {
            cout << "Spatiile de cazari nu pot fi negative";
        }
    }

    int getSpatiuDeCazari()
    {
        return this->spatii_de_cazare;
    }

    void addCamera(Camera* a)
    {
        camere.push_back(a);
    }

    unsigned int nrCamere()
    {
        return camere.size();
    }

    unsigned int nrCamereLibere()
    {
        int cnt = 0;

        for(unsigned int i = 0; i < camere.size(); i++)
            if(camere[i]->getStatus() == LIBERA)
                cnt++;

        return cnt;
    }

    unsigned int nrCamereOcupate()
    {
        return nrCamere() - nrCamereLibere();
    }

    Camera* getCamera(unsigned int index)
    {
        return camere[index];
    }

    void generareRaport()
    {
        cout << "Gradul de ocupare al acestei unitati esre " << (nrCamereOcupate() / nrCamere()) * 100 << "%." << endl;

        double S = 0;
        int cnt = 0;

        for(unsigned int i = 0; i < camere.size(); i++)
        {
            if(camere[i]->getStatus() == OCUPATA)
            {
                S += camere[i]->getCazare();
                cnt ++;
            }

        }

        cout << "Pretul mediu din aceasta perioada este: " << S / cnt << endl;
    }


    void adaugareRecenzie(string recenzie)
    {
        recenzii.push_back(recenzie);
    }

    void displayRecenzii()
    {
        for(unsigned int i = 0; i < recenzii.size(); i++)
            cout << recenzii[i] << '\n';
    }

    void displayRooms()
    {
        for(unsigned int i = 0; i < camere.size(); i++)
            cout << "Camera are numarul " << camere[i]->getNumar() << " si are " << camere[i]->getCapacitate() << " locuri." << endl;
    }

    void displayHotel()
    {
        cout << " Hotelul se numeste " << denumire << " si dispune de " << spatii_de_cazare << " spatii de cazare.";
    }

    //destructors
    ~Hotel()
    {
        recenzii.clear();
        recenzii.shrink_to_fit();
    }

};


//functii pentru meniu
class Icons
{
public:
    void printMenu()
    {
        cout << "Meniu" << endl;
    }

    void printMenuForAdmins()
    {
        cout << "Meniu" << endl;
    }
};




class Judet
{
private:
    string nume;
    vector < Hotel > hoteluri;

public:
    Judet() {}

    Judet(string name)
    {
        nume = name;
    }

    string getName()
    {
        return nume;
    }

    unsigned int judetSize()
    {
        return hoteluri.size();
    }

    Hotel hotelOfJudet(unsigned int i)
    {
        return hoteluri[i];
    }

    void addHotel(Hotel a)
    {
        hoteluri.push_back(a);
    }

    void displayHotels()
    {
        for(auto &i : hoteluri)
        {
            i.displayHotel();
            cout << endl;
        }

    }


    ~Judet()
    {
        hoteluri.clear();
        hoteluri.shrink_to_fit();
    }


};

vector < Judet > judete;
vector < pair < string, rezervare > > rezervari;

///pret rezervare = (pret camera + optiuni masa) * zile - bonus_fidelitate

double calcRezervare(rezervare r, string nume)
{
    double rezultat, bonus_fidelitate = 2, cazare;

    for(auto &i : rezervari)
        if(i.first == nume && i.second.status == APROBATA)
            if(bonus_fidelitate + 4 <= 18)
                bonus_fidelitate += 4;

    for(unsigned int i = 0; i < judete.size(); i++)
        if(judete[i].getName() == r.numeJudet)
            for(unsigned int j = 0; j < judete[i].judetSize(); j++)
                if(judete[i].hotelOfJudet(j).getDenumire() == r.numeHotel)
                    for(unsigned int k = 0; k < judete[i].hotelOfJudet(j).nrCamere(); k++)
                        if(judete[i].hotelOfJudet(j).getCamera(k)->getNumar() == r.nrCamera)
                        {
                            cazare = judete[i].hotelOfJudet(j).getCamera(k)->calcCazare();
                            break;
                        }

    rezultat = (cazare + pret_masa * r.nr_mese) * r.nrZile * (1 - bonus_fidelitate* 1.0 / 100);

    return rezultat;
}

class MeniuAdmin
{
private:
    int optiune, iesire;
    Icons icons;

public:

    MeniuAdmin()
    {
        optiune = 0;
        iesire = 1;
    }

    void view(int optiune)
    {

        if (optiune == 0)
        {
            cout << " Bun venit in meniul pentru administratori" << endl;
            cout << "1. Afiseaza judete" << endl;
            cout << "2. Adauga judet" << endl;
            cout << "3. Adauga hotel" << endl;
            cout << "4. Adauga camera" << endl;
            cout << "5. Afiseaza toate hotelurile dupa judet" << endl;
            cout << "6. Salveaza fisier" << endl;
            cout << "7. Genereaza raport" << endl;
            cout << "8. Vezi rezervari" << endl;
            cout << "9. Iesire" << endl;
        }

        if (optiune == 1)
        {
            cout << "Lista de judete:" << endl;
            for (unsigned int i = 0; i < judete.size(); i++)
            {
                cout << "\tNume judet: " << judete[i].getName() << endl;
            }
            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }


        if (optiune == 2)
        {
            cout << "Introduceti nume Judet: ";

            string numeJudet;
            cin >> numeJudet;

            Judet aux(numeJudet);
            judete.push_back(aux);

            cout << "Pentru a va intoarce apasati tasta 0 " << endl;
        }


        if (optiune == 3)
        {
            string numeJudet, numeHotel;
            int clasificare, spatiu;
            bool ok = false;

            cout << "Introduceti numele Judetului din care face parte hotelul: ";
            cin >> numeJudet;
            cout << endl;
            cout << "Introduceti numele hotelului: ";
            cin >> numeHotel;
            cout << endl;
            cout << "Introduceti clasificarea hotelului (numarul de stele): ";
            cin >> clasificare;
            cout << endl;
            cout << "Introduceti numarul spatiilor de cazare din hotel: ";
            cin >> spatiu;
            cout << endl;

            Hotel aux(numeHotel, clasificare, spatiu);

            for(unsigned int i = 0; i < judete.size(); i++)
                if(judete[i].getName() == numeJudet)
                {
                    judete[i].addHotel(aux);
                    ok = true;
                    break;
                }

            if(!ok)
            {
                Judet aux2(numeJudet);
                aux2.addHotel(aux);
                judete.push_back(aux2);
            }


            cout << "Hotelul a fost adaugat cu succes!" << endl;
            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }

        if(optiune == 4)
        {
            string numeHotel;
            int capacitate, nr;
            double pret;
            Camera* aux;

            cout << "Introduceti numele hotelului in care doriti sa adaugati camera: ";
            cin >> numeHotel;
            cout << endl;
            cout << "Introduceti numarul camerei: ";
            cin >> nr;
            cout << endl;
            cout << "Introduceti capacitatea camerei: ";
            cin >> capacitate;
            cout << endl;
            cout << "Introduceti pretul pe noapte al camerei: ";
            cin >> pret;
            cout << endl;

            if(capacitate == 2)
                aux = new CameraDubla(nr, pret);

            if(capacitate == 3)
                aux = new CameraTripla(nr, pret);

            if(capacitate == 4)
                aux = new Camera4(nr, pret);

            for(unsigned int i = 0; i < judete.size(); i++)
                for(unsigned int j = 0; j < judete[i].judetSize(); j++)
                    if(judete[i].hotelOfJudet(j).getDenumire() == numeHotel)
                    {
                        judete[i].hotelOfJudet(j).addCamera(aux);
                        break;
                    }

            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;

        }

        if (optiune == 5)
        {

            for (unsigned int i = 0; i < judete.size(); i++)
            {
                cout << "\tHotelurile din judetul " << judete[i].getName() << " sunt: " << endl;
                judete[i].displayHotels();

                cout << endl;
                cout << endl;

            }
            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }

        if (optiune == 6)
        {

            ofstream out;
            out.open("ListaHoteluri.txt");

            if (!out)
            {
                cout << "Fisier imposibil de deschis";
            }

            for (unsigned int i = 0; i < judete.size(); i++)
            {
                for(unsigned int j = 0; j < judete[i].judetSize(); j++)
                    out << judete[i].hotelOfJudet(j).getDenumire() << endl;

                cout << endl;

            }

            cout << "Fisier creat cu succes" << endl;

            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }


        if (optiune == 7)
        {

            for(unsigned int i = 0; i < judete.size(); i++)
                for(unsigned int j = 0; j < judete[i].judetSize(); j++)
                    judete[i].hotelOfJudet(j).generareRaport();
            cout << endl;

            cout << "Pentru a va intoarce apasati tasta 0 " << endl;
        }

        if (optiune == 8)
        {
            int cnt = 1;

            for(auto& i : rezervari)
            {
                cout << cnt << ". ";
                cout << "Utilizatorul " << i.first << " a facut o rezervare la hotelul " << i.second.numeHotel;
                cout << " la camera " << i.second.nrCamera << endl;
                cnt++;
            }

            cout << endl << endl;

            string raspuns;
            cout << "Doriti sa aprobati vreo rezevare? ( Y / N ) " << endl;
            cin >> raspuns;

            if(raspuns == "Y")
            {
                cout << endl << "Introduceti numarul rezervarii: ";
                cin >> cnt;

                rezervari[cnt - 1].second.status = APROBATA;
                cout << endl << "Rezervarea a fost aprobata cu succes" << endl;

                cout << "Pentru a va intoarce apasati tasta 0 " << endl;
            }

            else
                cout << "Pentru a va intoarce apasati tasta 0 " << endl;

        }

    }

    void stateMachine()
    {

        while (iesire)
        {
            system("cls");
            icons.printMenuForAdmins();
            view(optiune);

            cout << "Selectati o optiune: ";
            cin >> optiune;

            switch (optiune)
            {
            case 0:
                optiune = 0;
                break;
            case 1:
                optiune = 1;
                break;
            case 2:
                optiune = 2;
                break;
            case 3:
                optiune = 3;
                break;
            case 4:
                optiune = 4;
                break;
            case 5:
                optiune = 5;
                break;
            case 6:
                optiune = 6;
                break;
            case 7:
                optiune = 7;
                break;
            case 8:
                optiune = 8;
                break;
            case 9:
                iesire = 0;
                break;

            default:
                cout << "Optiune invalida";
                break;
            }
        }
    }


};


class MeniuUser
{
private:
    int optiune, iesire;
    Icons icons;
    string utilizator;

public:
    MeniuUser()
    {
        optiune = 0;
        iesire = 1;
    }

    void setUser(string x)
    {
        utilizator = x;
    }

    void view(int optiune)
    {
        if (optiune == 0)
        {
            cout << "Bun venit in meniul pentru utilizatori!" << endl;
            cout << "1. Afiseaza judete" << endl;
            cout << "2. Afiseaza hoteluri" << endl;
            cout << "3. Afiseaza recenzii hotel" << endl;
            cout << "4. Adauga rezervare" << endl;
            cout << "5. Adauga recenzie" << endl;
            cout << "6. Vezi rezervarile si statusul lor" << endl;
            cout << "7. Iesire" << endl;
        }
        if (optiune == 1)
        {
            cout << "Lista de judete:" << endl;
            for (unsigned int i = 0; i < judete.size(); i++)
            {
                cout << "\tNume judet: " << judete[i].getName() << endl;
            }
            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;

        }

        if (optiune == 2)
        {

            for (unsigned int i = 0; i < judete.size(); i++)
            {
                cout << "\tHotelurile din judetul " << judete[i].getName() << " sunt: " << endl;
                judete[i].displayHotels();

                cout << endl;
                cout << endl;

            }
            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }


        if(optiune == 3)
        {
            string numeHotel;

            cout << "Introduceti numele hotelului: ";
            cin >> numeHotel;
            cout << endl;

            bool ok = true;

            for(unsigned int i = 0; i < judete.size(); i++)
                for(unsigned int j = 0; j < judete[i].judetSize(); j++)
                    if(judete[i].hotelOfJudet(j).getDenumire() == numeHotel)
                    {
                        judete[i].hotelOfJudet(j).displayRecenzii();
                        ok = false;
                        return;
                    }

            if(!ok)
            {
                cout << "Numele hotelului nu exista!" << endl;
            }

            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }

        if (optiune == 4)
        {
            rezervare r;

            cout << "Introduceti numele Judetului din care face parte hotelul: ";
            cin >> r.numeJudet;
            cout << endl;

            cout << "Introduceti numele hotelului in care doriti sa faceti rezervarea: ";
            cin >> r.numeHotel;
            cout << endl;

            cout << "Introduceti data la care doriti sa va cazati: (AN LUNA ZI) ";
            cin >> r.dataCazare.an >> r.dataCazare.luna >> r.dataCazare.zi;
            cout << endl;

            cout << "Introduceti numarul camerei: ";
            cin >> r.nrCamera;
            cout << endl;

            cout << "Introduceti numarul de zile: ";
            cin >> r.nrZile;
            cout << endl;

            cout << "Introduceti numarul de mese: ";
            cin >> r.nr_mese;
            cout << endl;

            r.status = IN_ASTEPTARE;

            bool ok = true;

            for(auto &i : rezervari)
                if( i.second.numeJudet == r.numeJudet && i.second.numeHotel == r.numeHotel && i.second.dataCazare.an == r.dataCazare.an
                        && i.second.dataCazare.luna == r.dataCazare.luna && i.second.dataCazare.zi == r.dataCazare.zi && i.second.nrCamera == r.nrCamera
                        && i.second.nrZile == r.nrZile && i.second.nr_mese == r.nr_mese )
                {
                    ok = false;
                    break;
                }

            if(ok)
            {
                rezervari.push_back({utilizator, r});

                cout << "Rezervarea a fost adaugata cu succes!" << endl;
                cout << "Pretul rezervarii este " << fixed << setprecision(2) << calcRezervare(r, utilizator) << " RON." << endl;
                cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
            }

            else
            {
                string aux = utilizator + " a incercat adaugarea aceleiasi rezervari!";
                writeError(REZERVARE, aux);

                cout << "Rzervarea dumneavoastra a fost deja adaugata" << endl;
                cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
            }

        }

        if(optiune == 5)
        {
            string numeHotel;
            string recenzie;

            cout << "Introduceti numele hotelului: ";
            cin >> numeHotel;
            cout << endl;

            cin >> recenzie;

            getline(cin, recenzie);

            for(unsigned int i = 0; i < judete.size(); i++)
                for(unsigned int j = 0; j < judete[i].judetSize(); j++)
                    if(judete[i].hotelOfJudet(j).getDenumire() == numeHotel)
                    {
                        judete[i].hotelOfJudet(j).adaugareRecenzie(recenzie);
                        return;
                    }

            cout << "Recenzia a fost adaugata cu succes!" << endl;

            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }

        if (optiune == 6)
        {

            for(unsigned int i = 0; i < rezervari.size(); i++)
            {
                if(utilizator == rezervari[i].first)
                {
                    cout << "Nume judet " <<  rezervari[i].first << ", nume hotel " << rezervari[i].second.numeHotel;
                    cout << " la camera " << rezervari[i].second.nrCamera << ", incepand cu data " << rezervari[i].second.dataCazare.zi;
                    cout << " " << rezervari[i].second.dataCazare.luna << " " << rezervari[i].second.dataCazare.an;
                    cout << " si are statusul: " << rezervari[i].second.status << endl;
                }

            }

            cout << endl << "Pentru a va intoarce apasati tasta 0 " << endl;
        }

    }

    void stateMachine()
    {

        while (iesire)
        {
            system("cls");
            icons.printMenu();
            view(optiune);

            cout << "Selectati o optiune: ";
            cin >> optiune;

            switch (optiune)
            {
            case 0:
                optiune = 0;
                break;
            case 1:
                optiune = 1;
                break;
            case 2:
                optiune = 2;
                break;
            case 3:
                optiune = 3;
                break;
            case 4:
                optiune = 4;
                break;
            case 5:
                optiune = 5;
                break;
            case 6:
                optiune = 6;
                break;
            case 7:
                iesire = 0;
                break;

            default:
                cout << "Optiune invalida";
                break;
            }
        }
    }

};

class User
{
private:
    string userName;
    string pass;
    MeniuUser menuUser;
    MeniuAdmin menuAdmin;

public:
    User() {}

    User(string u, string p)
    {
        userName = u;
        pass = p;
    }

    string getUserName()
    {
        return userName;
    }

    string getPass()
    {
        return pass;
    }

    void setPass(string pass)
    {
        this->pass = pass;
    }
    void setUserName(string userName)
    {
        this->userName = userName;
    }

    void displayLoginMenu()
    {
        cout << "Login" << endl;
    }

    void checkAdmin()
    {
        if(userName == "admin" && pass == "admin")
            menuAdmin.stateMachine();

    }

    bool operator == (const User& other)
    {
        return this->userName == other.userName && this->pass == other.pass;
    }

    void check(string pa, string us)
    {
        checkAdmin();

        if (us == userName && pass == pa)
        {
            menuUser.setUser(userName);
            menuUser.stateMachine();
        }

    }

};


class Window
{
private:
    int stop;
    vector < User > utilizatori;
    User administator;

public:

    Window()
    {
        stop = 1;
    }

    void showMenu()
    {
        cout << "1. Fereastra de login" << endl;
        cout << "2. Descarca utlizatori" << endl;
        cout << "3. Iesire" << endl;
    }

    void addUtilizator(User x)
    {
        utilizatori.push_back(x);
    }

    bool checkUser(User x)
    {
        for(auto& i : utilizatori)
            if(i == x)
                return true;

        return false;
    }

    bool checkAdmin(User x)
    {
        return x == administator;
    }

    void setAdministrator(User a)
    {
        administator = a; // va fi admin, admin by default
    }

    void showWindow()
    {
        User admin("admin", "admin");
        setAdministrator(admin);

        while(stop)
        {
            int optiune;
            system("cls");

            cout << "\t\tBun venit in aplicatia noastra!" << endl;

            showMenu();

            cout << "Ce actiune doriti sa faceti?" << endl;

            cin >> optiune;

            if(optiune == 1)
            {
                system("cls");
                User user;
                cout << "Aveti cont?  (Y / N)  " << endl;
                string raspuns;
                cin >> raspuns;
                system("cls");

                if(raspuns == "N")
                {
                    cout << "Adaugati un user\nUsername: ";
                    string us;
                    cin >> us;
                    cout << "Adaugati parola: ";
                    string pass;
                    cin >> pass;
                    user.setPass(pass);
                    user.setUserName(us);

                    utilizatori.push_back(user);

                    system("cls");
                    user.displayLoginMenu();
                    cout << "\nIntroduceti username: ";
                    string userName;
                    cin >> userName;
                    cout << "Introduceti parola: ";
                    string passowrd;
                    cin >> passowrd;
                    user.check(passowrd, userName);
                }

                else
                {
                    user.displayLoginMenu();
                    cout << "\nIntroduceti username: ";
                    string userName;
                    cin >> userName;
                    cout << "Introduceti parola: ";
                    string password;
                    cin >> password;

                    user.setPass(password);
                    user.setUserName(userName);

                    if(checkUser(user) || checkAdmin(user))
                        user.check(userName, password);

                    else
                    {
                        cout << "Utilizator sau parola gresita!" << endl;
                        writeError(EROARE, "Numele de utilizator sau parola eronate!");
                        system("pause");
                    }
                }
            }

            if(optiune == 2)
            {
                ofstream fout("old.txt");

                fout << "Nume admin: " << administator.getUserName() << endl << endl;
                fout << "\tLista utilizatori:" << endl;

                for(auto &i : utilizatori)
                    fout << i.getUserName() << " " << i.getPass() << endl;
            }


            switch (optiune)
            {
            case 1:
                optiune = 1;
                break;
            case 2:
                optiune = 2;
                break;
            case 3:
                stop = 0;
                break;
            default:
                cout << "Optiune invalida";
                break;
            }

        }

    }

};


int main()
{
    ///pentru demonstratia corectitudinii aplicatiei am scris urmatoarele exemple
    Hotel a1("Olimp", 3, 20);
    Hotel a2("Davios", 4, 30);
    Hotel a3("Aqua", 3, 25);
    Hotel a4("Ramada", 3, 40);
    Judet a("Gorj");
    a.addHotel(a1);
    a.addHotel(a2);
    a.addHotel(a3);
    a.addHotel(a4);

    Hotel b1("Larisa", 3, 20);
    Hotel b2("Gabriela", 4, 30);
    Hotel b3("Elite", 3, 25);
    Hotel b4("Belle View", 3, 40);
    Judet b("Hunedoara");
    b.addHotel(b1);
    b.addHotel(b2);
    b.addHotel(b3);
    b.addHotel(b4);


    Hotel c1("Continental", 4, 15);
    Hotel c2("Vendi", 4, 20);
    Hotel c3("Grand", 4, 25);
    Hotel c4("Forum", 3, 40);
    Judet c("Sibiu");
    c.addHotel(c1);
    c.addHotel(c2);
    c.addHotel(c3);
    c.addHotel(c4);

    Hotel d1("Tower", 3, 15);
    Hotel d2("Silver", 3, 20);
    Hotel d3("Elexus", 3, 25);
    Hotel d4("LaHan", 4, 10);
    Judet d("Brasov");
    d.addHotel(d1);
    d.addHotel(d2);
    d.addHotel(d3);
    d.addHotel(d4);


    judete.push_back(a);
    judete.push_back(b);
    judete.push_back(c);
    judete.push_back(d);

    User x("alex12", "alex12");

    Window W;

    W.addUtilizator(x);

    W.showWindow();

}
