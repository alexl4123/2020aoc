import System.Environment
import Data.List
import System.IO
import Data.Char

execute :: IO ()
execute = do
    content <- readFile "input2"
    let content_parsed = parse (lines content) 1
    writeFile "input2_parsed.lp" (foldl (\sum cur -> sum ++ cur ++ "\n") [] content_parsed)

parse :: [String] -> Int -> [String]
parse [] _ = []
parse (a:as) i = ["p(" ++ a ++ "," ++ show i ++ ")."] ++ parse as (i+1)
