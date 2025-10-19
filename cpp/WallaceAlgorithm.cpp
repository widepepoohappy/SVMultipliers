
#include <iostream>
#include <vector>
#include <cstdint>
#include <cmath>
#include "utils.h"
#include "generateWallaceSV.h"
#include <algorithm>
#include <fstream>
#include <string>

using namespace std;

// Funzione per estrarre i bit in un vector<bool>
vector<bool> extractBits(int64_t number, int bitWidth) {
    vector<bool> bits(bitWidth);
    for (int i = 0; i < bitWidth; ++i) {
        bits[i] = (number >> i) & 1;
    }
    return bits;
}

vector<int> evaluate_FAnumber(vector<vector<bool>> matrice, vector<int>& HA_per_col, vector<int>& mCopy, int& FA_number, int& HA_number, bool DADDA = 0, bool debug = 0) {
//Based on the bit, if in column there are 3 bit available then we deicde to generate the FA working on those 3 bits
    int rows = matrice.size();
    //cout << rows << endl;
    
    // number of FA evaluation
    vector<int> tmp_mCopy(mCopy.size(),0);
    vector<int> FA_per_col;
    int bitCount = 0;
    int nFa = 0;
    int nHa = 0;
    int produceHA = 1;
    bool second_event = 0;
    bool start = 0;
    for (int i = 0; i <= rows-1; i++) {
        bitCount = mCopy[i];
        nFa = bitCount / 3;
        if(nFa > 0) {
            start = 1;
            if(second_event){
                produceHA = 0;
            }
            second_event = 1;
        }
        if (DADDA && produceHA && start) {
            //cout << "Printing start" << start << endl;
            nFa = 0;
            nHa = bitCount / 2;
        }
        FA_per_col.push_back(nFa);
        HA_per_col.push_back(nHa);
        //cout << "Printing nHa " << nHa << endl;
        FA_number += nFa;
        HA_number += nHa;
        int bit_remainder = bitCount - (nFa*3+nHa*2);

        mCopy[i] = nFa + nHa + bit_remainder + tmp_mCopy[i];
        
        if (nFa > 0 || nHa > 0) {

            // Aggiungi carry alla colonna successiva
            if (i + 1 < rows) {
                //nHa and nFa cannot have values simultaneously
                tmp_mCopy[i + 1] += nFa + nHa;
            } else {
                tmp_mCopy.push_back(nFa + nHa);
            }
        }
        nHa = 0;
     }


    // Stampa finale
    if(debug) {
        cout << "mCopy ridotta (numero di bit per colonna):" << endl;
        for (size_t i = 0; i < mCopy.size(); i++) {
           cout << "Col " << i << ": " << mCopy[i] << " bit" << endl;
        }
    }
    return FA_per_col;
}

