module XORCiphers
( fixedXOR
, repeatKeyXOR
, breakSingleCharXORCipher
, detectSingleCharXOR
, breakRepeatKeyXORCipher
, hamming
, findKeySize
) where

import Convert
import Data.Word
import Data.Bits
import FrequencyAnalysis
import Data.List (maximumBy, minimumBy, transpose)
import Data.List.Split (chunksOf)
import Data.Ord (comparing)
import Data.Char (chr, ord)
import Data.Maybe (fromMaybe)

fixedXOR :: Bits a => [a] -> [a] -> [a]
fixedXOR = zipWith xor

repeatKeyXOR :: Bits a => [a] -> [a] -> [a]
repeatKeyXOR k = fixedXOR (concat $ repeat k)

findKey :: [Word8] -> Word8
findKey xs = maxIndex $ map (score . rawToString . encrypt) [0..255] where
    encrypt k = repeatKeyXOR (return k) xs

breakSingleCharXORCipher :: [Word8] -> (Word8, String)
breakSingleCharXORCipher xs =
    let key = findKey xs
        plaintext =  rawToString $ repeatKeyXOR (return key) xs in
        (key, plaintext)

detectSingleCharXOR :: [[Word8]] -> ([Word8], (Word8, String))
detectSingleCharXOR cts = maximumBy (comparing $ score . snd . snd) pts where
    pts = zip cts $ map breakSingleCharXORCipher cts

breakRepeatKeyXORCipher :: [Word8] -> ([Word8], String)
breakRepeatKeyXORCipher xs = (key, decrypted) where
    key = map findKey (transpose keyblocks)
    keyblocks = chunksOf keysize xs
    keysize = findKeySize xs
    decrypted = rawToString $ repeatKeyXOR key xs

maxIndex :: (Enum b, Num b, Ord a) => [a] -> b
maxIndex xs = snd $ maximumBy (comparing fst) (zip xs [0..])

minIndex :: Ord a => [a] -> Int
minIndex xs = snd $ minimumBy (comparing fst) (zip xs [0..])

hamming :: (Num a, Bits a, Num b) => [a] -> [a] -> b
hamming a b = sum $ zipWith hammingDistance a b

hammingDistance :: (Num a, Bits a, Num b) => a -> a -> b
hammingDistance 0 0 = 0
hammingDistance x y = if (x .&. 1) /= (y .&. 1)
                      then 1 + hammingDistance (x `shiftR` 1) (y `shiftR` 1)
                      else hammingDistance (x `shiftR` 1) (y `shiftR` 1)

findKeySize :: [Word8] -> Int
findKeySize xs = 2 + minIndex (map (keySizeLikeliness xs) [2..40])

keySizeLikeliness :: (Bits a, Num a) => [a] -> Int -> Double
keySizeLikeliness xs k = hammingPairs (take 12 blocks) / fromIntegral k where
    blocks = chunksOf k xs

hammingPairs :: (Num a, Bits a, Num b) => [[a]] -> b
hammingPairs [] = 0
hammingPairs (_:[]) = 0
hammingPairs (x:y:zs) = hamming x y + hammingPairs zs

