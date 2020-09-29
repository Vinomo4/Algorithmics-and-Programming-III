#include <algorithm>
#include <ctime>
#include <fstream>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

// Variables globals
string outputFile;
int N1, N2, N3, T, J;
double t_inici = clock();
vector<int> solucio(11);
int preusf = 0, puntssf = 0;
//

// L'estructura Jugador guarda la informació d'un jugador excepte el seu equip
// irrellevant per a l'execució de l'algorisme.
struct Jugador {
    string nom;
    int preu;
    int punts;
    string pos;
};

vector<Jugador> jugadors; // Vector que guardarà els jugadors que puguin arribar a estar en el nostre equip per preu.


// Utilitzat per ordenar els jugadors en ordre decreixent de ratio f = punts / log(preu + 2).
bool comparacio(const Jugador& a, const Jugador& b) {
    return a.punts / log(a.preu + 2) > b.punts / log(b.preu + 2); // Sumem dues unitats al preu per assegurar-nos que la funció
}                                                                 // f tingui domini en tot el pla real (punts - preu) positiu.



// Funció auxiliar per evitar repetició de codi. Guarda el jugador en el vector
// solució i augmenta les variables de control corresponents.
void augmenta(int i, int& n_jug, int& n_pos) {
    solucio[n_jug] = i;
    preusf += jugadors[i].preu;
    puntssf += jugadors[i].punts;
    ++n_pos;
    ++n_jug;
}


// Funció que escriu a l'arxiu l'alineació, punts i preu trobats per l'algorisme greedy.
void escriure_equip() {
    ofstream solutions(outputFile);
    double t_final = clock();
    string nom_porter;
    vector<string> nom_defenses, nom_mitjos, nom_davanters;

    for(int x : solucio) {
        Jugador& jugador = jugadors[x];
        if(jugador.pos == "por") nom_porter = jugador.nom;
        else if(jugador.pos == "def") nom_defenses.push_back(jugador.nom);
        else if(jugador.pos == "mig") nom_mitjos.push_back(jugador.nom);
        else nom_davanters.push_back(jugador.nom);
    }

    solutions.setf(ios::fixed);
    solutions.precision(1); //Utilitzem un decimal per expressar el temps en trobar la resposta.

    solutions << double(t_final - t_inici) / CLOCKS_PER_SEC << endl;
    solutions << "POR: " << nom_porter << endl << "DEF: ";
    for(int i = 0; i < N1; ++i) solutions << (i == 0 ? "" : ";")  << nom_defenses[i];
    solutions << endl << "MIG: ";
    for(int i = 0; i < N2; ++i) solutions << (i == 0 ? "" : ";")  << nom_mitjos[i];
    solutions << endl << "DAV: ";
    for(int i = 0; i < N3; ++i) solutions << (i == 0 ? "" : ";")  << nom_davanters[i];
    solutions << endl << "Punts: " <<  puntssf << endl << "Preu: " << preusf << endl;
    solutions.close();
}


// Funció que crea l'equip mitjançant un algorisme greedy.
void crea_equip() {
    int n_jug = 0, n_por = 0, n_def = 0, n_mig = 0, n_dav = 0;
    int i = 0; // Indica la posició en el vector jugadors.
    while(n_jug < 11) {
        Jugador& jugador = jugadors[i];
        // Discernim la posició del jugador.
        if(jugador.preu + preusf <= T) {
            if(jugador.pos == "por" and n_por < 1)
                augmenta(i, n_jug, n_por);
            else if(jugador.pos == "def" and n_def < N1)
                augmenta(i, n_jug, n_def);
            else if(jugador.pos == "mig" and n_mig < N2)
                augmenta(i, n_jug, n_mig);
            else if (jugador.pos == "dav" and n_dav < N3)
                augmenta(i, n_jug, n_dav);
        }
        ++i;
    }
    escriure_equip();
}


// Llegeix les condicions que ha de complir l'equip de l'arxiu proporcionat.
void llegir_condicions(string conditionsFile) {
    ifstream conditions(conditionsFile);
    conditions >> N1 >> N2 >> N3 >> T >> J;
    conditions.close();
}


// Llegeix la base de dades de jugadors donada per l'usuari.
void llegir_base_dades(string data_baseFile) {
    ifstream in(data_baseFile);
    while (not in.eof()) {
        string nom, posicio, club;
        int punts, preu;
        getline(in, nom, ';');    if (nom == "") break;
        getline(in, posicio, ';');
        in >> preu;
        char aux;    in >> aux;
        getline(in, club, ';');
        in >> punts;
        string aux2;
        getline(in,aux2);
        // Selecció dels jugadors que compleixen els requisits de preu.
        if(preu <= J and preu <= T) {
            Jugador jug({nom, preu, punts, posicio});
            jugadors.push_back(jug);
        }
    }
    in.close();
}


int main(int argc, char** argv) {
    //Comrpovació d'errors en els paràmetres d'entrada.
    if (argc != 4) {
        cout << "El nombre d'arguments introduïts és incorrecte, si us plau, revisi l'entrada." << endl
             << "Si té dubtes sobre el format d'entrada, introdueixi 1, altrament, qualsevol altre número: ";
        int error;
        cin >> error;
        if(error == 1) cout << endl << "És necessari introduir tres arguments:" << endl <<  "    1. Arxiu que conté la base de dades de jugadors." << endl
                            << "    2. Nom de l'arxiu de consulta." << endl << "    3. Nom del fitxer de sortida." << endl << endl;
        exit(1);
    }

    // Llegim els paràmetres d'entrada
    outputFile = argv[3];
    llegir_condicions(argv[2]);
    llegir_base_dades(argv[1]);

    sort(jugadors.begin(), jugadors.end(), comparacio); // Ordenem els jugadors segons el criteri greedy seleccionat.
    crea_equip();
}
