
% input: move(ID, COUNT, FROM, TO).
% input: position(STACK, POSITION, CRATE).

position_before(0,STACK, POSITION, CRATE) :- position(STACK, POSITION, CRATE).

position_before(ID + 1, STACK, POSITION, CRATE) :- position_before(ID, STACK, POSITION, CRATE), not affected(ID, STACK, POSITION, CRATE), max_id(ID2), ID <= ID2.
affected(ID, STACK, POSITION, CRATE) :- position_before(ID, STACK, POSITION, CRATE), move(ID, COUNT, FROM, TO), FROM = STACK.
affected(ID, STACK, POSITION, CRATE) :- position_before(ID, STACK, POSITION, CRATE), move(ID, COUNT, FROM, TO), TO = STACK.

position_before(ID + 1, TO, POSITION, CRATE) :- affected(ID, STACK, POSITION, CRATE), move(ID, COUNT, FROM, TO), FROM = STACK, POSITION <= COUNT.
position_before(ID + 1, STACK, POSITION + COUNT, CRATE) :- affected(ID, STACK, POSITION, CRATE), move(ID, COUNT, FROM, TO), TO = STACK.
position_before(ID + 1, STACK, POSITION - COUNT, CRATE) :- affected(ID, STACK, POSITION, CRATE), move(ID, COUNT, FROM, TO), FROM = STACK, POSITION > COUNT.


max_id(ID2) :- ID2 = #max{ID : move(ID, COUNT, FROM, TO)}.

max_position_id(ID2) :- ID2 = #max{ID : position_before(ID, STACK, POSITION, CRATE)}.
max_position(STACK,POSITION,CRATE) :- position_before(ID, STACK, POSITION, CRATE), max_position_id(ID).

max_top_positions(STACK, CRATE) :- max_position(STACK, 1, CRATE).

%#show move/4.
%#show affected/4.
%#show position_before/4.
%#show max_position/3.
#show max_top_positions/2.








