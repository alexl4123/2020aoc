
% input: priority(ELEM,VALUE).
% input: item(RUCKSACK_ID, COMPARTEMENT, ELEM)

wrong_elem(ID, ELEM) :- item(ID, C, G, ELEM), item(ID, C2, G, ELEM), C != C2.

output(S) :- S = #sum{S2,ID : wrong_elem(ID,ELEM), priority(ELEM,S2)}.

group_item(G, ELEM) :- item(ID1, C1, G, ELEM), item(ID2, C2, G, ELEM), item(ID3, C3, G, ELEM), ID1 != ID2, ID1 != ID3, ID2 != ID3.

output_2(S) :- S = #sum{S2,ID : group_item(ID,ELEM), priority(ELEM,S2)}.



#show group_item/2.
#show output/1.
#show output_2/1.
