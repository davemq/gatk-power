// Header matter


// NOTE: THIS FILE IS GENERATED from an Org mode document. Edit the original
// document and re-tangle this file.

package com.ibm.power.pairhmm;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.FileUtils;

// Class


/// Provides native PairHMM code accelerated for Power vector
/// instructions, via compiler optimizations.
public class PowerPairHMM {

// Implement load

//     The class will load the native library through a src_java{static} block.[fn:3]


    static {
	final String systemLibraryName = System.mapLibraryName("vsx_pairhmm");
	System.loadLibrary(systemLibraryName);
    }

// Provide native subComputeNative


    public native double
	subComputeNative(int paddedReadLength,
			 int hapStartIndex,
			 int paddedHaplotypeLength,
			 double [][] matchMatrix,
			 double [][] prior,
			 double [][] transition,
			 double [][] insertion,
			 double [][] deletion);
}
