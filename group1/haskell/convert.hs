module Convert
( readBase16
) where

import Data.Word
import Data.Bits
import Data.Char
import Data.List.Split
import Data.List
import Control.Monad

readBase16 :: String -> Maybe [Word8]
readBase16 s = sequence . map readOneBase16 $ chunksOf 2 s
--readBase16 = map readOneBase16 $ chunk 2

-- read one base 16 character
readOneBase16 :: [Char] -> Maybe Word8
readOneBase16 [] = Nothing
readOneBase16 [x] = base16Lookup x
readOneBase16 [x, y] = do
    a <- base16Lookup x
    b <- base16Lookup y
    return $ a `shiftL` 4 .|. b
readOneBase16 xs = Nothing

base16Lookup :: Char -> Maybe Word8
base16Lookup c = liftM fromIntegral $ toLower c `elemIndex` "0123456789abcdef"
-- These all work, no idea what the best way to do it is
--base16Lookup c = do
    --x <- elemIndex (toLower c) "0123456789abcdef"
    --return $ fromIntegral x
--base16Lookup c = maybe Nothing (return . fromIntegral) $ elemIndex (toLower
                    --c) "0123456789abcdef"
--base16Lookup c = toLower c `elemIndex` "0123456789abcdef" >>=
--                      return . fromIntegral

printBase16 :: [Word8] -> String
printBase16 = concat . map base16ReverseLookup

base16ReverseLookup :: Word8 -> String
base16ReverseLookup x = [(base16Alphabet !! msn), (base16Alphabet !! lsn)] where
                            base16Alphabet = "0123456789abcdef"
                            msn = fromIntegral $ x `shiftR` 4
                            lsn = fromIntegral $ x .&. 0x0f
base64Alphabet :: [Char]
base64Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

base64Lookup :: Char -> Maybe Word8
base64Lookup '=' = Just 0
base64Lookup c = liftM fromIntegral $ c `elemIndex` base64Alphabet

readBase64 :: String -> Maybe [Word8]
readBase64 s = do
    tuples <- sequence $ map readBase64Group $ tuplefy64 s
    return . concat $ map detuplefy64 tuples

tuplefy64 :: [Char] -> [(Char, Char, Char, Char)]
tuplefy64 [] = []
tuplefy64 (a:b:c:d:xs) = (a, b, c, d) : tuplefy64 xs

detuplefy64 :: (Word8, Word8, Word8) -> [Word8]
detuplefy64 (a, b, c) = [a, b, c]

readBase64Group :: (Char, Char, Char, Char) -> Maybe (Word8, Word8, Word8)
readBase64Group (a, b, c, d) = do
    e <- base64Lookup a
    f <- base64Lookup b
    g <- base64Lookup c
    h <- base64Lookup d
    let x = (e `shiftL` 2) .|. (f `shiftR` 4)
        y = (f `shiftL` 4) .|. (g `shiftR` 2)
        z = (g `shiftL` 6) .|. h
    return (x, y, z)

