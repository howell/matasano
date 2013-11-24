module XORCiphers
(
) where

import Data.Bits
import FrequencyAnalysis

fixedXOR :: Bits a => [a] -> [a] -> [a]
fixedXOR = zipWith xor

repeatKeyXOR :: Bits a => [a] -> [a] -> [a]
repeatKeyXOR k = fixedXOR (concat $ repeat k)

--findKey :: [Word8] -> Word8
--findKey xs = maxIndex map (\x -> repeatKeyXOR (return x) xs) [0..255]

breakSingleCharXORCipher :: [Word8] -> String
breakSingleCharXORCipher xs = ""

maxIndex xs = maximumBy (comparing fst) (zip xs [0..])
