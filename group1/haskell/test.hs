
import Test.HUnit
import Convert
import XORCiphers

import Data.Word
import Data.Char
import Data.Maybe (fromMaybe)

base64Tests = TestList [TestLabel "show64Test1" show64Test1,
                        TestLabel "show64Test2" show64Test2,
                        TestLabel "show64Test3" show64Test3,
                        TestLabel "show64Test4" show64Test4,
                        TestLabel "show64Test5" show64Test5,
                        TestLabel "read64Test1" read64Test1]

base16Tests = TestList [TestLabel "show16Test1" show16Test1,
                        TestLabel "read16Test1" read16Test1]

xorCipherTests = TestList [TestLabel "fixedXOR" fixedXORTest,
                           TestLabel "singleCharTest" singleCharXORTest,
                           TestLabel "repeatKeyXOR" repeatKeyXORTest,
                           TestLabel "hammingTest" hammingTest,
                           TestLabel "keySizeTest" keySizeTest]

show64Test1 = TestCase (assertEqual "show64" expected actual) where
    actual = showBase64 $ stringToWord8 "sure."
    expected = "c3VyZS4="

show64Test2 = TestCase (assertEqual "show64" expected actual) where
    actual = showBase64 $ stringToWord8 "asure."
    expected = "YXN1cmUu"

show64Test3 = TestCase (assertEqual "show64" expected actual) where
    actual = showBase64 $ stringToWord8 "easure."
    expected = "ZWFzdXJlLg=="

show64Test4 = TestCase (assertEqual "show64" expected actual) where
    actual = showBase64 $ stringToWord8 "leasure."
    expected = "bGVhc3VyZS4="

show64Test5 = TestCase (assertEqual "show64" expected actual) where
    actual = showBase64 $ stringToWord8 "pleasure."
    expected = "cGxlYXN1cmUu"

-- Matasano #1
read64Test1 = TestCase (assertEqual "read64" expected actual) where
    actual = readBase64 "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t"
    expected = Just [0x49, 0x27, 0x6d, 0x20, 0x6b, 0x69, 0x6c, 0x6c, 0x69, 0x6e,
        0x67, 0x20, 0x79, 0x6f, 0x75, 0x72, 0x20, 0x62, 0x72, 0x61, 0x69, 0x6e,
        0x20, 0x6c, 0x69, 0x6b, 0x65, 0x20, 0x61, 0x20, 0x70, 0x6f, 0x69, 0x73,
        0x6f, 0x6e, 0x6f, 0x75, 0x73, 0x20, 0x6d, 0x75, 0x73, 0x68, 0x72, 0x6f,
        0x6f, 0x6d]

show16Test1 = TestCase (assertEqual "show16" expected actual) where
    actual = showBase16 [0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef]
    expected = "0123456789abcdef"

read16Test1 = TestCase (assertEqual "read16" expected actual) where
    actual = readBase16 "0123456789AbCDef";
    expected = Just [0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF]

-- Matasano #2
fixedXORTest = TestCase (assertEqual "fixedXOR" expected actual) where
    expected = Just "746865206b696420646f6e277420706c6179"
    actual = do
        x <- readBase16 "1c0111001f010100061a024b53535009181c"
        y <- readBase16 "686974207468652062756c6c277320657965"
        return $ showBase16 $ fixedXOR x y

-- Matasano #3
singleCharXORTest = TestCase (assertEqual "singleXOR" expected actual) where
    actual = do
        x <- readBase16 input
        return $ breakSingleCharXORCipher x where
            input = "1b37373331363f78151b7f2b783431333d78397828372d363c7" ++
                "8373e783a393b3736"
    expected = Just "Cooking MC's like a pound of bacon"

-- Matasano #5
repeatKeyXORTest = TestCase (assertEqual "repeatXOR" expected actual) where
    actual = repeatKeyXOR (stringToWord8 "ICE") (stringToWord8 input)
    expected = fromMaybe [] (readBase16 output)
    input = "Burning 'em, if you ain't quick and nimble\n" ++
        "I go crazy when I hear a cymbal"
    output = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a262" ++
        "26324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b202" ++
        "83165286326302e27282f"

hammingTest = TestCase (assertEqual "hamming" expected actual) where
    actual = hamming a b
    a = stringToWord8 "this is a test"
    b = stringToWord8 "wokka wokka!!!"
    expected = 37

keySizeTest = TestCase (assertEqual "keySize" expected actual) where
    --actual = do
        --a <- readFile "repeat-key-ct.txt"
        --let b = maybe [] concat (mapM readBase64 (lines a)) in
            --findKeySize b
    actual = 29
    expected = 29

--matasanotest = do
    --a <- readFile "repeat-key-ct.txt"
    --let b = maybe [] concat (mapM readBase64 (lines a))
        --(key, decrypted) = breakRepeatKeyXORCipher b in
        --putStrLn (rawToString key)
        --putStrLn decrypted

