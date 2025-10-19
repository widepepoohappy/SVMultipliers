
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>   // mkdir, stat
#include <unistd.h>     // access
#include <dirent.h>     // directory listing
#include "generateWallaceSV.h"
#include "utils.h"

using namespace std;

bool folderExists(const string& foldername) {
    struct stat info;
    return stat(foldername.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

void checkIfFolder(string foldername) {
    if (!folderExists(foldername)) {
        if (mkdir(foldername.c_str(), 0777) == 0) {
            cout << "Directory " << foldername << " created.\n";
        } else {
            perror("Error creating directory");
        }
    } else {
        cout << "Folder " << foldername << " already exists.\n";
    }
}

void generate_ANDoperation(int bitwidthA, int bitwidthB) {
     string filePath = "sv/AND.sv";

    if (access(filePath.c_str(), F_OK) == 0) {
        if (remove(filePath.c_str()) == 0) {
            cout << "AND.sv removed successfully.\n";
        } else {
            cerr << "Error removing AND.sv\n";
            exit(-1);
        }
    }

    ofstream file(filePath);
    if (file.is_open()) {
        file << "`timescale 10ns/1ns\n";
        file << "module ANDoperator (input wire [" << bitwidthA << "-1:0" << "] A, \n [" << bitwidthB << "-1:0" << "] B,\n";
        file << "                  output wire [" << bitwidthA*bitwidthB << "-1:0" << "] Out);\n";
        int bitpos = 0;
        for(int i = 0; i < bitwidthB; i++) {
            for(int j = 0; j < bitwidthA; j ++ ) {
                file << "assign Out[" << bitpos << "] = A[" << j << "] & B[" << i << "];\n";
                bitpos++;
            }
        }
        file << "endmodule\n";
        file.close();
        cout << "Done dumping AND.sv file.\n";
    } else {
        cerr << "Error opening the AND.sv file.\n";
    }
}


void generate_FA_sv() {
    string filePath = "sv/FullAdder.sv";

    if (access(filePath.c_str(), F_OK) == 0) {
        if (remove(filePath.c_str()) == 0) {
            cout << "FullAdder.sv removed successfully.\n";
        } else {
            cerr << "Error removing FullAdder.sv\n";
            exit(-1);
        }
    }

    ofstream file(filePath);
    if (file.is_open()) {
        file << "`timescale 10ns/1ns\n";
        file << "module FullAdder (input wire A, B, Cin,\n";
        file << "                  output wire Sum, Cout);\n";
        file << "  assign Sum = A ^ B ^ Cin;\n";
        file << "  assign Cout = A & B | A & Cin | B & Cin;\n";
        file << "endmodule\n";
        file.close();
        cout << "Done dumping FA.sv file.\n";
    } else {
        cerr << "Error opening the FA.sv file.\n";
    }
}

void generate_1stcsa_row_connection(vector<int>& FA_per_col, vector<vector<bool>> matrix, int bitwidth, ofstream& file) {
    /* ASSUMING THE OUTPUT FROM THE BITWISE AND OPERATION IS A SINGLE VECOTOR OF N*M BITS*/    

    int bit_size = FA_per_col.size();
    vector<int> indices;
    int resume = 0;
    for(int i = 0; i <= (bit_size-1)/2; i++) {
        //cout << "Fa per row is :" << FA_per_col[i] << endl;
        int subtract = FA_per_col[i] * 3;        
        int bit_remainders = matrix[i].size() - subtract;
        //cout << "Bit remainder " << bit_remainders << endl;
       
        for(int j = 0; j < FA_per_col[i];j++) {
            for(int k = 0; k < 3; k ++) {
                resume = (k+j*3)*(bitwidth-1);
                indices.push_back(resume + i);
                //cout <<  i + (k+j*3)*(bitwidth-1) << endl;
                //cout << "resume : " << resume << endl;
            }
        }    
        for (int j = 0; j < bit_remainders; j++) {
            if(resume == 0) {
                //cout  << resume + j*(bitwidth-1) + i  << endl;
                indices.push_back(resume + j*(bitwidth-1) + i);
            } else {
                indices.push_back(resume + (j+1)*(bitwidth-1) + i);
                //cout  << resume + (j+1)*(bitwidth-1) + i  << endl;

            }
        }
    }
    for(int i = 1; i < (bit_size-1)/2+1; i++) {
        resume = 0;
        int subtract = FA_per_col[i+(bit_size-1)/2] * 3;        
        int bit_remainders = matrix[i+(bit_size-1)/2].size() - subtract;
        int ioffset = (bitwidth-1)+i*bitwidth;

        for(int j = 0 ; j < FA_per_col[i+(bit_size-1)/2]; j ++) {
            for(int k = 0; k < 3; k ++) {
                resume = (k+j*3)*(bitwidth-1);
                indices.push_back(ioffset + resume);
                //cout << ioffset + resume << endl;
            }
        }
        for (int j = 0; j < bit_remainders; j++) {
            if(resume == 0) {
                //cout  << resume + j*(bitwidth-1) + ioffset  << endl;
                indices.push_back(resume + j*(bitwidth-1) + ioffset );
            } else {
                indices.push_back(resume + (j+1)*(bitwidth-1) + ioffset);
                //cout  << resume + (j+1)*(bitwidth-1) + ioffset  << endl;
            }
        }
    }

    instantiate_signals(matrix, 0, file);
    initialize_wallace_to_and(matrix, indices, file);
}


void generate_CSArow_stage(ofstream& file, vector<int>& FA_per_col, vector<int>& mCopy, vector<vector<bool>> matrix, int stage, bool debug) {
    
    int bit_size = FA_per_col.size();
    int inst_cnt = 0;

    if(!hasNonZero(FA_per_col)){
        return;
    }

    if(debug) {
        cout << "INFO: Generating CSA row structure." << endl;
        file << "/*-----------------------------------------------------*/\n";
        file << "/*------------------ Working on stage " << stage << " -----------------*/\n";    
        file << "/*-----------------------------------------------------*/\n";
        file << "\n";
    }
    int s_1r = 0;
    vector<int> s_p1_cp1;
    s_p1_cp1.resize(bit_size);
    for(int i = 0; i < bit_size; i++) {
        s_p1_cp1[i] = 0;
    }

    instantiate_signals(mCopy, stage+1, file);

    for (int i = 0; i < bit_size; i++) {
        if(debug){
            file << "/*-------- Working on colum " << i << " --------*/\n";    
            cout << "INFO: Working on col " << i << ". Generating " << FA_per_col[i] << " CSA." << endl;
        }

        for (int j = 0; j < FA_per_col[i]; j++) {
            file << "FullAdder fa_inst_s" << stage << "_u" << inst_cnt
                 << " ( .A(pp_s" << stage << "_col" << i << "_row" << j * 3
                 << "), .B(pp_s" << stage << "_col" << i << "_row" << j * 3 + 1
                 << "), .Cin(pp_s" << stage << "_col" << i << "_row" << j * 3 + 2
                 << "), .Sum(pp_s" << stage + 1 << "_col" << i << "_row" << s_1r + s_p1_cp1[i]
                 << "), .Cout(pp_s" << stage + 1 << "_col" << i + 1 << "_row" << s_p1_cp1[i+1]
                 << "));\n";
            inst_cnt++;
            s_1r++;
            s_p1_cp1[i+1]++;
            
        }

        int resume = FA_per_col[i] * 3;
        int bit_remainders = matrix[i].size() - resume;

        if(debug){
            cout << "INFO: Working on col " << i << ". Forwarding " << bit_remainders << " signals." << endl;
        }

        for (int j = 0; j < bit_remainders; j++) {
            file << "assign pp_s" << stage + 1 << "_col" << i << "_row" << s_1r + s_p1_cp1[i]
                 << " = pp_s" << stage << "_col" << i << "_row" << resume + j << ";\n";
            s_1r++;
        }
        s_1r = 0;
    }
}

void instantiate_signals(vector<int>& matrix, int stage, ofstream& file) {
    file << "\n\n" ;
    for(int i = 0; i < int(matrix.size()); i++){
        for(int j = 0; j < int(matrix[i]); j++) {
            file << "wire pp_s" << stage << "_col" << i << "_row" << j << ";\n";
        }
    }
    file << "\n" ;
}

void instantiate_signals(vector<vector<bool>>& matrix, int stage, ofstream& file) {
    file << "\n\n" ;
    for(int i = 0; i < int(matrix.size()); i++){
        for(int j = 0; j < int(matrix[i].size()); j++) {
            file << "wire pp_s" << stage << "_col" << i << "_row" << j << ";\n";
        }
    }
    file << "\n" ;
}

void initialize_wallace_to_and(vector<vector<bool>>& matrix, vector<int>& indices, ofstream& file) {
    int stage = 0;
    int k = 0;
    for(int i = 0; i < int(matrix.size()); i++){
        for(int j = 0; j < int(matrix[i].size()); j++) {
            file << "assign pp_s" << stage << "_col" << i << "_row" << j << " = INput[" << indices[k] << "];\n";
            k++;
        }
    }
    file << "\n" ;
}

void initiliaze_wallace_module(ofstream& file, int bitwidthA, int bitwidthB) {
    
    file << "`timescale 10ns/1ns\n";
    file << "module Wallace_tree (input wire [" << bitwidthA*bitwidthB <<"-1:0] INput,\n";
    file << "                    output wire [" << bitwidthA+bitwidthB <<"-1:0] WALLACEout [1:0]);\n";
    cout << "Done dumping Wallace_tree.sv file initialization.\n";
}


void finalize_wallace(ofstream& file, vector<vector<bool>>& matrix , int last_stage) {
    for(int j = 0 ; j < int(matrix.size()); j ++ ) {
        for( int i = 0; i < int(matrix[j].size()); i ++) {
            file << "\nassign WALLACEout[" << i << "][" << j << "] = pp_s" << last_stage << "_col" << j << "_row" << i << ";";
        }
    }
    file << "\n\nendmodule;\n";
}

void generate_RCA(int bitA, int bitB) { 
    string filePath = "sv/RCA.sv";

    if (access(filePath.c_str(), F_OK) == 0) {
        if (remove(filePath.c_str()) == 0) {
            cout << "RCA.sv removed successfully.\n";
        } else {
            cerr << "Error removing RCA.sv\n";
            exit(-1);
        }
    }

    ofstream file(filePath);
    if (file.is_open()) {
        int max = 0;
        if(bitA >= bitB){
            max = bitA;
        } else {
            max = bitB;
        }
        file << "module RCA (input wire [" << bitA+bitB <<"-1:0] A,\n";
        file << "            input wire [" << bitB+bitB <<"-1:0] B,\n";
        file << "            output wire [" << bitA+bitB <<":0] Sum);\n"; // bit in piu per l'overflow
        file << "   wire [" << bitA+bitB-1 << "-1:0] FA_Cout;\n";
        //file << "   wire [" << max << "-1:0] FA_sum;\n";
        for(int i = 0; i < bitA + bitB; i++){
            if(i == 0) {
                file << "   FullAdder FA_u" << i << " ( .A(A[" << i << "]),";
                file << " .B(B[" << i << "]),";
                file << " .Cin(0),";
                file << " .Sum(Sum[" << i << "]),";
                file << " .Cout(FA_Cout[" << i << "]));\n";
            }else if (i == bitA+bitB-1) { 
                file << "   FullAdder FA_u" << i << " ( .A(A[" << i << "]),";
                file << " .B(B[" << i << "]),";
                file << " .Cin(FA_Cout[" << i-1 << "]),";
                file << " .Sum(Sum[" << i << "]),";
                file << " .Cout(Sum[" << i+1 << "]));\n";
            }else {
                file << "   FullAdder FA_u" << i << " ( .A(A[" << i << "]),";
                file << " .B(B[" << i << "]),";
                file << " .Cin(FA_Cout[" << i-1 << "]),";
                file << " .Sum(Sum[" << i << "]),";
                file << " .Cout(FA_Cout[" << i << "]));\n";
            }
        }
        file << "endmodule;";
        file.close();
        cout << "INFO: Done dumping RCA module " << endl;
    }else{
        cerr << "RCA.sv is not being opened correctly " << endl;
    }
}

void instantiate_mul_unit(int bitA, int bitB){
    string filePath = "sv/mul_unit.sv";
    if (access(filePath.c_str(), F_OK) == 0) {
        if (remove(filePath.c_str()) == 0) {
            cout << "RCA.sv removed successfully.\n";
        } else {
            cerr << "Error removing RCA.sv\n";
            exit(-1);
        }
    }

    ofstream file(filePath);
    if (file.is_open()) {
        file << "`timescale 10ns/1ns\n";
        file << "module mul_unit(   input wire [" << bitA << "-1:0] A,\n";
        file << "                   input wire [" << bitB << "-1:0] B,\n";
        file << "                   input wire clk, rst, done,\n";
        file << "                   output wire [" << bitA + bitB << "-1:0] Sout);\n";
        file << "\n";
        file << "\n";
        file << "   wire [" << bitA*bitB << "-1:0] ANDout;\n";
        file << "   wire [" << bitA + bitB << "-1:0] WALLACEout [1:0];\n";
        file << "\n";
        file << "\n";
        file << "   ANDoperator AND_u (         .A(A),\n";
        file << "                               .B(B),\n";
        file << "                               .Out(ANDout));\n";
        file << "   Wallace_tree wallace_u (   .INput(ANDout),\n";
        file << "                               .WALLACEout(WALLACEout));\n";
        file << "\n";
        file << "   wire [" << bitA+bitB << "-1:0] RCA_out;\n";
        file << "   RCA RCA_u (                 .A(WALLACEout[0]),\n";
        file << "                               .B(WALLACEout[1]),\n";
        file << "                               .Sum(RCA_out));\n";
        file << "endmodule;";
        file.close();
        cout << "Done dumping MUL unit file" << endl;
    } else { 
        cerr << "Error while opening MUL.sv file" << endl;
        exit(-1);
    }
}
