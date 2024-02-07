#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Estructura para representar cada proceso
struct Proceso {
    string nombre;
    int llegada;
    int tamanyo;
    int tiempo;
};

// Estructura para representar una partición de memoria
struct Particion {
    int posicion;
    int tamaño;
    Proceso proceso;
};
struct Vacio{
    string nombre;
    int inicio;
    int fin;
};

// Función para leer la lista de procesos de un archivo
vector<Proceso> leerProcesos(string nombreArchivo) {
    vector<Proceso> procesos;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            Proceso proceso;

            int posComa1 = linea.find(',');
            int posComa2 = linea.find(',', posComa1+1);
            int posComa3 = linea.find(',', posComa2+1);
            string nombre = linea.substr(0, posComa1);
            int tiempoLlegada = stoi(linea.substr(posComa1+1, posComa2-posComa1-1));
            int memoria = stoi(linea.substr(posComa2+1, posComa3-posComa2-1));
            int tiempoEjecucion = stoi(linea.substr(posComa3+1));
            
            proceso.nombre = nombre;
            proceso.llegada = tiempoLlegada;
            proceso.tamanyo = memoria;
            proceso.tiempo = tiempoEjecucion;
            procesos.push_back(proceso);
        }
        archivo.close();
    }else{
        cout<<"No se ha podido abrir el archivo 'procesos.txt'"<<endl;
    }
    return procesos;
}

// Función para comparar dos procesos por tiempo de llegada
bool compararProcesosPorTiempoLlegada(Proceso p1, Proceso p2) {
    return p1.llegada < p2.llegada;
}

int asignarPorMejorHueco(vector<Vacio> &vacio, Proceso &proceso) {
    // Buscamos la partición de memoria más grande que esté libre
    int posicionParticion = -1;
    int espacioLibreMejorParticion = 2000;
    int espacioLibre;
    for (int i = 0; i < vacio[i].fin; i++) {
        espacioLibre = vacio[i].fin - vacio[i].inicio;
        if (espacioLibreMejorParticion > espacioLibre && espacioLibre >= proceso.tamanyo) {
            espacioLibreMejorParticion = espacioLibre;
            posicionParticion = i;        
        }
    }if(posicionParticion == -1){
        cout << "No se asigna ninguna partición" <<endl;
    }
    return vacio[posicionParticion].inicio;

}
// Función para asignar un proceso a una partición de memoria usando el algoritmo de peor hueco
int asignarPorPeorHueco(vector<Vacio> &vacio, Proceso &proceso) {
    // Buscamos la partición de memoria más grande que esté libre
    int indicePeorParticion = -1;
    int espacioLibrePeorParticion = 0;
    int espacioLibre;
    for (int i = 0; i < vacio[i].fin; i++) {
        espacioLibre = vacio[i].fin - vacio[i].inicio;
        if (espacioLibrePeorParticion < espacioLibre && espacioLibre > proceso.tamanyo) {
            espacioLibrePeorParticion = espacioLibre;
            indicePeorParticion = i;        
        }
        
    }if(indicePeorParticion == -1){
        cout << "No se asigna ninguna partición" <<endl;
    }
    return vacio[indicePeorParticion].inicio;

}
vector<Vacio> vacioFragmento(vector <Particion> listaParticion){
    vector<Vacio> listaVacio;
    Vacio inicioVacio;

    inicioVacio.inicio = 0;
    inicioVacio.fin = 2000;
    inicioVacio.nombre = "Vacío";

    listaVacio.push_back(inicioVacio);

    if(listaParticion.size()!= 0){
        for(long unsigned int i = 0; i<listaParticion.size();i++){
            int fin = listaParticion[i].posicion + listaParticion[i].tamaño;
            for(long unsigned int j = 0; j < listaVacio.size(); j++){
                if(listaParticion[i].posicion == listaVacio[j].inicio){
                    listaVacio[j].inicio = fin;
                    if(listaVacio[i].inicio == listaParticion[j].posicion){
                        listaVacio.erase(listaVacio.begin()+j);
                    }
                    break;
                }else if(fin ==listaVacio[j].fin){
                    listaVacio[j].fin - listaParticion[i].posicion;
                    break;
                }else if(listaParticion[i].posicion > listaVacio[j].inicio && fin < listaVacio[j].fin){
                    Vacio nuevoVacio;
                    nuevoVacio.nombre = "Vacío";
                    nuevoVacio.inicio = listaVacio[j].inicio;
                    nuevoVacio.fin = listaParticion[i].posicion;
                    listaVacio.push_back(nuevoVacio);
                    listaVacio[j].inicio = fin;
                    break;

                }
            }
        }
    }
    return listaVacio;
}

