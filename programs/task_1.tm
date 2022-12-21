#Q = {find_sentence_tail, cp_and_mark_sentence_tail, find_sentence_head, cp_tape0_to_tape1, cp_tape1_to_tape0, halt_accept}

#S = {0,1}

#G = {0,1,_,#}

#q0 = find_sentence_tail

#B = _

#F = {halt_accept}

#N = 2

find_sentence_tail ** ** r* find_sentence_tail
find_sentence_tail _* ** l* cp_and_mark_sentence_tail

cp_and_mark_sentence_tail 0* #0 ** find_sentence_head
cp_and_mark_sentence_tail 1* #1 ** find_sentence_head

find_sentence_head ** ** l* find_sentence_head
find_sentence_head _* ** rr cp_tape0_to_tape1

cp_tape0_to_tape1 0* *0 rr cp_tape0_to_tape1
cp_tape0_to_tape1 1* *1 rr cp_tape0_to_tape1
cp_tape0_to_tape1 #* ** *l cp_tape1_to_tape0

cp_tape1_to_tape0 *0 0_ ll cp_tape1_to_tape0
cp_tape1_to_tape0 *1 1_ ll cp_tape1_to_tape0
cp_tape1_to_tape0 __ ** rr halt_accept