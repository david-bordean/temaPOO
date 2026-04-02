#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

/*
Programul simuleaza gestionarea unui portofoliu de investitii in actiuni, 
permitand adaugarea, modificarea si analizarea acestora, precum si efectuarea de tranzactii si actualizarea preturilor pe piata

Date de intrare:
Pentru Actiune: nume, pret curent, pret de cumparare, cantitate
Pentru Tranzactie: nume actiune, tip trnzactie, cantitate, pret
Pentru Investitor: nume investitor, lista de actiuni
Pentru Piata: lista de actiuni existente

Operatii posibile:
Pe actiuni: calcul valoare totala, calcul profit, verificare profit, actualizare pret, modificare cantitate, afisare detalii, copiere si asignare obiecte
Pe tranzactii: creare tranzactii, copiere si asignare, acces la date, afisare tranzactie
Pe investitor: adaugare actiune in portofoliu, executare tranzactii, cumparare, vanzare, cautare actiune, stergere actiune, valoare totala portofoliu, profit total, afisar portofoliu, copiere si asignare investitor
Pe piata: actualizare preturi actiuni, afisare piata, copiere si asignare
*/

class Actiune {
private:
    std::string nume;
    double pretCurent;
    double pretCumparare;
    int cantitate;

    bool esteInProfit() const {
        return pretCurent > pretCumparare;
    }

    static int numarActiuniCreate;

public:

    Actiune(const std::string& n, double pc, double pInit, int c)
        : nume(n), pretCurent(pc), pretCumparare(pInit), cantitate(c) {
            numarActiuniCreate++;
        }

    Actiune(const Actiune& a) {
        nume = a.nume;
        pretCurent = a.pretCurent;
        pretCumparare = a.pretCumparare;
        cantitate = a.cantitate;
    }

    Actiune& operator=(const Actiune& a) {
        if (this != &a) {
            nume = a.nume;
            pretCurent = a.pretCurent;
            pretCumparare = a.pretCumparare;
            cantitate = a.cantitate;
        }
        return *this;
    }

    ~Actiune() {}

    inline double valoare() const {
        return pretCurent * cantitate;
    }

    inline double profit() const {
        return (pretCurent - pretCumparare) * cantitate;
    }

    inline void actualizeazaPret(double procent) {
        pretCurent *= procent;
    }

    inline void modificaCantitate(int c) {
        cantitate += c;
    }

    std::string getNume() const {
        return nume;
    }

    int getCantitate() const {
        return cantitate;
    }

    void setPretCurent(double p) { pretCurent = p; }

    static int getNrActiuniCreate() { return numarActiuniCreate; }

    friend std::ostream& operator<<(std::ostream& out, const Actiune& a) {
        out << "Actiune: " << a.nume
            << " | Pret: " << a.pretCurent
            << " | Cantitate: " << a.cantitate
            << " | Profit: " << a.profit()
            << " | In profit: " << (a.esteInProfit() ? "DA" : "NU");
        return out;
    }
};

int Actiune::numarActiuniCreate = 0;



class Tranzactie {
private:
    std::string numeActiune;
    char* tip; // cumparare / vanzare
    int cantitate;
    double pret;

public:
    Tranzactie(const std::string& n, const char* t, int c, double p)
        : numeActiune(n), cantitate(c), pret(p) {
            tip = new char[strlen(t) + 1];
            strcpy(tip, t); 
        }

    Tranzactie(const Tranzactie& t) {
        numeActiune = t.numeActiune;
        cantitate = t.cantitate;
        pret = t.pret;
        if (t.tip == nullptr) {
            tip = nullptr;
        } else {
            tip = new char[strlen(t.tip) + 1];
            strcpy(tip, t.tip);
        }
    }

    Tranzactie& operator=(const Tranzactie& t) {
        if (this != &t) {
            numeActiune = t.numeActiune;
            cantitate = t.cantitate;
            pret = t.pret;
            delete[] tip;
            if (t.tip == nullptr) {
                tip = nullptr;
            } else {
                tip = new char[strlen(t.tip) + 1];
                strcpy(tip, t.tip);
            }
        }
        return *this;
    }

