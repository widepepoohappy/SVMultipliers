
#ifndef FA_GENERATOR_H
#define FA_GENERATOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>   // mkdir, stat
#include <unistd.h>     // access

using namespace std;

bool folderExists(const string& foldername);
void checkIfFolder(string foldername = "sv");
void generate_FA_sv();
void generate_ANDoperation(int bitwidthA, int bitwidthB);
void instantiate_signals(vector<int>& matrix, int stage, ofstream& file);
void instantiate_signals(vector<vector<bool>>& matrix, int stage, ofstream& file);
void generate_wallace_sv(vector<vector<bool>>& matrix, vector<int>& FA_per_stage);
void generate_wallace_stage_sv(vector<bool>& matrix_col, int FA_per_stage);
void generate_1stcsa_row_connection(vector<int>& FA_per_col, vector<vector<bool>> matrix, int bitwidth, ofstream& file);
void generate_CSArow_stage(ofstream& file, vector<int>& FA_per_col, vector<int>& mCopy, vector<vector<bool>> matrix, int stage, bool debug = 0);
void initialize_wallace_to_and(vector<vector<bool>>& matrix, vector<int>& indices, ofstream& file);
void initiliaze_wallace_module(ofstream& file, int bitwidthA, int bitwidthB);
void finalize_wallace(ofstream& file, vector<vector<bool>>& matrix , int last_stage);
void generate_RCA(int bitA, int bitB);void instantiate_mul_unit(int bitA, int bitB);
void instantiate_mul_unit(int bitA, int bitB);


#endif // FA_GENERATOR_H

