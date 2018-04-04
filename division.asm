seti 128;
lw r1; // r1 = MSW of dividend
lw r14; // r14 = MSW of dividend
seti 129;
lw r2; // r2 = LSW of dividend
seti 130;
lw r3; // r3 is a divisor
lw r15; // r15 is a divisor
seti 16;
mov r4; 
seti 0;
mov r5; // r5 is a shifting counter
seti 0;
mov r6;
seti 128;
mov r7; // r7 is a bitmask(10000000b)
seti 0;
mov r8; // MSW of quotient
mov r9; // LSW of quotient
set r1; // R[r0] = R[r1]
and r7;
bez 0111; // go to next_check // if R[r0] == 0, no 2's complement
seti 0; // void instruction
set r2; //r0 = r2
bez 0100; // go to over_flow
flip r1;
flip r2;
addone r2; // general 2's complement
seti 0;
bez 0011; // go to next_check
flip r1; (over_flow)
seti 0; // void instruction
addone r1;
set r3; (next_check)
and r7;
bez 0011; // go to division
seti 0; // void instruction
flip r3;
addone r3; // 2's complement done for a divisor
slg r2; (division)
slo r1;
slo r6;
set r6; // r0 = r6
sub r3; // R[r0] = R[r0] - R[r3]
mov r10;
and r7;
slg r9;
slo r8;
bnez 0100; // go to check_point //if r0 != 0, it means r6 < r3, so do not subtract!
addone r9; // Subtraction is possible, so quotient increases by one
set r10; 
mov r6; // store a subtracted value in r6
addone r5; (check_point)
set r5;
xor r4; // compare a shifting counter with 16
bnez 1010; // go to division 
set r14;
and r7;
mov r14;
set r15;
and r7;
xor r14;
bez 0110; // go to Final_Attack
set r9;
bnez 0100; // go to two_flip
flip r8;
addone r8;
seti 0;
bez 0011; // go to Final_Attack
flip r8; // Two_Flip
flip r9;
addone r9;
seti 127; // Final_Attack
sw r9;
seti 126;
sw r8;
halt;