    ~Tranzactie() {delete [] tip; }

    std::string getNume() const { return numeActiune; }
    std::string getTip() const { return std::string(tip); }
    int getCantitate() const { return cantitate; }
    void setCantitate(int c) { cantitate = c; }


    friend std::ostream& operator<<(std::ostream& out, const Tranzactie& t) {
        out << "Tranzactie: " << t.tip << " " << t.cantitate
            << " actiuni " << t.numeActiune
            << " la pret " << t.pret;
        return out;
    }
};



class Investitor {
private:
    std::string nume;
    std::vector<Actiune> portofoliu;

    int numarActiuni() const {
        return (int)portofoliu.size();
    }

public:
    Investitor(const std::string& n) : nume(n) {}

    Investitor(const Investitor& i) {
        nume = i.nume;
        portofoliu = i.portofoliu;
    }

    Investitor& operator=(const Investitor& i) {
        if (this != &i) {
            nume = i.nume;
            portofoliu = i.portofoliu;
        }
        return *this;
    }

    ~Investitor() {}

    void adaugaActiune(const Actiune& a) {
        portofoliu.push_back(a);
    }


    void executaTranzactie(const Tranzactie& t) {
        for (Actiune& a : portofoliu) {
            if (a.getNume() == t.getNume()) {
                if (t.getTip() == "cumparare") {
                    a.modificaCantitate(t.getCantitate());
                } 
                else if (t.getTip() == "vanzare") {
                    if (a.getCantitate() >= t.getCantitate())
                        a.modificaCantitate(-t.getCantitate());
                } 
                else {
                    std::cout << "Datele nu au fost introduse corect!\n";
                }
                return;
            }
        }
        std::cout << "Actiunea nu exista in portofoliu.\n";
    }

    void cautaActiune(const std::string& numeCautat) const {
        for (const Actiune& a : portofoliu) {
            if (a.getNume() == numeCautat) {
                std::cout << a << "\n";
                return;
            }
        }
        std::cout << "Nu exista.\n";
    }

    void stergeActiune(const std::string& numeSters) {
        for (int i = 0; i < (int)portofoliu.size(); i++) {
            if (portofoliu[i].getNume() == numeSters) {
                portofoliu.erase(portofoliu.begin() + i);
                return;
            }
        }
    }

    double valoareTotala() const {
        double suma = 0;
        for (const Actiune& a : portofoliu)
            suma += a.valoare();
        return suma;
    }

    double profitTotal() const {
        double suma = 0;
        for (const Actiune& a : portofoliu)
            suma += a.profit();
        return suma;
    }

    void setNume(const std::string& n) { nume = n; }

    friend std::ostream& operator<<(std::ostream& out, const Investitor& i) {
        out << "Investitor: " << i.nume
            << " | Actiuni in portofoliu: " << i.numarActiuni() << "\n";
        for (const Actiune& a : i.portofoliu)
            out << a << "\n";
        return out;
    }

    Investitor& operator+=(const Actiune& a) {
    portofoliu.push_back(a);
    return *this;
    }
};




class Piata {
private:
    std::vector<Actiune> actiuni;

public:
    Piata(const std::vector<Actiune>& a) : actiuni(a) {}

    Piata(const Piata& p) {
        actiuni = p.actiuni;
    }

    Piata& operator=(const Piata& p) {
        if (this != &p)
            actiuni = p.actiuni;
        return *this;
    }

    ~Piata() {}

    void actualizeazaPreturi() {
        for (Actiune& a : actiuni) {
            double procent = 0.9 + (rand() % 21) / 100.0;
            a.actualizeazaPret(procent);
        }
    }

    void setActiuni(const std::vector<Actiune>& a) { actiuni = a; }

    friend std::ostream& operator<<(std::ostream& out, const Piata& p) {
        for (const Actiune& a : p.actiuni)
            out << a << "\n";
        return out;
    }
};