void mostrarMemoria(vector<Particion> &particiones, int tamanyoMemoriaTotal) {
    int memoriaLibre = tamanyoMemoriaTotal;
    for (int i = 0; i < particiones.size(); i++) {
        Particion particion = particiones[i];
        // Dibuja la barra de la partición
        cout << "\033[31m"; // Pone el texto en rojo
        for (int j = 0; j < (particion.tamaño * 50) / tamanyoMemoriaTotal; j++) {
            cout << "█";
        }
        cout << "\033[0m"; // Reestablece el color
        // Muestra el nombre del proceso y el tamaño que ocupa
        cout << " " << particion.proceso.nombre << " (" << particion.tamaño << ") "<<endl;
        memoriaLibre -= particion.tamaño;
    }
    // Dibuja la barra de memoria libre
    cout << "\033[37m"; // Pone el texto en gris
    for (int j = 0; j < (memoriaLibre * 50) / tamanyoMemoriaTotal; j++) {
        cout << "█";
    }
    cout<<" Memoria libre( "<<memoriaLibre<<")"<<endl;
    cout << "\033[0m" << endl; // Reestablece el color
}



void controlMemoria(vector<Particion> &listaParticion, vector<Proceso> &colaProcesos, char opcion) {
    Particion auxParticion;
    int contador = 0;
    vector<Vacio> vacio;

    while (colaProcesos.size() > 0) {
        // Añadimos a la lista de particiones los procesos que llegan en este tiempo
        for (long unsigned int i = 0; i < colaProcesos.size(); i++) {
            if (contador == colaProcesos[i].llegada) {
                auxParticion.proceso = colaProcesos[i];
                vacio = vacioFragmento(listaParticion);
                if (opcion == '1') {
                    auxParticion.posicion = asignarPorPeorHueco(vacio, colaProcesos[i]);
                }
                else if (opcion == '2') {
                    auxParticion.posicion = asignarPorMejorHueco(vacio, colaProcesos[i]);
                }
                auxParticion.tamaño = colaProcesos[i].tamanyo;
                listaParticion.push_back(auxParticion);
                colaProcesos.erase(colaProcesos.begin() + i);
                i--;
            }
        }

        // Decrementamos el tiempo restante de ejecución de cada proceso en la memoria
        for (long unsigned int i = 0; i < listaParticion.size(); i++) {
            listaParticion[i].proceso.tiempo--;
            if (listaParticion[i].proceso.tiempo == 0) {
                colaProcesos.erase(colaProcesos.begin() + i);
                listaParticion.erase(listaParticion.begin() + i);
                i--;
            }
        }

        contador++;
    }

    mostrarMemoria(listaParticion,2000);
}

void showMenu(){
    cout << "Menú" << endl
       << "1- Peor hueco" << endl
       << "2- Mejor hueco" << endl
       << "Option: ";
}

int main() {
    char opcion;
    // Leemos la lista de procesos del archivo procesos.txt
    vector<Proceso> procesos = leerProcesos("procesos.txt");
    vector<Particion> particiones;
    // Ordenamos la lista de procesos por tiempo de llegada
    sort(procesos.begin(), procesos.end(), compararProcesosPorTiempoLlegada);
    showMenu();
    cin >> opcion;

    switch(opcion){
        case '1':
            cout << "El algoritmo seleccionado es el de peor hueco"<<endl;
            break;
        case '2':
            cout << "El algoritmo seleccionado es el de mejor hueco"<<endl;
            break;
        default:
            cout << "Opción no válida"<<endl;
            exit(-1);
            break;
    }

    // Hacemos el control de memoria
    controlMemoria(particiones,procesos,opcion);

    // Mostramos la memoria en el archivo particiones.txt
    ofstream archivo("particiones.txt");
    if(archivo.is_open()){
        for (long unsigned int i = 0; i < particiones.size(); i++) {
            archivo << '[' << particiones[i].posicion << ' ' << particiones[i].proceso.nombre << ' ' << particiones[i].tamaño << ']' << endl;
        }
        archivo.close();
    }else{
        cout<<"El archivo 'particiones.txt' no se pudo crear"<<endl;
    }
    

    return 0;
}
