//
// Created by Luca on 3/22/2026.
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib> //pt rand() si srand()
#include <ctime> //pt time()
#include <exception>
#include <list>
#include <typeinfo>

using namespace std;

enum Pozitie { NESPECIFICAT = 0, PORTAR = 1, MIJLOCAS = 3, ATACANT = 4 };
class ExceptieVarstaInvalida : public exception {
public:

    const char * what () const throw () override {
        return "Varsta introdusa invalida! Fotbalistul trebuie sa aiba minimum 16 ani si maximum 40!";
    }
};

class ExceptieNumarTricouInvalid : public exception {
public:
    const char * what () const throw () override {
        return "Numarul de pe tricou trebuie sa fie intre 1 si 99!";
    }
};

//clasa de baza abstracta
class Jucator {
private:
    static int totalJucatoriCreati; //atribut static
protected: //nu private pt ca derivatele sa poata accesa atributele
    string nume;
    int numar_tricou, varsta, numar_goluri;
public:
    //constructor de inițializare
    Jucator(string nume = "Gigel", int numar_tricou = 0, int varsta = 0, int numar_goluri = 0);

    //constructor de copiere
    Jucator(const Jucator& j);

    //operator= de copiere
    Jucator& operator=(const Jucator& j);

    //destructor virtual in clasa de baza pt polimorfism
    virtual ~Jucator();

    virtual void citire(istream&);
    virtual void afisare(ostream&) const;

    //operator>> pentru citire (std::istream)
    friend istream& operator>>(istream& in, Jucator&);
    //operator<< pentru afișare (std::ostream)
    friend ostream& operator<<(ostream& out, const Jucator&);

    int operator+(const Jucator& j) const; //alt operator supraîncărcat ca funcție membră

    int getGoluri() const { return numar_goluri; }
    string getNume() const { return nume; }

    //functie statica complexa
    static void afiseazaMesajClub() {
        cout<<"!!!OFICIAL!!! \n In prezent avem "<<totalJucatoriCreati<<" jucatori inregistrati in baza noastra de date. \n";
    }

    //functie virtuala pura care face clasa abstracta
    virtual void antrenamentSpecific() const = 0;
};

int Jucator::totalJucatoriCreati = 0;

Jucator::Jucator(string n, int t, int v, int g) : nume(n),numar_tricou(t), varsta(v), numar_goluri(g) {
    totalJucatoriCreati++;
}

Jucator::Jucator(const Jucator& j) : nume(j.nume), numar_tricou(j.numar_tricou), varsta(j.varsta), numar_goluri(j.numar_goluri) {
    totalJucatoriCreati++;
}

Jucator& Jucator::operator=(const Jucator& j) {
    if (this != &j) { //verificam daca un obiect este atribuit lui insusi
        nume = j.nume;
        numar_tricou = j.numar_tricou;
        varsta = j.varsta;
        numar_goluri = j.numar_goluri;
    }
    return *this;
}

Jucator::~Jucator() {
    totalJucatoriCreati--;
}

void Jucator::citire(istream& in) {
    cout<<"Nume: "; in>>nume;
    cout<<"Numar tricou: ";   in>>numar_tricou;
    if (numar_tricou < 1 || numar_tricou > 99) throw ExceptieNumarTricouInvalid();

    cout<<"Varsta: "; in>>varsta;
    if (varsta <= 15 || varsta >= 45) throw ExceptieVarstaInvalida();

    cout<<"Numar goluri: "; in>>numar_goluri;
}

void Jucator::afisare(ostream& out) const {
    out<<"Jucator: "<<nume<<" | Numar tricou: "<<numar_tricou<<" | Varsta: "<<varsta<<" | Numar goluri: "<<numar_goluri<<endl;
}