int main() {


    std::cout << "=== Actiuni initiale ===\n";
    Actiune a1("Apple", 150, 120, 10);
    Actiune a2("Google", 200, 180, 5);
    Actiune a3("Microsoft", 300, 320, 8);

    std::cout << a1 << "\n";
    std::cout << a2 << "\n";
    std::cout << a3 << "\n";

    Actiune a1_copie(a1);
    Actiune a3_asignat = a3;
    std::cout << "\n--- Copie Apple: " << a1_copie << "\n";
    std::cout << "--- Asignat Microsoft: " << a3_asignat << "\n";


    std::cout << "\n=== Valoare si profit individuale ===\n";
    std::cout << "Valoare Apple: " << a1.valoare() << "\n";
    std::cout << "Profit Apple: " << a1.profit() << "\n";
    std::cout << "Profit Microsoft: " << a3.profit() << "\n";


    std::cout << "\n=== Modificari directe pe actiune ===\n";
    a2.actualizeazaPret(1.10);
    std::cout << "Google dupa crestere 10%: " << a2 << "\n";
    a2.modificaCantitate(3);
    std::cout << "Google dupa adaugare 3 bucati: " << a2 << "\n";


    std::cout << "\n=== Tranzactii ===\n";
    Tranzactie t1("Apple", "cumparare", 5, 150);
    Tranzactie t2("Google", "vanzare", 2, 200);
    Tranzactie t3("Tesla", "cumparare", 3, 500); // nu exista
    std::cout << t1 << "\n";
    std::cout << t2 << "\n";


    Tranzactie t1_copie(t1);
    Tranzactie t2_asignat = t2;
    std::cout << "Copie t1: " << t1_copie << "\n";
    std::cout << "Asignat t2: " << t2_asignat << "\n";

    std::cout << "\n=== Portofoliu initial ===\n";
    Investitor inv("Ion Popescu");
    inv += a1;
    inv += a2;
    inv += a3;
    std::cout << inv;


    std::cout << "\n=== Dupa tranzactii ===\n";
    inv.executaTranzactie(t1); 
    inv.executaTranzactie(t2); 
    inv.executaTranzactie(t3); // nu exista
    Tranzactie t_gresit("Apple", "imprumut", 3, 150); // tip invalid
    inv.executaTranzactie(t_gresit);
    std::cout << inv;

    std::cout << "=== Cautare actiuni ===\n";
    std::cout << "Caut Apple: "; inv.cautaActiune("Apple");
    std::cout << "Caut Tesla: "; inv.cautaActiune("Tesla");


    std::cout << "Valoare totala portofoliu: " << inv.valoareTotala() << "\n";
    std::cout << "Profit total portofoliu: " << inv.profitTotal() << "\n";


    Investitor inv_copie(inv);
    Investitor inv_asignat = inv;
    std::cout << "\n--- Copie investitor:\n" << inv_copie;
    std::cout << "--- Asignat investitor:\n" << inv_asignat;


    std::cout << "=== Dupa stergere Microsoft ===\n";
    inv.stergeActiune("Microsoft");
    std::cout << inv;




    std::cout << "=== Piata ===\n";
    std::vector<Actiune> listaActiuni = { a1, a2, a3 };
    Piata p(listaActiuni);
    std::cout << "Piata inainte de actualizare:\n" << p;
    p.actualizeazaPreturi();
    std::cout << "Piata dupa actualizare:\n" << p;

    Piata p_copie(p);
    Piata p_asignat = p;
    std::cout << "Copie piata:\n" << p_copie;
    std::cout << "Asignat piata:\n" << p_asignat;

    std::cout << "Total actiuni create: " << Actiune::getNrActiuniCreate() << "\n";


    std::cout << "\n=== Testare setteri ===\n";
    a1.setPretCurent(160);
    std::cout << "Apple dupa setteri: " << a1 << "\n";

    t1.setCantitate(10);
    std::cout << "Tranzactie dupa setteri: " << t1 << "\n";

    inv.setNume("Gheorghe Popescu");
    std::cout << "Investitor dupa setter nume: " << inv;

    std::vector<Actiune> listaNoua = { a1, a2 };
    p.setActiuni(listaNoua);
    std::cout << "Piata dupa setter actiuni:\n" << p;

    return 0;
}
