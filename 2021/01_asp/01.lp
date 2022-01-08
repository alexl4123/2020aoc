% Executed with ''clingo'' from Potassco
#show result/1.

o(X1,T1,X2,T2) :- p(X1,T1), p(X2,T2), X1 != X2, T1 != T2, T1 +1 = T2, X2 > X1.

result(X) :- X = #count{X1,T1,X2,T2 : o(X1,T1,X2,T2)}.



