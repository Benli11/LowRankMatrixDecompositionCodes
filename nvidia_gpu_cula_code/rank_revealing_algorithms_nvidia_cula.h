#include "matrix_vector_functions_nvidia_cula.h"


/* computes the approximate low rank SVD of rank k of matrix M using BBt version */
void randomized_low_rank_svd1(mat *M, int k, mat **U, mat **S, mat **V);


/* computes the approximate low rank SVD of rank k of matrix M using QR version */
void randomized_low_rank_svd2(mat *M, int k, mat **U, mat **S, mat **V);


/* computes the approximate low rank SVD of rank k of matrix M using QR version 
 * with range sampling via (M M^T)^q M R*/
void randomized_low_rank_svd3_old(mat *M, int k, int q, mat **U, mat **S, mat **V);


/* computes the approximate low rank SVD of rank k of matrix M using QR version 
 * with range sampling via (M M^T)^q M R*/
void randomized_low_rank_svd3(mat *M, int k, int q, int s, mat **U, mat **S, mat **V);


/* computes the approximate low rank SVD of rank k of matrix M using the 
QB blocked algorithm for Q and BBt method; desired k \approx kstep*nstep */
void randomized_low_rank_svd4(mat *M, int kstep, int nstep, int q, mat **U, mat **S, mat **V);


/* version 2 of ramdomized low rank SVD with autorank estimation 1 */
void randomized_low_rank_svd2_autorank1(mat *M, double frac_of_max_rank, double TOL, mat **U, mat **S, mat **V);


/* version 2 of ramdomized low rank SVD with autorank estimation 2 */
void randomized_low_rank_svd2_autorank2(mat *M, int kblocksize, double TOL, mat **U, mat **S, mat **V);


/* computes the approximate low rank SVD of rank k of matrix M using QR version 
via (M M^T)^q M R, automatically estimates the rank needed */
void randomized_low_rank_svd3_autorank2(mat *M, int kblocksize, double TOL, int q, int s, mat **U, mat **S, mat **V);
 

/* householder matrix implementation */
void get_householder_matrix(vec *x, int ind1, int ind2, mat *H);

/* partial pivoted qr via householder  - faster version, no P */
void pivoted_QR_of_specified_rank(mat *M, int k, int *frank, mat **Qk, mat **Rk, vec **I);

/* randQB algorithm one vector at a time */
void randQB_p(mat *M, int k, int p, mat **Q, mat **B);

/* randQB algorithm blocked */
void randQB_pb(mat *M, int kstep, int nstep, int p, mat **Q, mat **B);

