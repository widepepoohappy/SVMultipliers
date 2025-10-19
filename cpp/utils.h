
#pragma once
#include <vector>

void creaTriangoloEquilatero(std::vector<std::vector<bool>>& matrice);
void stampaMatrice(const std::vector<std::vector<bool>>& matrice);
bool hasNonZero(const std::vector<int>& v);
void printTogether(const std::vector<std::vector<bool>>& FA_carry,
                      const std::vector<std::vector<bool>>& matrice,
                      const std::vector<std::vector<bool>>& intermediate_matrix);
void stampaMatriceH(const std::vector<std::vector<bool>>& matrice);
void printLastIterationMatrix(const std::vector<std::vector<bool>>& matrice);
int vectorBoolToInt(const std::vector<bool>& bits);

