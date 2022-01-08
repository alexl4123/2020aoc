#show result/1.
#show result2/4.

#script (python)

from clingo.symbol import Number
from clingo.symbol import String

class Context:
    def inc(self, x):
        return Number(x.number)

    def seq(self, x, y):
        return [x, y]

    def apply(self,a,b):
        if (a.string == "(" or a.string == "[" or a.string == "{" or a.string == "<"):
            return String(b.string + a.string)
        else:
            return String(b.string[:-1])

    def isError(self,a,b):
        if (a.string == "(" or a.string == "[" or a.string == "{" or a.string == "<"):
            return Number(0)
        else:
            last = b.string[-1]
            if ((a.string == ")" and last == "(") or
                (a.string == "]" and last == "[") or
                (a.string == "}" and last == "{") or
                (a.string == ">" and last == "<")):
                return Number(0)
            else:
                return Number(1)

    def getVal(self,a):
        if a.string == ")":
            return Number(3)
        elif a.string == "]":
            return Number(57)
        elif a.string == "}":
            return Number(1197)
        elif a.string == ">":
            return Number(25137)
        else:
            return Number(0)

    def apply2(self,a,b):
        if (not b.string):
            return Number(a.number)

        last = b.string[-1]
        multiplied = 5 * a.number

        if last == "(":
            return Number(multiplied + 1)
        elif last == "[":
            return Number(multiplied + 2)
        elif last == "{":
            return Number(multiplied + 3)
        elif last == "<":
            return Number(multiplied + 4)
        else:
            return Number(a.number)

    def apply22(self,b):
        if (not b.string):
            return String(b.string)
        else:
            return String(b.string[:-1])

    def isempty01(self,s):
        if (not s.string):
            return Number(1)
        else:
            return Number(0)

    def middle(self,c):
        return Number(c.number / 2)

def main(prg):
    prg.ground([("base", [])], context=Context())
    prg.solve()

#end.

o(T,X,Y) :- p(T,X,Y), X = 0.
o(@apply(T,S1),X,Y) :- p(T,X,Y), o(S1,X1,Y), X1 + 1 = X.

f(T,X,Y) :- p(T,X,Y), o(S1,X1,Y), X1 + 1 = X, @isError(T,S1) = 1.

row(Y) :- p(T,X,Y).
minF(X2,Y) :- row(Y), X2 = #min{X : f(T,X,Y)}, X2 > 0, X2 < #sup.

ff(@getVal(T),T,X,Y) :- f(T,X,Y), minF(X,Y).
result(S) :- S = #sum{SS,T,X,Y : ff(SS,T,X,Y)}.

% Task 2:

notValidRow(Y) :- row(Y), ff(S,T,X,Y).
validRow(Y) :- row(Y), not notValidRow(Y).

maxPosInRow(X2, Y) :- validRow(Y), X2 = #max{X : p(T,X,Y)}.

oo(0,S,X,Y) :- maxPosInRow(X,Y), o(S,X,Y).
oo(@apply2(C1,S1),@apply22(S1),X,Y) :- oo(C1,S1,X1,Y), X1 + 1 = X, @isempty01(S1) = 0.

maxCor(X2,Y) :- validRow(Y), X2 = #max{X : oo(C,S,X,Y)}.
ooo(C,S,X,Y) :- oo(C,S,X,Y), maxCor(X,Y).

sorted(C,X,Y,I) :- ooo(C,S,X,Y), I = #count{C2,S2,X2,Y2 : ooo(C2,S2,X2,Y2), C2 <= C, Y2 != Y, C2 > 0}, C > 0.
maxSorted(C2 - 1) :- C2 = #count{C,S,X,Y : ooo(C,S,X,Y)}.
result2(C,X,Y,I) :- sorted(C,X,Y,I), maxSorted(I2), I = @middle(I2).
result22(S,X,Y) :- maxPosInRow(X,Y), result2(C,X2,Y,I), o(S,X,Y).



