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

-- seperate the most significant and least significant nibbles of a character
--getNibbles :: Char -> (Word8, Word8)
--getNibbles c = (raw `shiftR` 4, raw .&. 4) where
                --raw = fromIntegral . ord . toLower $ c

--combineNibbles :: (Word8, Word8) -> Word8
--combineNibbles (x, y) = x `shiftL` 4 .|. y


