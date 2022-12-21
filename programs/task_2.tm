#Q = {0,accept,reject,find_tape_23_head,n_self_increase,generate_string|main,generate_string|find_tape_3_head,generate_string|tape_2_step,compare_tape_01,find_tape_01_head,reject|find_tape_0_head_and_clean,accept|find_tape_0_head_and_clean,accept|t,accept|tr,accept|tru,accept|true,reject|f,reject|fa,reject|fal,reject|fals,reject|false,halt_accept}

#S = {0,1}

#G = {0,1,_,#,t,r,u,e,f,a,l,s}

#q0 = 0

#B = _

#F = {halt_accept}

#N = 4

0 _*** **** **** accept
0 **** **** **** n_self_increase

n_self_increase **** **** **rr n_self_increase
n_self_increase **_* **_* ***r n_self_increase
n_self_increase ***_ ***_ **r* n_self_increase
n_self_increase **__ **## **** find_tape_23_head

find_tape_23_head **** **** **ll find_tape_23_head
find_tape_23_head **__ **** **rr generate_string|main

generate_string|main **## *1** *r*r generate_string|main
generate_string|main **#_ **** ***l generate_string|find_tape_3_head
generate_string|main **_* **** *ll* find_tape_1_head

find_tape_1_head **** **** *l** find_tape_1_head
find_tape_1_head *_** **** *r** compare_tape_01

generate_string|find_tape_3_head **** **** ***l generate_string|find_tape_3_head
generate_string|find_tape_3_head ***_ **** ***r generate_string|tape_2_step

generate_string|tape_2_step **** **** **r* generate_string|main

compare_tape_01 11** **** rr** compare_tape_01
compare_tape_01 1_** **** ll** find_tape_01_head
compare_tape_01 _1** **** l*** reject|find_tape_0_head_and_clean
compare_tape_01 __** **** l*** accept|find_tape_0_head_and_clean

find_tape_01_head **** **** ll** find_tape_01_head
find_tape_01_head __** **** rr** n_self_increase

accept|find_tape_0_head_and_clean **** _*** l*** accept|find_tape_0_head_and_clean
accept|find_tape_0_head_and_clean _*** **** r*** accept

reject|find_tape_0_head_and_clean **** _*** l*** reject|find_tape_0_head_and_clean
reject|find_tape_0_head_and_clean _*** **** r*** reject

accept **** t*** r*** accept|t
accept|t **** r*** r*** accept|tr
accept|tr **** u*** r*** accept|tru
accept|tru **** e*** r*** accept|true

reject **** f*** r*** reject|f
reject|f **** a*** r*** reject|fa
reject|fa **** l*** r*** reject|fal
reject|fal **** s*** r*** reject|fals
reject|fals **** e*** r*** reject|false

accept|true **** **** l*** halt_accept
reject|false **** **** l*** halt_accept