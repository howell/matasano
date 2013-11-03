module XORCiphers
(
) where

import Data.Bits

fixedXOR :: Bits a => [a] -> [a] -> [a]
fixedXOR = zipWith xor

