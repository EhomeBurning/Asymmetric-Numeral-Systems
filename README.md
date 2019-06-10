# Asymmetric-Numeral-Systems

public-domain implementation of entropy coder from ANS family. 

## Introduction   

The modern data compression is mainly based on two approaches to entropy coding: Huffman and arithmetic/range coding. The former is much faster, but approximates probabilities with powers of 2, usually leading to relatively low compression rates. The arithmetic coding uses nearly exact probabilities which easily approaching theoretical compression rate limit (Shannon entropy), but at cost of much larger computational cost. 

Asymmetric numeral systems (ANS) is a new approach to accurate entropy coding, which allows to end this tradeoff between speed and rate. This advantage is due to being simpler than AC: using single natural number as the state, instead of two to represent a range. Beside simplifying renormalization, it allows to put the entire behavior for given probability distribution into a relatively small table: defining entropy coding automaton. The memory cost of such table for 256 size alphabet is a few kilobytes. There is a large freedom while choosing a specific table - using pseudorandom number generator initialized with cryptographic key for this purpose allows to simultaneously encrypt the data.  

## Table of Contents   

*  [1. Introduction](#Introduction)
*  [2. Methods](#Methods)
*  [3. Results](#Results)
*  [4. Discussion](#Discution)
*  [5. Future Work](#Future-Work)
*  [References](#References)




## 1 Introduction

In this Project, our group focused on the recent work by Duda [2][3], where a novel approach
implementing recent asymmetric numeral system(ANS) [2], [3] based on the arithmetic/range
coding(AC)[10], [9]. We implemented the proposed method and tested it on a numerical data
set.
In order to fully understand ANS, I think the best way is to know what is Arithmetic
coding. First of all, Arithmetic coding takes a message (often a file) composed of symbols
(nearly always eight-bit characters), and converts it to a floating point number greater than


```
or equal to zero and less than one. This floating point number can be quite long - effectively
your entire output file is one long number - which means it is not a normal data type that
you are used to using in conventional programming languages. My implementation of the
algorithm will have to create this floating point number from scratch, bit by bit, and likewise
read it in and decode it bit by bit.
This encoding process is done incrementally. As each character in a file is encoded, a
few bits will be added to the encoded message, so it is built up over time as the algorithm
proceeds.
The second thing to understand about arithmetic coding is that it relies on a model to
characterize the symbols it is processing. The job of the model is to tell the encoder what the
probability of a character is in a given message. If the model gives an accurate probability of
the characters in the message, they will be encoded very close to optimally.
Let us look at both approaches. For arithmetic coding, knowing that we are currently
in given half of the range is worth 1 bit of information, the current content is size of range
of range bits of information. For ANS, observingxas containinglg(x)bits of information,
informational content should increase tolg(x) +lg(1/p) =lg(x/p)bits while adding symbol
of probabilityp. So we need to ensurex→≈≈x/ptransition relation. Intuitively, rule:
xbecomesx-th appearance ofs-th subset fulfills this relation if this subset is uniformly
distributed with Pr(s) density.
```
```
The figure above shows two ways of binary numeral system. Having some information
stored in a natural numberx,to attach information from 0/ 1 symbols,we can add it in
the most significant position(x′=x+s 2 m),whereschooses between ranges, or in the least
significant(x′= 2x+s)position, whereschooses even and odd numbers.
```

## 2 Methods

```
Let’s denote two functions now: The coding function C and the decoding function D.
```
```
C(s,x) :=Mbx/Fsc+Bs+ (xmodFs)
D(x) := (s,Fs|x/Mc+ (xmodM)−Bs)wheres=s(xmodM)
D determines the encoded symbol s by looking at(xmodM)and looking up the corre-
sponding value in our cumulative frequency table (s is the unique s such thatBs≤xmodM <
Bs+Fs)
Denotes a "normalized" intervalI:={L,L+ 1,...,bL− 1 }= [L:bL), where the [L.bL)
thing on the right is the notation I’ll be using for a half-open interval of integers).bis the
radix of our encoder;b= 2means we’re emitting bits at a time,b= 256means we’re emitting
bytes, and so forth. Here is a general idea of how decoder works in code:
while (! done ) {
// Loop invariant : x i s normalized.
```

```
a s s e r t (L <= x && x < b∗L ) ;
```
```
// Decode a symbol
s , x = D(x ) ;
```
```
// keep reading more b i t s ( nibbles , bytes ,... )
while (x < L)
x = x∗b + readFromStream ( ) ;
}
Likewise, we want our encoder to be the inverse of our decoder. That means we have to
do the inverse of the operations the decoder does, in the opposite order. Which means our
encoder has to look something like this:
while (! done ) {
// Inverse renormalization : emit b i t s / bytes etc.
while ( emit b i t s ) {
writeToStream (x % b ) ;
x /= b ;
}
```
```
// Encode a symbol
x = C( s , x ) ;
```
```
// Loop invariant : x i s normalized
a s s e r t (L <= x && x < b∗L ) ;
}
The decoder reads bits whenever the normalization variant is violated after decoding a
symbol, to make sure it holds for the next iteration of the loop. The encoder, again, needs to
do the opposite – we need to proactively emit bits before coding s to make sure that, after we
have applied C, x will be normalized.
```
## 3 Results

### 3.1 Data

We use a data sample set which concludes 2GB numbers and test five times for encoding and
decoding. And then we use the mean speed value to evaluate the algorithm performance.


### 3.2 Conclusion

```
Here are the basic analysis of the results:
```
## 4 Discussion

### 4.1 Advantages

Its multiple ways of use give alternatives for different variants of AC. However, there are some
advantages thanks to this simplicity:

- instead of complex AC renormalization procedure, we can just transfer bit blocks of
known size (once per symbol).
- the huge freedom while choosing the exact encoder and chaotic state behavior makes it
also perfect to simultaneously encrypt the data or as an inexpensive nonlinear building
block of cryptosytems.


### 4.2 Disadvantages

While maintaining the state space (I), we can freely change between different probability
distributions, alphabets or even ABS/ANS variants. For example for complex tasks like video
compression, let say the used first 32 coding tables can be for quantized different probabilities
for binary alphabet, while succeeding tables would be for static probability distributions on
larger alphabets for various specific contexts, like for quantized DCT coefficients. The final
memory cost would be a few kilobytes times the number of tables/contexts.

- requires building and storing coding tables: about 1-16kB for 256 size alphabet
- the decoding is in opposite direction to encoding, what requires storing the final state
and may be an inconvenience, especially while adaptive applications.

## 5 Future Work

We have not got the chance to implement Huffman coding and Arithmetic Coding and compare
with this rANS method. In the future, we are going to optimize this method and run the
same data with original Huffman and Arithmetic Coding.


## References

```
[1] Y. Collet, Finite State Entropy, https://github.com/Cyan4973/FiniteStateEntropy.
[2] J. Duda, Optimal encoding on discrete lattice with translational invariant constrains
using statistical algorithms, arXiv:0710.3861.
[3] J. Duda, Asymmetric numerical systems, arXiv:0902.0271.
[4] J. Duda, Data Compression Using Asymmetric Numeral Systems
http://demonstrations.wolfram.com/author.html?author=Jarek+Duda.
[5] P. G. Howard, J. S. Vitter, Practical Implementations of Arithmetic Coding, Image
and Text Compression, 1992, 85-112.
[6] D.A. Huffman, A Method for the Construction of Minimum-Redundancy Codes,
Pro_x0002_ceedings of the I.R.E., September 1952, pp 1098-1102.
[7] M. Mahoney, Data Compression Programs website, http://mattmahoney.net/dc/.
[8] D. Marpe, H. Schwarz, T. Wiegand, Context-Based Adaptive Binary Arithmetic
Cod_x0002_ing in the H.264/AVC Video Compression Standard, IEEE Transactions on
Circuits and Systems for Video Technology, Vol. 13, No. 7, pp. 620-636, July 2003.
[9] G.N.N. Martin, Range encoding: an algorithm for removing redundancy from a
digi_x0002_tized message, Video and Data Recording Conf., Southampton, England,
July 1979.
[10] J.J. Rissanen, Generalized Kraft inequality and arithmetic coding, IBM J. Res.
De_x0002_velop., vol. 20, no. 3, pp. 198-203, May 1976.
[11] W. Szpankowski, Asymptotic Average Redundancy of Huffman (and Other) Block
Codes, IEEE Trans. Information Theory, 46 (7) (2000) 2434-2443.
```





