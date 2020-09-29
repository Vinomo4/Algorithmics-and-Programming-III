#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// Variables globals
string outputFile;
int max_punts = -1, min_preu = -1;;
int N1, N2, N3, T, J;
double t_inici = clock();
//

// L'estructura Jugador guarda la informació d'un jugador excepte el seu equip
// irrellevant per a l'execució de l'algorisme.
struct Jugador {
    string nom;
    int preu;
    int punts;
    string pos;
};

vector<Jugador> jugadors; // Vector que guardarà els jugadors que puguin estar en el nostre equip per preu.

// Utilitzat per ordenar els jugadors dels vectors per ordre decreixent de punts.
bool comparacio(const Jugador& a, const Jugador& b) {
    return a.punts > b.punts;
}

// Funció que escriu a l'arxiu resposta la solució més òptima trobada fins al moment.
void escriure_equip(const vector<int>& solucio, int preusf) {
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
    solutions << endl << "Punts: " <<  max_punts << endl << "Preu: " << preusf << endl;
    solutions.close();
}


void crea_equip(vector<int>& solucio, int i, int n_jug, int n_por, int n_def, int n_mig, int n_dav, int preusf, int puntssf) {
    // Si disposem d'un equip finalitzat, comprovem si la suma dels seus punts és millor que la trobada fins al moment.
    if(n_jug == 11) {
        if(puntssf > max_punts) {
            max_punts = puntssf;
            escriure_equip(solucio, preusf);
        }
        else if(puntssf == max_punts and (preusf < min_preu or min_preu == -1)) { // A igualtat de punts agafem
            min_preu = preusf;                                                    // l'equip més econòmic.
            escriure_equip(solucio, preusf);
        }
    }
    else {
        for(uint k = i; k < jugadors.size(); ++k) {
            // Guardem a la variable la suma de la puntuació dels n seguents jugadors
            // essent n_jug el nombre de jugadors que encara hem de posar al nostre equip.
            int max_assolible = 0;
            for(uint j = k; j < k + 11 - n_jug; ++j) max_assolible += jugadors[j].punts;

            Jugador& jugador = jugadors[k];

            if(jugador.preu + preusf <= T) {
                // Si es compleix que podem comprar el jugador i és possible trobar una solució millor a la trobada fins el moment seguim permutant.
                if (puntssf + max_assolible >= max_punts) {
                    // Discernim la posició del jugador i cridem recursivament a la funció crea_equip per seguir omplint el vector solucio tot
                    // en una unitat el comptador de la posició corresponent.
                    if(jugador.pos == "por") {
                        if(n_por < 1) {
                            solucio[n_jug] = k;
                            crea_equip(solucio, k + 1, n_jug + 1, n_por + 1, n_def, n_mig, n_dav, preusf + jugador.preu, puntssf + jugador.punts);
                        }
                    }
                    else if(jugador.pos == "def") {
                        if(n_def < N1){
                            solucio[n_jug] = k;
                            crea_equip(solucio, k + 1, n_jug + 1, n_por, n_def + 1, n_mig, n_dav, preusf + jugador.preu, puntssf + jugador.punts);
                        }
                    }
                    else if(jugador.pos == "mig") {
                        if(n_mig < N2) {
                            solucio[n_jug] = k;
                            crea_equip(solucio, k + 1, n_jug + 1, n_por, n_def, n_mig + 1, n_dav, preusf + jugador.preu, puntssf + jugador.punts);
                        }
                    }
                    else if(n_dav < N3) {
                        solucio[n_jug] = k;
                        crea_equip(solucio, k + 1, n_jug + 1, n_por, n_def, n_mig, n_dav + 1, preusf + jugador.preu, puntssf + jugador.punts);
                    }
                }
                else return; // Si agafant el millor jugador del vector jugadors[k, ...] no trobem cap solució
                             // no cal que seguim provant més jugadors ja que, qualsevol altre jugador que
                             // puguem agafar en una iteració futura disposarà de menys puntuació que l'actual.
            }
        }
    }
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
             << "Si té dubtes sobre el format d'entrada, introdueixi 1, altrament, introdueixi qualsevol altre número: ";
        int error;
        cin >> error;
        if(error == 1) cout << "És necessari introduir tres arguments:" << endl <<  "1. Arxiu que conté la base de dades de jugadors." << endl
                            << "2. Nom de l'arxiu de consulta." << endl << "3. Nom del fitxer de sortida." << endl;
        exit(1);
    }

    outputFile = argv[3];
    llegir_condicions(argv[2]);
    llegir_base_dades(argv[1]);
    sort(jugadors.begin(), jugadors.end(), comparacio); // Ordenem els jugadors llegits per puntuació decreixent.

    vector<int> solucio(11);
    crea_equip(solucio, 0, 0, 0, 0, 0, 0, 0, 0);
}
