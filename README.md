# Asymmetric-Numeral-Systems

public-domain implementation of entropy coder from ANS family. 

## Introduction   

The modern data compression is mainly based on two approaches to entropy coding: Huffman and arithmetic/range coding. The former is much faster, but approximates probabilities with powers of 2, usually leading to relatively low compression rates. The arithmetic coding uses nearly exact probabilities which easily approaching theoretical compression rate limit (Shannon entropy), but at cost of much larger computational cost. 

Asymmetric numeral systems (ANS) is a new approach to accurate entropy coding, which allows to end this tradeoff between speed and rate. This advantage is due to being simpler than AC: using single natural number as the state, instead of two to represent a range. Beside simplifying renormalization, it allows to put the entire behavior for given probability distribution into a relatively small table: defining entropy coding automaton. The memory cost of such table for 256 size alphabet is a few kilobytes. There is a large freedom while choosing a specific table - using pseudorandom number generator initialized with cryptographic key for this purpose allows to simultaneously encrypt the data.  

## Table of Contents   

* 
* 



