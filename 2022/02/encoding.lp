
% input: move(number, enemy, my)

% Lose
outcome(N, S) :- move(N, E, M), S = M, E = 1, M = 3.
outcome(N, S) :- move(N, E, M), S = M, E = 2, M = 1.
outcome(N, S) :- move(N, E, M), S = M, E = 3, M = 2.

% Win
outcome(N, S) :- move(N, E, M), S = M + 6, E = 3, M = 1.
outcome(N, S) :- move(N, E, M), S = M + 6, E = 1, M = 2.
outcome(N, S) :- move(N, E, M), S = M + 6, E = 2, M = 3.

% Draw
outcome(N, S) :- move(N, E, M), S = M + 3, E = M.

match_value(S) :- S = #sum{S2,N2 : outcome(N2, S2)}.

#show match_value/1.


% Lose
outcome_2(N, S) :- move(N, E, M), S = 3, E = 1, M = 1. % E picks rock, I need to pick siccors
outcome_2(N, S) :- move(N, E, M), S = 1, E = 2, M = 1. % E picks paper, I need to pick rock
outcome_2(N, S) :- move(N, E, M), S = 2, E = 3, M = 1. % E picks siccors, I need to pick paper

% Draw
outcome_2(N, S) :- move(N, E, M), S = E + 3, M = 2. % E picks rock, I need to pick rock

% Lose
outcome_2(N, S) :- move(N, E, M), S = 2 + 6, E = 1, M = 3. % E picks rock, I need to pick paper
outcome_2(N, S) :- move(N, E, M), S = 3 + 6, E = 2, M = 3. % E picks paper, I need to pick siccors
outcome_2(N, S) :- move(N, E, M), S = 1 + 6, E = 3, M = 3. % E picks siccors, I need to pick rock

match_value_2(S) :- S = #sum{S2,N2 : outcome_2(N2, S2)}.

#show match_value/1.
#show match_value_2/1.