istream& operator>>(istream& in,Jucator& j) {
    j.citire(in);
    return in;
}
ostream& operator<<(ostream& out,const Jucator& j) {
    j.afisare(out);
    return out;
}
//operator membru
int Jucator::operator+(const Jucator& j) const {
    return this-> numar_goluri + j.numar_goluri;
}
//operator non-membru
bool operator<(const Jucator& j1, const Jucator& j2) {
    return j1.getGoluri() < j2.getGoluri();
}

//Derivata din clasa Jucator
class Portar : public Jucator {
private:
    int meciuriFaraGolPrimit;
public:
    Portar(string nume="Gigel", int t=1, int v=20, int g=0, int mfgp=0) : Jucator(nume, t, v, g), meciuriFaraGolPrimit(mfgp) {}

    //constructor copiere redefinit
    Portar(const Portar& p): Jucator(p), meciuriFaraGolPrimit(p.meciuriFaraGolPrimit) {}

    //operator= redefinit pt atribuiri corecte
    Portar& operator=(const Portar& p) {
        if (this != &p) {
            Jucator::operator=(p);
            meciuriFaraGolPrimit = p.meciuriFaraGolPrimit;
        }
        return *this;
    }

    void citire(istream& in) override {
        Jucator::citire(in);
        cout<<"Meciuri fara gol primit: "; in >> meciuriFaraGolPrimit;
    }
    void afisare(ostream& out) const override {
        Jucator::afisare(out);
        out<<" | Clean Sheet uri: "<<meciuriFaraGolPrimit<<endl;
    }

    void antrenamentSpecific() const override {
        cout<<"Portarul "<<nume<<" exerseaza apararea suturilor de la distanta \n";
    }
};

//Derivata din clasa Jucator
class JucatordeCamp : public Jucator {
protected:
    int stamina;
public:
    JucatordeCamp(string nume="Vasile", int t=8, int v=23, int g=0, int rez=80) : Jucator(nume, t, v, g), stamina(rez) {}
    JucatordeCamp(const JucatordeCamp& j) : Jucator(j), stamina(j.stamina) {}

    JucatordeCamp& operator=(const JucatordeCamp& j) {
        if (this != &j) {
            Jucator::operator=(j);
            stamina = j.stamina;
        }
        return *this;
    }

    void citire(istream& in) override {
        Jucator::citire(in);
        cout<<"Stamina (alege o valoare din 1-100): "; in >> stamina;
    }
    void afisare(ostream& out) const override {
        Jucator::afisare(out);
        out<<"Stamina: "<<stamina<<"% | ";
    }

    void antrenamentSpecific() const override = 0; //clasa abstracta, nu poate fi instantiata
};

//Derivate din clasa JucatordeCamp (nivelul al 3 lea al ierarhiei)
class Mijlocas : public JucatordeCamp {
private:
    int numarPase;
public:
    Mijlocas(string nume="Gheorghe", int t=17, int v=17, int g=0, int rez=90, int pase=0) : JucatordeCamp(nume, t, v, g, rez), numarPase(pase) {}

    Mijlocas(const Mijlocas& m) : JucatordeCamp(m), numarPase(m.numarPase) {}

    Mijlocas& operator=(const Mijlocas& m) {
        if (this != &m) {
            JucatordeCamp::operator=(m);
            numarPase = m.numarPase;
        }
        return *this;
    }

    void citire(istream& in) override {
        JucatordeCamp::citire(in);
        cout<<"Numar pase: "; in >> numarPase;
    }
    void afisare(ostream& out) const override {
        JucatordeCamp::afisare(out);
        out<<"Pase: "<<numarPase<<endl;
    }
    
    void antrenamentSpecific() const override {
        cout<<"Mijlocasul "<<nume<<" exerseaza precizia paselor \n";
    }
};

class Atacant : public JucatordeCamp {
private:
    int finalizare;
public:
    Atacant(string nume="Alex", int t=9, int v=19, int g=0, int rez=85, int f=75) : JucatordeCamp(nume, t, v, g, rez), finalizare(f) {}

