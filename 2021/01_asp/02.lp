% Executed with ''clingo'' form Potassco
#show result/1.

s(S,T1,T2,T3) :- p(X1,T1), p(X2,T2), p(X3,T3), T1 + 1 = T2, T2 + 1 = T3, S = X1 + X2 + X3.

o(S1,T11,T12,T13) :- s(S1,T11,T12,T13), s(S2,T21,T22,T23), S2 > S1, T11 + 1 = T21.

result(S1) :- S1 = #count{S,T1,T2,T3 : o(S,T1,T2,T3)}.


