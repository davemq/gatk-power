/* Header files */

/* We need to include */
/* - math.h for NAN */
/* - stdlib.h to use src_c{calloc} and src_c{posix_memalign} */
/* - string.h for src_c{memset} */
/* - PowerPairHMM.h for JNI and function declarations */


/*
 * NOTE: THIS FILE IS GENERATED from an Org mode document. Edit the original
 * document and re-tangle this file.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "PowerPairHMM.h"

/* getDouble2dArray: Get access to double [][] arrays */

/* This C function */
/* - gets the size of the outer array using src_c{GetObjectLength} */
/* - allocates an array of src_c{jdouble *} */
/* - allocates an array of src_c{jdoubleArray} to use later to release */
/*   memory */
/* - for each element of the outer array */
/*   - call src_c{GetDoubleArrayElements} to get access to the Java */
/*     src_java{double [][]} */
/*   - save the array src_c{jdoubleArray} and the native src_c{jdouble *} */


static
jdouble **
getDouble2dArray(JNIEnv *env, jobjectArray matrix, jdoubleArray **arrays)
{



	/* Get the length of the outer array. */


	jsize jlen = (*env)->GetArrayLength(env, matrix);
	size_t len = (size_t) jlen;



	/* Allocate src_c{jdouble *} and src_c{jdoubleArray} arrays. We */
	/* use src_c{posix_memalign} for the src_c{jdouble *} array to */
	/* help the compiled code use POWER vector instructions. */


	jdouble **ptrs;
	jdoubleArray *arrayp;

	int rc = posix_memalign((void **)&ptrs, 16, len * sizeof(jdouble *));
	if (rc != 0) {
		return NULL;
	}
	(void) memset(ptrs, 0, len * sizeof(jdouble *));

	*arrays = calloc(len, sizeof(jdoubleArray *));
	if (*arrays == NULL) {
		goto cleanptrs;
	}
	arrayp = *arrays;



	/* For each index src_c{i} from 0 to src_c{len - 1} */
	/* - get outer array element, a src_c{jdoubleArray}. Save in src_c{*arrays[i]} */
	/* - call GetDoubleArrayElements on src_c{*arrays[i]}. Save in */
        /*   src_c{ptrs[i]} */


	for (jsize i = 0; i < jlen; i++) {
		arrayp[i] = (*env)->GetObjectArrayElement(env, matrix, i);
		if (arrayp[i] == NULL) {
			goto cleanarrays;
		}
		ptrs[i] = (*env)->GetDoubleArrayElements(env, arrayp[i], NULL);
		if (ptrs[i] == NULL) {
			goto cleanarrays;
		}
	}



	/* At this point, we're done, so return src_c{ptrs}. */


	return ptrs;



	/* Now we have the cleanup labels. Recall that the code jumps to */
	/* either src_c{cleanptrs:} or src_c{cleanarrays:} in the case of */
	/* errors. We handle these in reverse order, with */
	/* src_c{cleanarrays:} first and then falling through to */
	/* src_c{cleanptrs:}. */

	/* For src_c{cleanarrays:} we need to release memory back to */
	/* Java. Rely on the fact that we zeroed out src_c{ptrs} and */
	/* src_c{*arrays} when allocating memory to break out of the loop */
	/* early. Also free src_c{*arrays} here. */


cleanarrays:
	for (int i = 0; i < len; i++) {
		if (ptrs[i] == NULL) {
			break;
		}

		/* ReleaseDoubleArrayElements() with mode=0 frees ptrs[i] */
		(*env)->ReleaseDoubleArrayElements(env, *arrays[i], ptrs[i], 0);
	}

	free(*arrays);



	/* src_c{cleanptrs:} only handles freeing src_c{*ptrs}, then */
	/* returns NULL. */


cleanptrs:
	free(ptrs);
	return NULL;
}

/* releaseDouble2dArray: release memory and flush to Java */

/* src_c{releaseDouble2dArray} is similar to the clean up code in */
/* src_c{getDouble2dArray}, but leaves out checks for NULL pointers as */
/* there shouldn't be any. */


static
void
releaseDouble2dArray(JNIEnv *env, jobjectArray matrix, jdouble **native, jdoubleArray **jarray)
{



	/* First, get the length of the array. */


	jsize len = (*env)->GetArrayLength(env, matrix);



	/* For each index from 0 to src_c{len - 1} */
	/* - call ReleaseDoubleArrayElements with src_c{native[i]} and */
        /*   src_c{*jarray[i]} */


	for (int i = 0; i < len; i++) {
		/* ReleaseDoubleArrayElements with mode=0 frees memory */
		(*env)->ReleaseDoubleArrayElements(env, *jarray[i], native[i], 0);
	}



	/* Now, free src_c{jarray} and src_c{native}. */


	free(*jarray);
	*jarray = NULL;
	free(native);
}

/* Native code and JNI */


/* The native code will use JNI to get access to Java class data. Since */
/* we have 5 matrices declared as src_java{double [][]}, we'll write a C */
/* function src_c{get2dDoubleArray} to set up access to these matrices. */

/* A src_java{double [][]} is implemented as an array of arrays of */
/* doubles. This is passed as a src_c{jObjectArray} to */
/* C/C++. src_c{GetObjectArrayLength} is called to get the length of the */
/* outer array. Then a array of double pointers (src_c{double *}) is */
/* dynamically allocated using src_c{malloc} or src_c++{new}. */