    Atacant(const Atacant& a) : JucatordeCamp(a), finalizare(a.finalizare) {}

    Atacant& operator=(const Atacant& a) {
        if(this != &a) {
            JucatordeCamp::operator=(a);
            finalizare = a.finalizare;
        }
        return *this;
    }

    void citire(istream& in) override {
        JucatordeCamp::citire(in);
        cout<<"Procentaj finalizare: "; in >> finalizare;
    }
    void afisare(ostream& out) const override {
        JucatordeCamp::afisare(out);
        out<<"Finalizare: "<<finalizare<<"% \n";
    }
    void antrenamentSpecific() const override {
        cout << "Atacantul "<<nume<<" se antreneaza la finalizare \n";
    }

    //downcast
    void executaPenalty() const {
        cout<<nume<<" a executat un penalty \n";
    }
};

//relatie agregare
class Stadion {
private:
    string numeStadion;
public:
    Stadion(string nume = "Stadionul Ghencea") : numeStadion(nume) {}
    string getNumeStadion() const {return numeStadion;}
};


//compunere + agregare
class Echipa {
private:
    string nume;
    list<Jucator*> jucatori; //list (structura STL)
    Stadion* stadionAsociat; //agregare

    class AutocarEchipa {
    public:
        string firma;
        AutocarEchipa(string f = "Mercedes") : firma(f) {}
    };
    AutocarEchipa autocar; //compunere

public:
    Echipa(string nume = "Abcd", string firmaAutocar = "Mercedes") : nume(nume), stadionAsociat(nullptr), autocar(firmaAutocar) {}

    void atribuieStadion(Stadion* st) {stadionAsociat = st; }

    void adaugaJucator(Jucator* j) { jucatori.push_back(j); }

    void inlocuiesteAutocar(string firmaNoua) {
        autocar.firma = firmaNoua;
    }

    int goluri_totale() const;
    void afisare_Jucatori() const;

    string getNume() const {return nume; }

    // Calculează o valoare de atac bazată pe experiența (golurile) jucătorilor
    int getValoareAtac() const {
        int total = 0;
        for (const auto& j : jucatori) {
            total += j->getGoluri();
        }
        return total;
    }
};
int Echipa::goluri_totale() const {
    float s = 0;
    for (const auto& j : jucatori)
        s += j->getGoluri();
    return s;
}
void Echipa::afisare_Jucatori() const {
    cout<<"Echipa: "<<nume<<endl;
    cout<<"Autocarul echipei apartine firmei: "<<autocar.firma<<endl;
    if (stadionAsociat) cout<<"Isi joaca meciurile de acasa pe "<<stadionAsociat->getNumeStadion()<<endl;
    if (jucatori.empty()) cout<<"Echipa nu are jucatori in momentul de fata. \n";
    for (const auto& j : jucatori)
        cout<<*j<<endl;
}

class Meci {
private:
    Echipa* gazde;
    Echipa* oaspeti;
    int goluri_gazde, goluri_oaspeti;
public:
    Meci(Echipa* g = nullptr, Echipa* o = nullptr) : gazde(g), oaspeti(o), goluri_gazde(0), goluri_oaspeti(0) {}

    void SimulareMeci(); //functia complexa
    void castigator();
};

