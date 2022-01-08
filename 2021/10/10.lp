#show result/1.

#script (lua)

clingo = require("clingo")
N = clingo.Number

function apply(a,b)
    if isOpeningBracket(a) then
        return b.string .. a.string
    else
        return b.string:sub(1,-2)
    end
end

function isOpeningBracket(a)
    if (a.string == "(" or a.string == "[" or a.string == "{" or a.string == "<") then
        return true
    else
        return false
    end
end

function isError(a,b)
    if isOpeningBracket(a) then
        return 0
    else
        last = string.sub(b.string, -1, -1)
        if ((a.string == ")" and last == "(") or
            (a.string == "]" and last == "[") or
            (a.string == "}" and last == "{") or
            (a.string == ">" and last == "<")) then
            return 0
        else
            return 1
        end
    end
end

function getVal(a)
    if a.string == ")" then
        return 3
    elseif a.string == "]" then
        return 57
    elseif a.string == "}" then
        return 1197
    elseif a.string == ">" then
        return 25137
    else
        return 0
    end
end

#end.

o(T,X,Y) :- p(T,X,Y), X = 0.
o(@apply(T,S1),X,Y) :- p(T,X,Y), o(S1,X1,Y), X1 + 1 = X.

f(T,X,Y) :- p(T,X,Y), o(S1,X1,Y), X1 + 1 = X, @isError(T,S1) = 1.

row(Y) :- p(T,X,Y).
minF(X2,Y) :- row(Y), X2 = #min{X : f(T,X,Y)}, X2 > 0, X2 < #sup.

ff(@getVal(T),T,X,Y) :- f(T,X,Y), minF(X,Y).
result(S) :- S = #sum{SS,T,X,Y : ff(SS,T,X,Y)}.








