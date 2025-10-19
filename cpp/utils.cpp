
#include "utils.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip> 
#include <sstream>
#include <cmath>

using namespace std;

int findHighestDimension (const vector<vector<bool>>& matrice) {
    int riga_size = 0;
    int max = 0;
    for (const auto& riga: matrice) {
        riga_size = riga.size();
        if(riga_size > max) {
            max = riga_size;
        }
    }
    return max;
}

void creaTriangoloEquilatero(vector<vector<bool>>& matrice) {
    int rows = matrice.size();
    int cols = matrice[0].size(); 
    //cout << "Printing cols" << cols << endl;
    //cout << "Printing rows" << rows << endl;
    // raccolgo i bit
    int n = cols;
    vector<vector<bool>> mCopy;
    vector<bool> cols_content;
    for(int i = 0;i <= rows/2 - 1; i++) {
        for(int j = 0 ; j <= cols-n ;j++){
            cols_content.push_back(matrice[i][j]);
            //cout << i << endl;

        }
        mCopy.push_back(cols_content);
        n -= 1;
        cols_content.clear();
    }
    n = cols-1;
    for(int i = (rows/2); i <= rows - 1; i++) {
        for(int j = cols-n ; j <= cols-1 ;j++){
            cols_content.push_back(matrice[i][j]);
            //cout << i << endl;
        }
        mCopy.push_back(cols_content);
        n -= 1;
        cols_content.clear();
    }
    matrice = mCopy;
}

void stampaMatrice(const vector<vector<bool>>& matrice) {
    for (const auto& riga : matrice) {
        for (bool val : riga) {
            cout << val;
        }
        cout << endl;
    }
}


void printLastIterationMatrix(const vector<vector<bool>>& matrice) {
    int matrix_size = matrice.size();
    int row_dim = findHighestDimension(matrice);
    
    for (int i = 0; i < row_dim ; i++) {
        for (int j = matrix_size-1; j>=0; j--) {
            if(int(matrice[j].size()) < row_dim && i == row_dim - 1){
                cout << "- ";
            } else {
                cout << matrice[j][i] << " ";
            }
        }
     
        cout << endl;
    }
}



void printTogether(const std::vector<std::vector<bool>>& FA_carry,
                      const std::vector<std::vector<bool>>& matrice,
                      const std::vector<std::vector<bool>>& intermediate_matrix) {

    int righe = std::max(FA_carry.size(), std::max(matrice.size(), intermediate_matrix.size()));


    cout << "Printing carry, n-1 matrix + intermediate matrix :" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << std::left << std::setw(10) << "Row"
         << std::setw(20) << "Initial matrix"
         << std::setw(20) << "| FA carries"
         << std::setw(20) << "| Intermediate result" << endl;

    for (int i = 0; i < righe; ++i) {
        // Stampa indice riga
        cout << std::left << std::setw(10) << ("Row " + std::to_string(i));

        // Stampa FA_carry[i]
        std::ostringstream oss_matrix;
        if (i < int(matrice.size())) {
            for (bool val : matrice[i]) oss_matrix << val;
        }
        
        cout << std::setw(20) << oss_matrix.str();

        // Stampa matrice[i]
        std::ostringstream oss_carry;
        if (i < int(FA_carry.size())) {
            for (bool bit : FA_carry[i]) oss_carry << bit << " ";
        }
        cout << std::setw(20) << ("| " + oss_carry.str());

        // Stampa intermediate_matrix[i]
        std::ostringstream oss_intermediate;
        if (i < int(intermediate_matrix.size())) {
            for (bool val : intermediate_matrix[i]) oss_intermediate << val;
        }
        cout << std::setw(20) << ("| " + oss_intermediate.str());

        cout << endl;
    }
}




bool hasNonZero(const std::vector<int>& v) {
    for (int b : v) {
        if (b != 0) return true;
    }
    return false;
}



int vectorBoolToInt(const std::vector<bool>& bits) {
    int result = 0;
    int exp = 0;
    for (bool bit : bits) {
        result += bit*int(pow(2,exp));
        exp++;
    }
    return result;
}