void Meci::SimulareMeci() {
    if (!gazde || !oaspeti) {
        cout<<"Nu exista echipe, prin urmare meciul nu se poate desfasura";
        return;
    }

    //resetez scorul la 0-0 pt a instantia meciul
    goluri_gazde = 0;
    goluri_oaspeti = 0;

    cout<<"START "<< gazde->getNume()<<" vs "<< oaspeti->getNume()<< endl;

    //preiau calitatea atacurilor fiecarei echipe
    int atacGazde = gazde->getValoareAtac();
    int atacOaspeti = oaspeti->getValoareAtac();

    for (int ocazie = 1; ocazie <=5; ocazie++) {
        cout<<"Ocazia "<<ocazie<<endl;

        //Ocazie Gazde
        int ocazieGazde = (rand() % 100) + (atacGazde / 5);
        cout <<ocazieGazde<< " Atac " << gazde->getNume() << "... ";
        if (ocazieGazde > 75) {
            cout << "GOOOL " << gazde->getNume() << "! \n";
            goluri_gazde++;
    } else {
        cout<<"Ratare \n";
    }

        //Ocazie Oaspeti
        int ocazieOaspeti = (rand() % 100) + (atacOaspeti * 3);
        cout <<ocazieOaspeti<< " Atac " << oaspeti->getNume() << "... ";
        if (ocazieOaspeti > 75) {
            cout << "GOOOL " << oaspeti->getNume() << "! \n\n";
            goluri_oaspeti++;
        } else {
            cout<<"Ratare \n\n";
        }
        cout<<"Scor dupa "<<ocazie<<" ocazie/ocazii: "<<goluri_gazde<<" - "<<goluri_oaspeti<<endl;
    }
    cout<<"Scor final: "<<goluri_gazde<<" - "<<goluri_oaspeti<<endl;
}

void Meci::castigator() {
    if (!gazde || !oaspeti) {
        cout<<"Nu exista meciul \n";
        return;
    }
    if (goluri_gazde > goluri_oaspeti)
            cout<<"Castiga gazdele, "<<gazde->getNume()<<endl;
    else if (goluri_gazde < goluri_oaspeti)
            cout<<"Castiga oaspetii, "<<oaspeti->getNume()<<endl;
    else
        cout<<"Meciul a fost egal \n";
}

//Singleton
class Campionat {
private:
    string numeCampionat;
    int numarEchipe;
    static Campionat* liga;

    //constructor private
    Campionat() : numeCampionat("Premier League"), numarEchipe(20) {}
public:
    Campionat(const Campionat&) = delete;
    Campionat& operator=(const Campionat&) = delete;

    static Campionat* getLiga() {
        if (liga == nullptr) {
            liga = new Campionat();
        }
        return liga;
    }

    void afiseazaInformatiiLiga() const {
        cout<<"\n Campionatul "<<numeCampionat<<" are "<<numarEchipe<<" echipe \n";
    }
    static void LigaZero() {
        if (liga != nullptr) {
            delete liga;
            liga = nullptr;
        }
    }
};
Campionat* Campionat::liga = nullptr;

//Template
template <typename T>
class Trofee {
private:
    string detinator;
    vector<T> trofee; //structura STL
public:
    Trofee(string detinator) : detinator(detinator) {}
    void adaugaTrofeu(T t) {trofee.push_back(t);}

    void afiseazaTrofee() const {
        cout<<"Vitrina de trofee al echipei "<<detinator<<" contine "<<trofee.size()<<" trofee."<<endl;
    }
};

void uiMeniu() {
    cout<<"+++++++++++++++++++++++++++++++"<<endl;
    cout<<" Introdu 1 pentru a adauga n jucatori in lista fotbalistilor"<<endl;
    cout<<" Introdu 2 pentru a afisa lista jucatorilor "<<endl;
    cout<<" Introdu 3 pentru a trimite toti jucatorii la antrenamentul lor specific "<<endl;
    cout<<" Introdu 4 pentru a aduna golurile a 2 jucatori"<<endl;

    cout<<" Introdu 5 pentru a legitima un jucator din lista intr-una din echipe "<<endl;
    cout<<" Introdu 6 pentru a afisa echipele si stadionul pe care joaca"<<endl;
    cout<<" Introdu 7 pentru a afisa totalul golurilor marcate per echipa"<<endl;

    cout<<" Introdu 8 pentru a programa un meci"<<endl;
    cout<<" Introdu 9 pentru a simula meciul si a anunta castigatorul "<<endl;

    cout << " Introdu 10 pentru a executa penalty" << endl;
    cout << " Introdu 11 pentru a afisa comunicatul oficial al clubului" << endl;
    cout << " Introdu 12 pentru a afisa detaliile campionatului"<<endl;
    cout << " Introdu 13 pentru a vedea trofeele clubului"<<endl;

    cout<< "Introdu 14 pentru a sorta jucatorii descrescator dupa numarul de goluri marcate"<<endl;

    cout<<" Introdu 0 pentru a iesi din program si a retrage jucatorii"<<endl;
    cout<<"-----------------------------\n"<<endl;
}

