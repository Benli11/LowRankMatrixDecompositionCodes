#include "low_rank_svd_algorithms_gsl.h"



/* computes the approximate low rank SVD of rank k of matrix M using BBt version */
void randomized_low_rank_svd1(gsl_matrix *M, int k, gsl_matrix *U, gsl_matrix *S, gsl_matrix *V){
    int i,j,m,n;
    double val;
    m = M->size1; n = M->size2;

    // build random matrix
    printf("form RN..\n");
    gsl_matrix *RN = gsl_matrix_calloc(n, k); // calloc sets all elements to zero
    initialize_random_matrix(RN);

    // multiply to get matrix of random samples Y
    printf("form Y..\n");
    gsl_matrix *Y = gsl_matrix_alloc(m,k);
    matrix_matrix_mult(M, RN, Y);

    // build Q from Y
    printf("form Q..\n");
    gsl_matrix *Q = gsl_matrix_alloc(m,k);
    QR_factorization_getQ(Y, Q);

    
    // build the matrix B B^T = Q^T M M^T Q 
    printf("form BBt..\n");
    gsl_matrix *B = gsl_matrix_alloc(k,n);
    matrix_transpose_matrix_mult(Q,M,B);

    gsl_matrix *Bt = gsl_matrix_alloc(n,k);
    matrix_transpose_matrix_mult(M,Q,Bt);    

    gsl_matrix *BBt = gsl_matrix_alloc(k,k);
    matrix_matrix_mult(B,Bt,BBt);    


    // compute eigendecomposition of BBt
    printf("get eigendecomposition of BBt..\n");
    gsl_vector *evals = gsl_vector_alloc(k);
    gsl_matrix *Uhat = gsl_matrix_alloc(k, k);
    compute_evals_and_evecs_of_symm_matrix(BBt, evals, Uhat);


    // compute singular values and matrix Sigma
    printf("form S..\n");
    gsl_vector *singvals = gsl_vector_alloc(k);
    for(i=0; i<k; i++){
        gsl_vector_set(singvals,i,sqrt(gsl_vector_get(evals,i)));
    }
    build_diagonal_matrix(singvals, k, S);
    

    // compute U = Q*Uhat mxk * kxk = mxk  
    printf("form U..\n");
    matrix_matrix_mult(Q,Uhat,U);


    // compute nxk V 
    // V = B^T Uhat * Sigma^{-1}
    printf("form V..\n");
    gsl_matrix *Sinv = gsl_matrix_alloc(k,k);
    gsl_matrix *UhatSinv = gsl_matrix_alloc(k,k);
    invert_diagonal_matrix(Sinv,S);
    matrix_matrix_mult(Uhat,Sinv,UhatSinv);
    matrix_matrix_mult(Bt,UhatSinv,V);

    // clean up
    gsl_matrix_free(RN);
    gsl_matrix_free(Y);
    gsl_matrix_free(Q);
    gsl_matrix_free(B);
    gsl_matrix_free(Bt);
    gsl_matrix_free(Sinv);
    gsl_matrix_free(UhatSinv);
}


/* computes the approximate low rank SVD of rank k of matrix M using QR method */
void randomized_low_rank_svd2(gsl_matrix *M, int k, gsl_matrix *U, gsl_matrix *S, gsl_matrix *V){
    int i,j,m,n;
    double val;
    m = M->size1; n = M->size2;

    // build random matrix
    printf("form RN..\n");
    gsl_matrix *RN = gsl_matrix_calloc(n,k); // calloc sets all elements to zero
    //RN = matrix_load_from_file("data/R.mtx");
    initialize_random_matrix(RN);

    // multiply to get matrix of random samples Y
    printf("form Y..\n");
    gsl_matrix *Y = gsl_matrix_alloc(m,k);
    matrix_matrix_mult(M, RN, Y);

    // build Q from Y
    printf("form Q..\n");
    gsl_matrix *Q = gsl_matrix_alloc(m,k);
    QR_factorization_getQ(Y, Q);

    // form Bt = Mt*Q : nxm * mxk = nxk
    printf("form Bt..\n");
    gsl_matrix *Bt = gsl_matrix_alloc(n,k);
    matrix_transpose_matrix_mult(M,Q,Bt);

    printf("doing QR..\n");
    gsl_matrix *Qhat = gsl_matrix_calloc(n,k);
    gsl_matrix *Rhat = gsl_matrix_calloc(k,k);
    compute_QR_compact_factorization(Bt,Qhat,Rhat);

    // compute SVD of Rhat (kxk)
    printf("doing SVD..\n");
    gsl_matrix *Uhat = gsl_matrix_alloc(k,k);
    gsl_vector *Sigmahat = gsl_vector_alloc(k);
    gsl_matrix *Vhat = gsl_matrix_alloc(k,k);
    gsl_vector *svd_work_vec = gsl_vector_alloc(k);
    gsl_matrix_memcpy(Uhat, Rhat);
    gsl_linalg_SV_decomp (Uhat, Vhat, Sigmahat, svd_work_vec);

    // record singular values
    printf("form S..\n");
    build_diagonal_matrix(Sigmahat, k, S);

    // U = Q*Vhat
    printf("form U..\n");
    matrix_matrix_mult(Q,Vhat,U);

    // V = Qhat*Uhat
    printf("form V..\n");
    matrix_matrix_mult(Qhat,Uhat,V);

    // free stuff
    gsl_matrix_free(RN);
    gsl_matrix_free(Y);
    gsl_matrix_free(Q);
    gsl_matrix_free(Rhat);
    gsl_matrix_free(Qhat);
    gsl_matrix_free(Uhat);
    gsl_matrix_free(Vhat);
    gsl_matrix_free(Bt);
}

