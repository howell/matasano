module FrequencyAnalysis
( score
) where

import qualified Data.Map as M
import Data.Char
import Control.Applicative

type Percent = Double
type LetterFrequencies = M.Map Char Percent

score :: String -> Double
score = dotProduct english . frequencies

countedCharacters :: String
countedCharacters = "abcdefghigklmnopqrstuvwxyz "

occurrences :: Char -> String -> Int
occurrences c = length . filter (==c)

frequency :: Char -> String -> Percent
frequency c s = fromIntegral (100 * occurrences c s) / fromIntegral (length s)

frequencies :: String -> LetterFrequencies
frequencies s = M.fromList $ zip countedCharacters freqs where
    freqs = frequency <$> countedCharacters <*> pure (downcase s)

downcase :: String -> String
downcase = map toLower

english :: LetterFrequencies
english = M.fromList $ zip countedCharacters freqs where
          freqs = [6.33, 1.39, 2.09, 3.39, 10.56, 1.84, 1.55, 5.13, 5.78, 0.14,
                   0.50, 3.27, 2.24, 5.74, 6.14, 1.29, 0.09, 4.97, 5.02, 7.14,
                   2.30, 0.87, 1.87, 0.13, 1.93, 0.14, 18.15]

--dotProduct :: LetterFrequencies -> LetterFrequencies -> Double
dotProduct :: (Num b, Ord k) => M.Map k b -> M.Map k b -> b
dotProduct = (M.fold (+) 0 .) . M.intersectionWith (*)

