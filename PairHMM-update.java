// Updates to existing GATK Java code

// Java class
// src_java{org.broadinstitute.hellbender.utils.pairhmm.PairHMM} has
// src_java{enum Implementation} with each element being an available
// implementation of PairHMM. We need to update this to include a new
// element src_java{VSX_LOGLESS_CACHING}, and to try to use this element
// in the element src_java{FASTEST_AVAILABLE}. We'll also remove the
// elements src_java{AVX_LOGLESS_CACHING_OMP} and
// src_java{AVX_LOGLESS_CACHING}, as these are not available on Power.

//         Here's a fragment of code to replace the AVX elements:


	// VSX implementation of LOGLESS_CACHING through JNI. */
	VSX_LOGLESS_CACHING(args -> {
		final VSXLoglessPairHMM hmm = new VSXLoglessPairHMM();
		logger.info("Using the VSX native PairHMM implementation");
		return hmm;
	    }),



        // The src_java{FASTEST_AVAILABLE} element will be replaced with this:


        /* Uses the fastest available PairHMM implementation supported on the platform.
           Order of precedence:
            1. VSX_LOGLESS_CACHING
            2. LOGLESS_CACHING
         */
        FASTEST_AVAILABLE(args -> {
            try {
		final VSXLoglessPairHMM hmm = new VSXLoglessPairHMM();
		logger.info("Using the VSX native PairHMM implementation");
		return hmm;
            }
            catch ( UserException.HardwareFeatureException e ) {
                logger.warn("***WARNING: Machine does not have the VSX instruction set support needed for the accelerated VSX PairHmm. " +
                            "Falling back to the MUCH slower LOGLESS_CACHING implementation!");
                return new LoglessPairHMM();
            }
        });
