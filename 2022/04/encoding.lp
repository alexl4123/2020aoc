
% input: assignments(INDEX, FIRST_FIRST, FIRST_SECOND, SECOND_FIRST, SECOND_SECOND).

overlapping_1(ID) :- assignments(ID, FF, FS, SF, SS), FF >= SF, FS <= SS.
overlapping_1(ID) :- assignments(ID, FF, FS, SF, SS), SF >= FF, SS <= FS.

number_of_overlaps_1(S) :- S = #count{ID: overlapping_1(ID)}.

overlapping_2(ID) :- overlapping_1(ID).
overlapping_2(ID) :- assignments(ID, FF, FS, SF, SS), FS >= SF, FF <= SF.
overlapping_2(ID) :- assignments(ID, FF, FS, SF, SS), FF <= SS, FS >= SS.

number_of_overlaps_2(S) :- S = #count{ID: overlapping_2(ID)}.

#show number_of_overlaps_1/1.
#show number_of_overlaps_2/1.




