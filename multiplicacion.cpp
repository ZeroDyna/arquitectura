#include <iostream>
#include <cmath> //para uso de isinf() y isnan() y redondeo
using namespace std;

const int SESGO = 127;
const int EXP_MAX = 128; // Exponente máximo permitido en IEEE 754
const int EXP_MIN = -126; // Exponente mínimo permitido en IEEE 754

// Función para redondear al más cercano
float roundToNearest(float number) {
    return roundf(number); 
}

// Paso 1: Verificar si hay un 0
void mult(unsigned int* A, unsigned int* B) {
    unsigned int r = 0;
    if (*A == 0 || *B == 0) {
        cout << "La respuesta es 0" << endl;
        return; // Si uno de los numeros es 0 todo sale 0 y sale de la funcion
    } 
    if (*A != 0 || *B != 0){
        // Paso 2: Suma de exponentes
        int exponente_A = ((*A >> 23) & 0xFF) - SESGO;
        int exponente_B = ((*B >> 23) & 0xFF) - SESGO;

        // Realizar la suma de exponentes
        int suma_exponentes = exponente_A + exponente_B;

        // Paso 3: Aplicar el descuento de sesgo
        int exponente_real = suma_exponentes + SESGO;

        // Paso 4 verificar overflow y underflow de exponentes junto con el Paso 5(reportar si hay un Overflow)
        if (exponente_real > EXP_MAX) {
            cout << "Overflow de exponente" << endl;
        } 
        if (exponente_real < EXP_MIN) {
            cout << "Underflow de exponente" << endl;
        } 

            // exponente real
            cout << "Exponente real: " << exponente_real << endl;

            // Paso 6: Multiplicación de las mantisas (significandos)
            
            // Extraer las mantisas de A y B (los bits después del bit de signo y antes del "punto decimal")
            unsigned int mantisa_A = *A & 0x7FFFFF;
            unsigned int mantisa_B = *B & 0x7FFFFF;

            // Realizar la multiplicación de las mantisas
            unsigned long long mantisa_resultante = static_cast<unsigned long long>(mantisa_A) * mantisa_B;

            // Paso 7 aplicar redondeo al más cercano al resultado de la mantisa
            float resultado_final = static_cast<float>(mantisa_resultante) / static_cast<float>(1ULL << 23);
            resultado_final = roundToNearest(resultado_final);

            // Paso 8 ensamblar el resultado final en un registro de 32 bits punto flotante
            unsigned int signo_resultado = (*A & 0x80000000) ^ (*B & 0x80000000); // Bit de signo
            unsigned int exponente_resultado = (exponente_real + SESGO) << 23; // Exponente
            unsigned int mantisa_resultado = static_cast<unsigned int>(mantisa_resultante >> 23) & 0x7FFFFF; // Mantisa

            unsigned int resultado_ensamblado = signo_resultado | exponente_resultado | mantisa_resultado;

            // Imprimir el resultado final ensamblado en binario
            cout << "Resultado final ensamblado en binario: ";
            for (int i = 31; i >= 0; i--) {
                cout << ((resultado_ensamblado >> i) & 1);
            }
            cout << endl;

            // Imprimir el resultado final
            cout << "Resultado final: " << resultado_final << endl;
        
    }
}

int main() {
    float n1;
    cin >> n1;
    float n2;
    cin >> n2;

    // Obtener un puntero al número de punto flotante
    unsigned int* puntero = reinterpret_cast<unsigned int*>(&n1);
    unsigned int* puntero1 = reinterpret_cast<unsigned int*>(&n2);

    // Obtener la representación en binario del número de punto flotante por punteros
    unsigned int bits = *puntero;
    unsigned int bits1 = *puntero1;
    mult(puntero, puntero1);

    // Imprimir la representación en binario a traves de un bucle
    cout << "Número de punto flotante en binario: ";
    for (int i = 31; i >= 0; i--) {
        cout << ((bits >> i) & 1);
    }
    cout << endl;
    cout << "Número 2 de punto flotante en binario: ";
    for (int i = 31; i >= 0; i--) {
        cout << ((bits1 >> i) & 1);
    }
    cout << endl;

    return 0;
}
