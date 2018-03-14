Random information:
can enter words up to 50 chars for p11
will show itself as an anagram "eg. 1804289383 would print out 1804289383 as one of its anagrams"

for 1.2 I decided to just do a single presort, just sorting the strings into signatures, and not sorting the array of signatures

1.2 ended up taking longer to execute than 1.1, but 1.2 has a longer initilization stage and would be much quicker for repeated 
finding of anagrams, as there is not carry over of resources from 1.1 each time you find a new anagram, whereas 1.2, once the sorting
is done, every anagram you want to find after that will be much quicker. Also 1.2 could be speed up with a hash table recording the sorted values
as keys and anagrams as a linked list value of the hash key, and would be a 0(1) access time for the anagrams once set up.

2.1-3 max pattern size - 255 chars

To Build:
type make in root folder

To Exceute:
type ./"name of question you want to run" eg. ./P11 for question 1.1

Analysis:
*Each anaylsis is run 1000 times to get an acutal execution time

Brute Force: (pattern shifts : runtime*1000 (ms))
"a" : 3296590 : 13000
"of" : 3296589 : 13000
"the" : 3296588 : 15000
"were" : 3296587 : 12000
"taken" : 3296586 : 14000
"beyond" : 3296585 : 12000
"details" : 3296584 : 13000
"disclose" : 3296583 : 13000
"including" : 3296582 : 14000
"individual" : 3296581 : 15000

Horspool: (pattern shifts : runtime*1000 (ms))
"a" : 3296591 : 28000
"of" : 1691152 : 14000
"the" : 1142016 : 12000
"were" : 901328 : 10000
"taken" : 735478 : 8000
"beyond" : 622570 : 7000
"details" : 571193 : 6000
"disclose" : 473226 : 6000
"including" : 434474 : 5000
"individual" : 385750 : 5000

** my boyer moore would always choose bad char skips, and therefore had the same pattern 
shifts as horspool, with a longer execution time due to building the good suffix tables **

Boyer-Moore: (pattern shifts : runtime*1000 (ms)) 
"a" : 3296591 : 29000
"of" : 1691152 : 15000
"the" : 1142016 : 13000
"were" : 901328 : 11000
"taken" : 735478 : 9000
"beyond" : 622570 : 8000
"details" : 571193 : 7000
"disclose" : 473226 : 7000
"including" : 434474 : 6000
"individual" : 385750 : 6000

Brute Force vs Horspool:
Pattern Shift ratios: 1 + 0.51 + 0.3464 + 0.2734 + 0.223 + 0.1888 + 0.173 + 0.1426 + 0.139 + 0.134 / 10 = 0.32692
Execution Time: 2.1 + 1.07 + .867 + .833 + .571 + .583 + .461 + .461 + .357 + .333 / 10 = .764

Therefore Horspool has on average 32% of the patterns shifts that brute force does, and runs on average in 76% of the time

Like I said in the note, my boyer moore algo, ended up just running the same as horspool, even though I am pretty sure I 
implemented it correctly, so I cant do the analysis for that.  



