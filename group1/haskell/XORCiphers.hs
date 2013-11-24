module XORCiphers
( fixedXOR
, breakSingleCharXORCipher
) where

import Convert
import Data.Word
import Data.Bits
import FrequencyAnalysis
import Data.List (maximumBy)
import Data.Ord (comparing)
import Data.Char (chr)

fixedXOR :: Bits a => [a] -> [a] -> [a]
fixedXOR = zipWith xor

repeatKeyXOR :: Bits a => [a] -> [a] -> [a]
repeatKeyXOR k = fixedXOR (concat $ repeat k)

findKey :: [Word8] -> Word8
findKey xs = maxIndex $ map (score . rawToString . encrypt) [0..255] where
    encrypt k = repeatKeyXOR (return k) xs

breakSingleCharXORCipher :: [Word8] -> String
breakSingleCharXORCipher xs = rawToString $ repeatKeyXOR (return (findKey xs)) xs

maxIndex :: (Enum b, Num b, Ord a) => [a] -> b
maxIndex xs = snd $ maximumBy (comparing fst) (zip xs [0..])

rawToString :: [Word8] -> String
rawToString = map (chr . fromIntegral)
