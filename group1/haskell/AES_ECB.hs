module AES_ECB
( ecbDecryptAES128
, findEcbEnc
) where

-- cabal install cryptocipher
import Crypto.Cipher (AES128, cipherInit, makeKey, ecbDecrypt)
import Data.ByteString (ByteString)
import qualified Data.ByteString as B
import Convert (readBase64, stringToWord8, rawToString)
import Data.Word (Word8)
import Data.Maybe (fromMaybe)
import Data.List (nub)
import Data.List.Split (chunksOf)

initAES128 :: ByteString -> AES128
initAES128 = either (error . show) cipherInit . makeKey

ecbDecryptAES128 :: [Word8] -> [Word8] -> [Word8]
ecbDecryptAES128 key ct = B.unpack $ ecbDecrypt cipher cipherText where
    cipher = initAES128 (B.pack key)
    cipherText = B.pack ct

findEcbEnc :: [[Word8]] -> [[Word8]]
findEcbEnc = filter isEcbEnc

isEcbEnc :: [Word8] -> Bool
isEcbEnc xs = length (nub blocks) /= length blocks where
    blocks = chunksOf 16 xs
