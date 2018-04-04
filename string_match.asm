seti 9; // R[r0] = 9
lw r1; // r1 = sequence string
seti 0; // R[r0] = 0
mov r5; // R[r5] = 0, r5 stores the result(match count)
seti 5; // R[r0] = 5
mov r6; // R[r6] = 5, r6 stores the number of comparisons(5)
seti 32; // R[r0] = 32
mov r7; // R[r7] = 32, r7 stores a memory location
seti 96; // R[r0] = 96
mov r8; // R[r8] = 96, r8 stores a finish point
seti 15;
mov r9; // R[r9] = 00001111b, r9 is a bitmask
seti 0; (outer_loop_modified)
mov r10; // r10 is a inner_loop counter
set r7; // R[r0] = R[r7](outer_loop)
lw r4; // R[r4] = a string from a memory location
set r9; // (inner_loop)R[r0] = a bitmask 00001111b
and r4; // R[r0] = R[r0] & R[r4]
xor r1;
bnez 0100; // go to check_point
addone r5;
seti 255; // r0 = 255
xor r5;
bez 0110; // go to conclusion
srg r4; // r4 = r4 >> 1 (check_point)
addone r10;
set r10;
xor r6;
bnez 1100; // go to innner_loop
addone r7;
set r7; r0 = r7
xor r8;
bnez 1000; // go to outer_loop
seti 10; (conclusion)
sw r5; // storing the result at the MEM[10]
halt;
