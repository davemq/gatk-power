// Header matter


// NOTE: THIS FILE IS GENERATED from an Org mode document. Edit the original
// document and re-tangle this file.

package com.ibm.power.pairhmm;

// Class


/// Provides native PairHMM code accelerated for Power vector
/// instructions, via compiler optimizations.
public class PowerPairHMM {

// Implement load

//     The class will load the native library through a src_java{static} block.[fn:3]


    static {
	System.loadLibrary("vsx_pairhmm");
    }

// Provide native subComputeReadLikelihoodGivenHaplotypeLog10Native


    public native double
	subComputeReadLikelihoodGivenHaplotypeLog10Native(int paddedReadLength,
							  int hapStartIndex,
							  int paddedHaplotypeLength,
							  double [][] matchMatrix,
							  double [][] prior,
							  double [][] transition,
							  double [][] insertion,
							  double [][] deletion);
}