/* The native code for the main loop of */
/* src_java{subComputeReadLikelihoodGivenHaplotypeLog10} will start with */


JNIEXPORT jdouble JNICALL
Java_com_ibm_power_pairhmm_PowerPairHMM_subComputeNative
(JNIEnv *env, jobject this, jint paddedReadLength, jint hapStartIndex,
 jint paddedHaplotypeLength, jobjectArray matchMatrix, jobjectArray priorMatrix,
 jobjectArray transitionMatrix, jobjectArray insertionMatrix,
 jobjectArray deletionMatrix)
{



	/* Recall in [[Second loop]] that src_java{hapStartIndex}, */
	/* src_java{paddedHaplotypeLength}, and src_java{paddedReadLength} are */
	/* all integer types. We create 3 C variables that use these */
	/* values cast to src_c{int}. */


	int start = (int) hapStartIndex;
	int hapLength = (int) paddedHaplotypeLength;
	int readLength = (int) paddedReadLength;



	/* We declare and initialize src_c{finalSumProbabilities} to */
	/* src_c{NAN} here so if there's a problem encountered before */
	/* computing src_c{finalSumProbabilities} we return src_c{NAN}. */


	jdouble finalSumProbabilities = NAN;


	
	/* The rest of the variables are src_c{jobjectArray}s that are Java */
	/* src_java{double [][]}. We will represent these as src_c{double **} */
	/* variables. */


	jdouble **match, **prior, **transition, **insertion, **deletion;



	/* Underneath these we have src_c{jdoubleArray}s that we will gain access */
	/* to via the JNI function src_c{GetDoubleArrayElements}. In order to */
	/* release our access when we're done, we'll need to pass the original */
	/* src_c{jdoubleArray} and the native type pointer to */
	/* src_c{ReleaseDoubleArrayElements()}. So we'll need to track both of */
	/* these together. We will track the src_c{jdoubleArray}s in separate */
	/* arrays, dynamically allocated. */


	jdoubleArray *jMatch, *jPrior, *jTransition, *jInsertion, *jDeletion;



	/* For each of the matrices, we call a new routine */
	/* src_c{getDouble2dArray} to retrieve the array elements and track them. */


	if ((match = getDouble2dArray(env, matchMatrix, &jMatch)) == NULL) {
		return NAN;
	}
	if ((prior = getDouble2dArray(env, priorMatrix, &jPrior)) == NULL) {
		goto free_match;
	}
	if ((transition = getDouble2dArray(env, transitionMatrix, &jTransition)) == NULL) {
		goto free_prior;
	}
	if ((insertion = getDouble2dArray(env, insertionMatrix, &jInsertion)) == NULL) {
		goto free_transition;
	}
	if ((deletion = getDouble2dArray(env, deletionMatrix, &jDeletion)) == NULL) {
		goto free_insertion;
	}



	/* If these calls succeed, we can now get on to the matrix */
	/* manipulations. */

	/* We add some constants used in the loop from Java class */
	/* PairHMMModel here. We could get these via JNI, but they */
	/* wouldn't be constants, possibly making the loop below more */
	/* difficult to optimize. */


	/* From class PairHMMModel */
	const int matchToMatch       = 0,
		indelToMatch         = 1,
		matchToInsertion     = 2,
		insertionToInsertion = 3,
		matchToDeletion      = 4,
		deletionToDeletion   = 5;

	for (int i = 1; i < readLength; i++) {
		/* +1 here is because hapStartIndex is 0-based, but our matrices are 1 */
		/* based */
		for (int j = start+1; j < hapLength; j++) {
			match[i][j] = prior[i][j] *
				( match[i - 1][j - 1] *
				  transition[i][matchToMatch] +
				  insertion[i - 1][j - 1] *
				  transition[i][indelToMatch] +
				  deletion[i - 1][j - 1] *
				  transition[i][indelToMatch] );
			insertion[i][j] = match[i - 1][j] *
				transition[i][matchToInsertion] +
				insertion[i - 1][j] *
				transition[i][insertionToInsertion];
			deletion[i][j] = match[i][j - 1] *
				transition[i][matchToDeletion] +
				deletion[i][j - 1] *
				transition[i][deletionToDeletion];
		}
	}



	/* Next we compute the final log probability. */


	/*
	 * final log probability is the log10 sum of the last element in the
	 * Match and Insertion state arrays
	 * this way we ignore all paths that ended in deletions! (huge)
	 * but we have to sum all the paths ending in the M and I matrices,
	 * because they're no longer extended.
	 */
        int endI = readLength - 1;
        finalSumProbabilities = 0.0;
        for (int j = 1; j < hapLength; j++) {
            finalSumProbabilities += match[endI][j] + insertion[endI][j];
        }



	/* We next release all the memory for the matrices. */


	releaseDouble2dArray(env, deletionMatrix, deletion, &jDeletion);
free_insertion:
	releaseDouble2dArray(env, insertionMatrix, insertion, &jInsertion);
free_transition:
	releaseDouble2dArray(env, transitionMatrix, transition, &jTransition);
free_prior:
	releaseDouble2dArray(env, priorMatrix, prior, &jPrior);
free_match:
	releaseDouble2dArray(env, matchMatrix, match, &jMatch);



	/* Finally, we return src_c{finalSumProbabilities}. */


	return finalSumProbabilities;
}

#ifdef __cplusplus
}
#endif
