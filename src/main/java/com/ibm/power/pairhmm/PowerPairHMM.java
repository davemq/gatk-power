// Header matter


// NOTE: THIS FILE IS GENERATED from an Org mode document. Edit the original
// document and re-tangle this file.

package com.ibm.power.pairhmm;

import Java.io.File;
import Java.net.URL;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.FileUtils;

// Class


/// Provides native PairHMM code accelerated for Power vector
/// instructions, via compiler optimizations.
public class PowerPairHMM {

// Implement load

//     The class will load the native library through a src_java{static} block.[fn:3]


    static {
	String systemLibraryName = System.mapLibraryName("vsx_pairhmm");
	String resourcePath = "native/" + systemLibraryName;
	URL inputUrl = PowerPairHMM.class.getResource(resourcePath);
	try {
	    File temp = File.createTempFile(FilenameUtils.getBaseName(resourcePath),
					    "." + FilenameUtils.getExtension(resourcePath));
	    FileUtils.copyURLToFile(inputUrl, temp);
	    temp.deleteOnExit();
	    System.load(temp.getAbsolutePath());
	} catch (Exception|Error e) {
    }

// Provide native subComputeReadLikelihoodGivenHaplotypeLog10Native


    public native double
	subComputeReadLikelihoodGivenHaplotypeLog10Native(int,
							  int,
							  int,
							  double [][],
							  double [][],
							  double [][],
							  double [][],
							  double [][]);
}
