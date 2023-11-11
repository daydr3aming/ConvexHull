#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stack>

using namespace std;

// compilacion: g++ -std=c++11 main.cpp -o main
// ejecucion: ./main.exe < in.txt o ./main.out < in.txt en ambiente linux o git bash
// in.txt, primera linea es n puntos, seguidos de n lineas con los puntos en formato {x, y} o (x, y)

// Actividad 4.1 Implementación Polígonos Convexos aplicando geometría computacional.
// Rogelio Guzman Cruzado - A01639914
// David Alejandro González Ibarra - A01633817 

// Encuentra la orientacion de tres puntos (Pa, Pb, Pb) que forman un angulo
// Regresa 0 si los puntos son colineales, 1 si hacen un giro counterclockwise, y -1 si hacen un giro clockwise.
// Esto sirve como una forma de saber si tres puntos crean un angulo de menos de 180 grados o mas de 180 grados
// Debido a que si se forma un giro clockwise, el angulo logicamente es mayor a 180 grados, y viscerversa
// Funcion adaptada de GeeksForGeeks de Graham Scan example: https://www.geeksforgeeks.org/convex-hull-using-graham-scan/#:~:text=int%20orientation(Point,%7D
// Recuperado el: 10/11/2023
int orientation(pair<int, int> Pa, pair<int, int> Pb, pair<int, int> Pc) {
    int val = (Pb.second - Pa.second) * (Pc.first - Pb.first) -
              (Pb.first - Pa.first) * (Pc.second - Pb.second);

    if (val == 0) return 0;  // Colineal
    return (val > 0) ? 1 : -1;  // Clockwise o Counterclockwise
}
// Funcion de utilidad que regresa el cuadrado de la distancia entre dos puntos
// Funcion adaptada de GeeksForGeeks Graham Scan example: https://www.geeksforgeeks.org/convex-hull-using-graham-scan/#:~:text=int%20distSq(Point,%7D
// Recuperado en: 10/11/2023
int dist(pair<int, int> Pa, pair<int, int> Pb) {
    return (Pa.first - Pb.first) * (Pa.first - Pb.first) +
           (Pa.second - Pb.second) * (Pa.second - Pb.second);
}

// Sorteamos los puntos de acuerdo a la magnitud del angulo que forman con el punto P
bool polarOrder(pair<int, int> p, pair<int, int> q, const vector<pair<int, int>>& points) {
    // points[0] == P
    int orientation_val = orientation(points[0], p, q);
    if (orientation_val == 0) {
        // Si dos puntos tienen el mismo angulo, borramos el punto con menor magnitud
        return (dist(points[0], p) < dist(points[0], q));
    }
    return (orientation_val == -1);
}

// Funcion de utilidad para obtener el siguiente en el stack
// Propositos de codigo limpio
// Funcion adaptada de GeeksForGeeks Graham Scan example: https://www.geeksforgeeks.org/convex-hull-using-graham-scan/#:~:text=Point%20nextToTop(stack,%7D
// Recuperado en: 10/11/2023
pair<int, int> nextToTop(stack<pair<int, int>>& S) {
    pair<int, int> top = S.top();
    S.pop();
    pair<int, int> next_top = S.top();
    S.push(top);
    return next_top;
}

// Algoritmo Graham Scan. Complejidad Temporal: O(nlogn)
stack<pair<int, int>> GrahamScan(vector<pair<int, int>> points){

    // Numero de puntos
    int n = points.size();
    // Para hacer un convex hull necesitamos al menos 3 puntos
    if (n < 3) {
        throw invalid_argument("Se necesitan al menos 3 puntos para hacer un convex hull");
        return stack<pair<int, int>>();
    }
    // Encontramos P encontrando ymin
    int ymin = points[0].second, min = 0;
    for (int i = 1; i < n; i++){
        int y = points[i].second;
        if ((y < ymin) || (ymin == y &&
            points[i].first < points[min].first))
        ymin = points[i].second, min = i;
    }
    // Hacemos que P sea el punto origen
    swap(points[0], points[min]);

    // Sorteamos los puntos dependiendo de su magnitud de angulo con P
    sort(points.begin() + 1, points.end(), [&points](const pair<int, int>& a, const pair<int, int>& b) {
        return polarOrder(a, b, points);
    });

    // Iniciamos el convex hull con P y los primeros 2 puntos, ya que sabemos que siempre forman parte del convex hull
    stack<pair<int, int>> hull;
    hull.push(points[0]);
    hull.push(points[1]);
    hull.push(points[2]);

    // Eliminamos los puntos que crean un angulo mayor a 180 grados, ya que no forman parte del convex hull
    for (int i = 3; i < n; i++) {
        while (orientation(nextToTop(hull), hull.top(), points[i]) != -1)
            hull.pop();
        hull.push(points[i]);
    }
    return hull;
}

// Codigo driver
int main() {

    // Input test recuperado de: https://www.geeksforgeeks.org/convex-hull-using-graham-scan/#:~:text=input_points%20%3D%20%5B(,%2C%203)%5D
    int n;
    cin >> n;
    vector<pair<int, int>> points(n);
    char c;
    int x, y;
    for (int i = 0; i < n; i++) {
        cin >> c >> x >> c >> y >> c;
        points[i] = make_pair(x, y);
    }
    
    // Imprimimos los puntos que forman parte del convex hull en orden counterclockwise
    stack<pair<int, int>> convexHull= GrahamScan(points);
    while (!convexHull.empty()) {
        pair<int, int> p = convexHull.top();
        cout << "(" << p.first << ", " << p.second << ")" << endl;
        convexHull.pop();
    }
    return 0;
}

