module FrequencyAnalysis
(
) where

import Data.List
import Data.Char

characterCounts s = group . sort $ filter isCounted $ map toLower s where
    isCounted c = c `elem` countedCharacters

countedCharacters :: String
countedCharacters = "abcdefghigklmnopqrstuvwxyz "
