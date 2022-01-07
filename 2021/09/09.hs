import System.IO
import Data.Char

-- Preliminary definitions
fst' :: (a,b,c) -> a
fst' (a,b,c) = a

snd' :: (a,b,c) -> b
snd' (a,b,c) = b

thd' :: (a,b,c) -> c
thd' (a,b,c) = c 

-- ''Main'' program
execute :: IO ()
execute = do 
        content <- readFile "input3"
        let inMatrix = map (\l -> map (\c -> digitToInt c) l) (lines content) :: Data
        let found = calcValues ((length inMatrix) - 1) 0 ((length (inMatrix!!0)) - 1) 0 inMatrix :: Result1
        let res = sum $ map (\a -> fst' a + 1) found
        putStrLn $ "The result for the first task is: " ++ show res

        let foundBasins = calcBasinSizes found ((length inMatrix) - 1) ((length (inMatrix!!0)) - 1) inMatrix
        let lengthsBasins = map (\a -> length a) foundBasins
        let foundBasinsSet = map (\a -> toSet a) foundBasins
        let lengthsBasinsSet = quickSort' $ map (\a -> length a) foundBasinsSet
        let biggestThree = take 3 lengthsBasinsSet
        let result2 = biggestThree!!0 * biggestThree!!1 * biggestThree!!2
        -- putStrLn ("Found lowest points: " ++ show found)
        putStrLn ("Biggest Three: " ++ show biggestThree ++ ",value: " ++ show result2)

-- EX 1
type Data = [[Int]]
type Row = Int
type Column = Int
type MaxRow = Int
type MaxColumn = Int
type Result1 = [(Int,Row,Column)]

-- Recursion
calcValues :: MaxRow -> Row -> MaxColumn -> Column -> Data -> Result1
calcValues mr r mc c d
  | mr == r && mc == c  = checkIfLowpoint mr r mc c d
  | mc == c             = checkIfLowpoint mr r mc c d ++ calcValues mr (r+1) mc 0 d
  | otherwise           = checkIfLowpoint mr r mc c d ++ calcValues mr r mc (c+1) d

-- Casually checking for low points
checkIfLowpoint :: MaxRow -> Row -> MaxColumn -> Column -> Data -> Result1
checkIfLowpoint mr r mc c d
  | 0 == r && 0 == c   = if (curVal < rightVal && curVal < lowerVal) then [(curVal,r,c)] else [] 
  | 0 == r && mc == c  = if (curVal < leftVal && curVal < lowerVal) then [(curVal,r,c)] else [] 
  | mr == r && 0 == c  = if (curVal < rightVal && curVal < upperVal) then [(curVal,r,c)] else [] 
  | mr == r && mc == c = if (curVal < leftVal && curVal < upperVal) then [(curVal,r,c)] else [] 
  | mc == c = if (curVal < leftVal && curVal < upperVal && curVal < lowerVal) then [(curVal,r,c)] else [] 
  | 0 == c = if (curVal < rightVal && curVal < upperVal && curVal < lowerVal) then [(curVal,r,c)] else []
  | mr == r = if (curVal < leftVal && curVal < rightVal && curVal < upperVal) then [(curVal,r,c)] else []
  | 0 == r = if (curVal < leftVal && curVal < rightVal && curVal < lowerVal) then [(curVal,r,c)] else []
  | otherwise = if (curVal < leftVal && curVal < rightVal && curVal < upperVal && curVal < lowerVal) then [(curVal,r,c)] else [] where
                            curVal = d!!r!!c
                            leftVal = d!!r!!(c-1)
                            rightVal = d!!r!!(c+1)
                            lowerVal = d!!(r+1)!!c
                            upperVal = d!!(r-1)!!c

-- EX 2
type Result2 = [Result1]
calcBasinSizes :: Result1 -> MaxRow -> MaxColumn -> Data -> Result2
calcBasinSizes [] mr mc d = []
calcBasinSizes (a:as) mr mc d = [[a] ++ basinSize a mr mc d] ++ calcBasinSizes as mr mc d

basinSize :: (Int,Row,Column) -> MaxRow -> MaxColumn -> Data -> Result1
basinSize (val,r,c) mr mc d
  | 0 == r && 0 == c = checkRightTile ++ checkLowerTile
  | 0 == r && mc == c = checkLeftTile ++ checkLowerTile
  | mr == r && 0 == c = checkRightTile ++ checkUpperTile
  | mr == r && mc == c = checkLeftTile ++ checkUpperTile
  | mc == c = checkLeftTile ++ checkUpperTile ++ checkLowerTile
  | 0 == c = checkRightTile ++ checkUpperTile ++ checkLowerTile
  | 0 == r = checkLeftTile ++ checkRightTile ++ checkLowerTile
  | mr == r = checkLeftTile ++ checkRightTile ++ checkUpperTile
  | otherwise = checkLeftTile ++ checkRightTile ++ checkUpperTile ++ checkLowerTile where
    
    checkLeftTile = check (val,r,c) r mr (c-1) mc d
    checkRightTile = check (val,r,c) r mr (c+1) mc d
    checkLowerTile = check (val,r,c) (r+1) mr c mc d
    checkUpperTile = check (val,r,c) (r-1) mr c mc d

check :: (Int,Row,Column) -> Row -> MaxRow -> Column -> MaxColumn -> Data -> Result1
check (val,r,c) r2 mr d2 mc d
  | d!!r2!!d2 >= 9 = []
  | val >= d!!r2!!d2 = []
  | otherwise = [(d!!r2!!d2,r2,d2)] ++ basinSize (d!!r2!!d2,r2,d2) mr mc d

-- Converts an array to a set 
-- Needs toSet and contains
toSet :: (Eq a) => [a] -> [a]
toSet [] = []
toSet (a:as)
  | contains a as = toSet as
  | otherwise = [a] ++ toSet as

contains :: (Eq a) => a -> [a] -> Bool
contains a [] = False
contains a (b:bs)
  | a == b = True
  | otherwise = contains a bs

quickSort' :: [Int] -> [Int]
quickSort' [] = []
quickSort' (a:as) = (quickSort' lower) ++ [a] ++ (quickSort' higher) where
    lower = [x | x <- as, x > a]
    higher = [x | x <- as, x <= a]


