module RCA (input wire [16-1:0] A,
            input wire [16-1:0] B,
            output wire [16:0] Sum);
   wire [15-1:0] FA_Cout;
   FullAdder FA_u0 ( .A(A[0]), .B(B[0]), .Cin(0), .Sum(Sum[0]), .Cout(FA_Cout[0]));
   FullAdder FA_u1 ( .A(A[1]), .B(B[1]), .Cin(FA_Cout[0]), .Sum(Sum[1]), .Cout(FA_Cout[1]));
   FullAdder FA_u2 ( .A(A[2]), .B(B[2]), .Cin(FA_Cout[1]), .Sum(Sum[2]), .Cout(FA_Cout[2]));
   FullAdder FA_u3 ( .A(A[3]), .B(B[3]), .Cin(FA_Cout[2]), .Sum(Sum[3]), .Cout(FA_Cout[3]));
   FullAdder FA_u4 ( .A(A[4]), .B(B[4]), .Cin(FA_Cout[3]), .Sum(Sum[4]), .Cout(FA_Cout[4]));
   FullAdder FA_u5 ( .A(A[5]), .B(B[5]), .Cin(FA_Cout[4]), .Sum(Sum[5]), .Cout(FA_Cout[5]));
   FullAdder FA_u6 ( .A(A[6]), .B(B[6]), .Cin(FA_Cout[5]), .Sum(Sum[6]), .Cout(FA_Cout[6]));
   FullAdder FA_u7 ( .A(A[7]), .B(B[7]), .Cin(FA_Cout[6]), .Sum(Sum[7]), .Cout(FA_Cout[7]));
   FullAdder FA_u8 ( .A(A[8]), .B(B[8]), .Cin(FA_Cout[7]), .Sum(Sum[8]), .Cout(FA_Cout[8]));
   FullAdder FA_u9 ( .A(A[9]), .B(B[9]), .Cin(FA_Cout[8]), .Sum(Sum[9]), .Cout(FA_Cout[9]));
   FullAdder FA_u10 ( .A(A[10]), .B(B[10]), .Cin(FA_Cout[9]), .Sum(Sum[10]), .Cout(FA_Cout[10]));
   FullAdder FA_u11 ( .A(A[11]), .B(B[11]), .Cin(FA_Cout[10]), .Sum(Sum[11]), .Cout(FA_Cout[11]));
   FullAdder FA_u12 ( .A(A[12]), .B(B[12]), .Cin(FA_Cout[11]), .Sum(Sum[12]), .Cout(FA_Cout[12]));
   FullAdder FA_u13 ( .A(A[13]), .B(B[13]), .Cin(FA_Cout[12]), .Sum(Sum[13]), .Cout(FA_Cout[13]));
   FullAdder FA_u14 ( .A(A[14]), .B(B[14]), .Cin(FA_Cout[13]), .Sum(Sum[14]), .Cout(FA_Cout[14]));
   FullAdder FA_u15 ( .A(A[15]), .B(B[15]), .Cin(FA_Cout[14]), .Sum(Sum[15]), .Cout(Sum[16]));
endmodule;