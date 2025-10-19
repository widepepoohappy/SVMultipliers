`timescale 10ns/1ns
module mul_unit(   input wire [8-1:0] A,
                   input wire [8-1:0] B,
                   input wire clk, rst, done,
                   output wire [16-1:0] Sout);


   (* keep *) wire [64-1:0] ANDout;
   (* keep *) wire [16-1:0] WALLACEout [1:0];


   (* keep *) ANDoperator AND_u (         .A(A),
                               .B(B),
                               .Out(ANDout));
   (* keep *) Wallace_tree wallace_u (   .INput(ANDout),
                               .WALLACEout(WALLACEout));

   (* keep *) wire [16-1:0] RCA_out;
   (* keep *) RCA RCA_u (                 .A(WALLACEout[0]),
                               .B(WALLACEout[1]),
                               .Sum(RCA_out));
assign Sout = RCA_out;
endmodule;