void meniu () {
    vector<Jucator*> listaJucatori; //vector

    listaJucatori.push_back(new Portar("Caracatita", 1, 19, 1, 250));
    listaJucatori.push_back(new Mijlocas("Vultur", 8, 19, 10, 250));
    listaJucatori.push_back(new Atacant("Lup", 9, 19, 35, 70, 80));
    listaJucatori.push_back(new Atacant("Leopard", 11, 19, 20, 150, 65));


    Echipa e1("Echipa Gazda"), e2("Echipa Oaspete");

    //instantiere stadion
    Stadion arena("Parc des Princes");
    e1.atribuieStadion(&arena);

    Meci m;
    bool stop = false;
    int opt;

    while(stop == false) {
        uiMeniu();
        cout<<"Introd opt: ";
        cin>>opt;

        switch (opt) {
            case 1: {
                int n;
                cout<<"Cati jucatori vor fi adaugati? ";
                cin>>n;
                for (int i = 0; i < n; i++) {
                    cout << "\nAlege tipul jucatorului " << i + 1 << ":\n";
                    cout<<"1 pt Portar, 3 pt Mijlocas, 4 pt Atacant \n Optiune tip: ";
                    int alegereTip;
                    cin>>alegereTip;

                    Jucator* j = nullptr;

                    if (alegereTip == 1) j=new Portar();
                    else if (alegereTip == 3) j=new Mijlocas();
                    else if (alegereTip == 4) j=new Atacant();
                    else {
                        cout<<"Tip invalid! Se sare peste jucator. \n";
                        continue;
                    }

                    try {
                        cin>>*j; //upcast implicit: jucatorul derivat citit ca referinta de baza (polimorfism la citire)
                        listaJucatori.push_back(j);
                    }
                    catch (const exception& e) {
                        cout<<"\n Exceptie detectata! "<<e.what()<<endl;
                        cout<<"Datele au fost introduse gresit. Obiectul a fost sters. \n\n";
                        delete j; //se curata memoria daca a picat validarea, prevenind memory leak
                    }
                }
                break;
            }
            case 2: {
                if(listaJucatori.empty()) cout<<"Lista e goala! Nu exista jucatori!\n";
                for (size_t i = 0; i < listaJucatori.size(); i++) {
                    cout<< "[" << i << "] " << *listaJucatori[i] << endl;
                }
                break;
            }
            case 3: {
                if (listaJucatori.empty()) cout<<"Nu sunt jucatori pentru a se putea desfasura antrenamentul";
                for (const auto& j : listaJucatori)
                    j->antrenamentSpecific();
                break;
            }
            case 4: {
                if (listaJucatori.size() < 2) {
                    cout<<"Nu sunt destui jucatori";
                    break;
                }

                int i, j;
                cout<<"Indexul primului jucator: ";
                cin>>i;
                cout<<"Indexul celui de-al doilea jucator: ";
                cin>>j;

                if (i >= 0 && i < listaJucatori.size() && j >= 0 && j < listaJucatori.size()) {
                    int goluri_combinate = (*listaJucatori[i]) + (*listaJucatori[j]);
                    cout<<"Numarul golurilor marcate impreuna de "<<listaJucatori[i]->getNume()<<" si "<<listaJucatori[j]->getNume()<<" este "<<goluri_combinate<<endl;
                } else cout<<"Index invalid! \n";
                break;
            }


            case 5: {
                if (listaJucatori.empty()) {
                    cout<<"Nu exista jucatori";
                    break;
                }

                size_t i;
                int team;
                cout<<"Index jucator din lista totala: ";
                cin>>i;
                cout<<"Alege 1 daca jucatorul joaca la echipa gazda/ 2 daca joaca la echipa oaspete:  ";
                cin>>team;

                if (i >= 0 && i < (int)listaJucatori.size()) {
                    if (team == 1)
                        e1.adaugaJucator(listaJucatori[i]);
                    else
                        e2.adaugaJucator(listaJucatori[i]);
                } else cout<<"Index invalid! \n";
                break;
            }
            case 6: {
                e1.afisare_Jucatori();
                e2.afisare_Jucatori();
                break;
            }
            case 7: {
                cout << "Goluri pentru "<< e1.getNume()<<": "<< e1.goluri_totale() << endl;
                cout << "Goluri pentru: "<< e2.getNume()<<": "<<e2.goluri_totale() << endl;
                break;
            }


            case 8: {
                cout<<"Meciul a fost generat cu succes! Apasa tasta 9 pentru a-l simula!";
                m = Meci(&e1, &e2);
                break;
            }
            case 9: {
                m.SimulareMeci();
                m.castigator();
                break;
            }

            case 10: { //downcast&typeid
                cout<<"Incercam rularea unei functii specifica doar atacantilor \n";
                bool aSutatAtacantul = false;
                
                for (auto j : listaJucatori) {
                    cout << "Jucatorul cu numele " << j->getNume() << " este la rulare de tipul exact: " << typeid(*j).name() << "\n";

                    //se incearca conversia de la clasa de baza Jucator* la derivata Atacant*
                    Atacant* ptrAtacant = dynamic_cast<Atacant*>(j);
                    if (ptrAtacant != nullptr) { //pointer nu e null => jucatorul e atacant
                        ptrAtacant->executaPenalty(); //apel functie membra specifica exclusiv derivatei
                        aSutatAtacantul = true;
                    }
                }
                if (!aSutatAtacantul) cout<<"Nu am gasit niciun obiect de tip Atacant in lista jucatorilor. \n";
                break;
            }
            case 11: { //apel atribut si metoda statica
                Jucator::afiseazaMesajClub();
                break;
            }
            case 12: { //singleton
                Campionat* liga = Campionat::getLiga();
                liga->afiseazaInformatiiLiga();
                break;
            }
            case 13: {//template
                Trofee<string> cabinetEchipa("Abcdef");
                cabinetEchipa.adaugaTrofeu("Supercupa Internationala");
                cabinetEchipa.adaugaTrofeu("Europa League");
                cabinetEchipa.afiseazaTrofee();
                break;
            }
            case 14: {
                if (listaJucatori.size() < 2) {
                    cout<< "Nu sunt suficienti jucatori pentru a-i sorta.";
                    break;
                }

                cout<< "Sortare descrescatoare jucatori dupa goluri: \n";

                sort(listaJucatori.begin(), listaJucatori.end(), [](Jucator* j1, Jucator* j2) {
                    return *j2 < *j1;
                });

                for (size_t i = 0; i < listaJucatori.size(); i++) {
                    cout<< i << *listaJucatori[i];
                }
                break;
            }

            case 0: {
                stop = true;
                break;
            }
            default:
                cout << "Optiune nevalida! Incearca din nou.\n";
        }

        system("pause");
        system("cls");

    }

    //curatare memorie
    for (auto j : listaJucatori) {
        delete j;
    }
    listaJucatori.clear();
    Campionat::LigaZero(); //eliberare memorie a instantei Singleton
    cout<<"Memorie eliberata cu succes, nu mai exista jucatori sau campionate\n";
}




int main() {
    srand(static_cast<unsigned int>(time(0)));
    meniu();
    return 0;
}