int main(int argc, char* argv[])  {
    int8_t A = 19;
    int8_t B = 10;

    bool generate_hardware = 0;
    string filePath = "sv/Wallace_tree.sv";

    int bitA = sizeof(A) * 8;
    int bitB = sizeof(B) * 8;
    //int Nstage = ceil(1.7*log2(bitB)-1.7);

    bool DADDA = 0; 
    bool debug = 0;

    for (int i = 1; i < argc; i++) { // i parte da 1 per saltare il nome del programma
        string arg = argv[i];
        if (arg == "DADDA") {
            DADDA = true;
        } else if (arg == "debug") {
            debug = true;
        } else if (arg == "generate_hw") {
            generate_hardware = 1;
        }
    }
    
    cout << "Size of A: " << bitA << " bit" << endl;
    cout << "Size of B: " << bitB << " bit" << endl;

    int cols = bitA + bitB;
    int rows = bitB;

    // Matrice C[cols][rows] inizializzata a 0
    vector<vector<bool>> C(cols, vector<bool>(rows, 0));

    vector<bool> bitsA = extractBits(A, bitA);
    vector<bool> bitsB = extractBits(B, bitB);


    // MULTIPLICATION COMPUTATION STAGE
    cout << "Multiplying: " << int(A) << " * " << int(B) << endl;
    for (int i = bitA - 1; i >= 0; i--) cout << bitsA[i];
    cout << " * ";
    for (int i = bitB - 1; i >= 0; i--) cout << bitsB[i];
    cout << endl;

    for (int i = 0; i < bitB; i++) {
        for (int j = 0; j < bitA; j++) {
            // defined as cols - rows
            C[j + i][i] = bitsA[j] & bitsB[i];
        }
    }

    // WALLACE EVALUATION STAGE
    cout << "Result matrix: " << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = cols-1; j >= 0; j--) {
            if (i > j || j >= bitA + i) {
                cout << "- ";
            } else {
                cout << C[j][i] << " ";
            }
        }
        cout << endl;
    }
    //stampaMatrice(C);
    creaTriangoloEquilatero(C);
    //stampaMatrice(C);
    // setting new rows for new C representation
    rows = C.size();
    
    checkIfFolder();
    ofstream file(filePath);
    if(generate_hardware){
        if (!file.is_open()) {
        cerr << "Error opening the Wallace_tree.sv file.\n";
        }
    }

    int FA_number = 0;
    int HA_number = 0;
    vector<int> HA_per_row;
    vector<int> mCopy;
    for(int i = 0; i < C.size(); i++){
        mCopy.push_back(C[i].size());
    }
    vector<int> FA_per_row = evaluate_FAnumber(C, HA_per_row, mCopy, FA_number, HA_number, DADDA, debug);

    vector<bool> remainders;
    vector<bool> partial_results;
    vector<vector<bool>> intermediate_matrix;
    vector<vector<bool>> FA_carry;
    vector<bool> combined_vectors;

    FA_carry.resize(rows + 1); // Prepara spazio per tutti i carry, incluso l'ultimo
    int iteration = 0;
    
    if(generate_hardware) {
        initiliaze_wallace_module(file, bitA, bitB);

        generate_1stcsa_row_connection(FA_per_row, C, bitA, file);

        generate_CSArow_stage(file, FA_per_row, mCopy, C, iteration);
    }
    while (hasNonZero(FA_per_row))  {
        for (int i = 0; i <= rows - 1; i++) {
            int length = (FA_per_row[i] * 3 + HA_per_row[i]*2);
            //cout << FA_per_row[i] << endl;
            //cout << "i = " << i << endl;
            // Estrai i bit rimanenti dalla riga corrente
            remainders.assign(C[i].begin() + length, C[i].end());

            partial_results.clear();
            combined_vectors.clear();
            for (int j = 0; j < FA_per_row[i]; j++) {
                // Calcolo della somma FA
                bool FA_sum = C[i][j * 3] ^ C[i][j * 3 + 1] ^ C[i][j * 3 + 2];
                partial_results.push_back(FA_sum);

                // Calcolo del carry FA
                bool carry = (C[i][j * 3] & C[i][j * 3 + 1]) |
                             ((C[i][j * 3] | C[i][j * 3 + 1]) & C[i][j * 3 + 2]);
                FA_carry[i + 1].push_back(carry);
            }
            for(int j = 0; j < HA_per_row[i]; j++) {
                bool HA_sum = C[i][j * 3] ^ C[i][j * 3 + 1];
                partial_results.push_back(HA_sum);

                // Calcolo del carry FA
                bool carry = (C[i][j * 3] & C[i][j * 3 + 1]);
                FA_carry[i + 1].push_back(carry);
            }
            // Combina i risultati parziali, i resti e i carry precedenti
            combined_vectors.insert(combined_vectors.end(), partial_results.begin(), partial_results.end());
            combined_vectors.insert(combined_vectors.end(), remainders.begin(), remainders.end());
            combined_vectors.insert(combined_vectors.end(), FA_carry[i].begin(), FA_carry[i].end());

            // Aggiunge la nuova riga alla matrice intermedia
            intermediate_matrix.push_back(combined_vectors);
        }

        // Stampa la matrice aggiornata
        printTogether(FA_carry, C, intermediate_matrix);

        // Aggiorna la matrice C con i nuovi risultati
        C = intermediate_matrix;

        // Pulisce le strutture per il prossimo ciclo
        intermediate_matrix.clear();
        partial_results.clear();
        remainders.clear();
        combined_vectors.clear();
            
        

        FA_carry.clear();
        FA_carry.resize(rows + 1); // Ricrea la struttura FA_carry per il prossimo ciclo
        // Ricalcola il numero di FA per riga
        HA_per_row.clear();
        FA_per_row = evaluate_FAnumber(C, HA_per_row, mCopy, FA_number, HA_number, DADDA, debug);

        iteration++;
        if(generate_hardware){
            generate_CSArow_stage(file, FA_per_row, mCopy, C, iteration);
        }
    }
    if (generate_hardware) {
        finalize_wallace(file, C, iteration);    
    }
    cout << endl << "Wallace tree generation results:" << endl;
    cout << "Total number of FA used: " << FA_number << endl;
    cout << "Final intermediate result:" << endl;
    printLastIterationMatrix(C);
    
    // CARRY PROPAGATION STAGE

    cout << endl <<  "Performing RCA operation" << endl;
    vector<bool> RCA_Sum;
    vector<bool> RCA_carry;
    RCA_carry.resize(rows);
    RCA_Sum.resize(rows-1);
    // We only have 2 rows as Wallace algorithm leaves out 2 rows of bit to add
    for (int j = 0; j <= rows-1; j++) {
        if(int(C[j].size()) < 2){
            C[j].push_back(0);
        }
        bool FA_sum = C[j][0] ^ C[j][1] ^ RCA_carry[j];
        bool carry = (C[j][0] & C[j][1]) |
                        ((C[j][0] | C[j][1]) & RCA_carry[j]);
        RCA_carry[j + 1] = carry;
        RCA_Sum[j] = FA_sum;    
    }
    for(int i = rows-1; i >= 0;i--) {
        cout << RCA_Sum[i] << " ";   
    }
    cout << endl;
    cout << "Multiplication result of " << int(A) << " * " << int(B) << " : " << vectorBoolToInt(RCA_Sum) << endl;
 
    
    if(generate_hardware){
        generate_FA_sv();
        generate_ANDoperation(bitA,bitB);
        generate_RCA(bitA,bitB);
        instantiate_mul_unit(bitA,bitB);
        file.close();
    }
    return 0;
}

