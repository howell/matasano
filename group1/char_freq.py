#!/usr/local/bin/python

import glob
import os
import string

def add_string_char_freqs(s, char_freqs):
  """Count the number of occurences of each letter, case-insensitive, in a
  string and add them to an existing dictionary of type character => number
  of occurences"""
  s = s.lower()
  for char in s:
#    if char in string.printable and char not in string.whitespace:
    if char in string.letters:
      if char in char_freqs:
        char_freqs[char] += 1
      else:
        char_freqs[char] = 1
    elif char == ' ':
      if ' ' in char_freqs:
        char_freqs[' '] += 1
      else:
        char_freqs[' '] = 1

def add_file_char_freqs(file_name, char_freqs):
  """Count the number of occurences of each character in a file and add them
  to an existing dictionary of of type character => number of occurences"""
  with open(file_name, 'r') as f:
    for line in f:
      add_string_char_freqs(line, char_freqs)

def output_char_freqs(char_freqs):
  """Open an output file and write the found character frequencies"""
  total_characters = sum(char_freqs.values())
  with open("char_freqs.txt", 'w') as f:
    for c in sorted(char_freqs.iterkeys()):
      freq = (char_freqs[c] * 100.0) / total_characters
      f.write("{} = {:.02f}\n".format(c, freq))
  return

def main():
  os.chdir("./SampleText")
  char_freqs = { }
  for f in glob.glob("*"):
    add_file_char_freqs(f, char_freqs)
  os.chdir("..")
  output_char_freqs(char_freqs)


if __name__ == "__main__":
  main()
