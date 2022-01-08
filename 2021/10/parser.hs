import System.IO
import Data.Char

execute :: IO ()
execute = do
    let filename = "input2"
    content <- readFile filename
    let content_parsed = parse (lines content) 0
    writeFile (filename ++ "_parsed.lp") (foldl (\sum cur -> sum ++ (foldl (\sum2 cur2 -> sum2 ++ cur2) "" cur) ++ "\n") "" content_parsed)

parse :: [String] -> Int -> [[String]]
parse [] _ = []
parse (a:as) i = [parseLine a i 0] ++ parse as (i+1)

parseLine :: String -> Int -> Int -> [String]
parseLine [] _ _ = []
parseLine (a:as) y x = ["p(\"" ++ [a] ++ "\"," ++ show x ++ "," ++ show y ++ ")."] ++ parseLine as y (x+1) 

{- where
    value b
        | b == '('  = 3
        | b == ')'  = 6
        | b == '['  = 5
        | b == ']'  = 10
        | b == '{'  = 7
        | b == '}'  = 14
        | b == '<'  = 11
        | otherwise = 22
-}
