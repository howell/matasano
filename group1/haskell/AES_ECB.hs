module AES_ECB
( ecbDecryptAES128
) where

-- cabal install cryptocipher
import Crypto.Cipher (AES128, cipherInit, makeKey, ecbDecrypt)
import Data.ByteString (ByteString)
import qualified Data.ByteString as B
import Convert (readBase64, stringToWord8, rawToString)
import Data.Word (Word8)
import Data.Maybe (fromMaybe)

initAES128 :: ByteString -> AES128
initAES128 = either (error . show) cipherInit . makeKey

ecbDecryptAES128 :: [Word8] -> [Word8] -> [Word8]
ecbDecryptAES128 key ct = B.unpack $ ecbDecrypt cipher cipherText where
    cipher = initAES128 (B.pack key)
    cipherText = B.pack ct

runDecrypt = do
    a <- readFile "aes_ecb_enc64.txt"
    let input = fromMaybe [] $ (readBase64 . concat . lines) a
        key = stringToWord8 "YELLOW SUBMARINE" in
        putStrLn . rawToString $ ecbDecryptAES128 key input
