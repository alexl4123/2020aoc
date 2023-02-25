% Input: calory(<ELFID>,<ELFTHINGYID>,<WEIGHT>).
% Input: elf(<ELFID>)

elf(ELFID) :- calory(ELFID, A, B).


elf_sum(ELFID, S) :- elf(ELFID), S = #sum{WEIGHT,ELFTHINGYID : calory(ELFID, ELFTHINGYID, WEIGHT)}.

max_elf(S, ELFID) :- S = #max{S2,ELFID2 : elf_sum(ELFID2,S2)}, elf_sum(ELFID, S).
max_elf_2(S, ELFID) :- S = #max{S2,ELFID2 : elf_sum(ELFID2,S2), not max_elf(S2, ELFID2)}, elf_sum(ELFID, S).
max_elf_3(S, ELFID) :- S = #max{S2,ELFID2 : elf_sum(ELFID2,S2), not max_elf(S2, ELFID2), not max_elf_2(S2, ELFID2)}, elf_sum(ELFID, S).

elf_three_best_sum(S) :- max_elf(S1, ELFID), max_elf_2(S2, E2), max_elf_3(S3, E3), S = S1 + S2 + S3.

#show max_elf/2.
#show max_elf_2/2.
#show max_elf_3/2.
#show elf_three_best_sum/1.

