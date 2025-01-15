#include <stdlib.h>
#include <stdio.h>

volatile char MARKER_START, MARKER_END;

typedef struct matrix {
  int rows;
  int cols;
  int *data;
} matrix_t;

matrix_t *alloc_matrix(int rows, int cols) {
  matrix_t *out = malloc(sizeof(matrix_t));
  out->rows = rows;
  out->cols = cols;
  out->data = calloc(rows * cols, sizeof(int));

  return out;
}

int *access_matrix(matrix_t *mat, int row, int col) {
  return (mat->data + (row * mat->cols) + col);
}

void free_matrix(matrix_t *mat) {
  free(mat->data);
  free(mat);
}

void print_matrix(FILE *fp, matrix_t *mat) {
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      fprintf(fp, "%d ", mat->data[(i * mat->cols) + j]);
    }
    fprintf(fp, "\n");
  }
}

matrix_t *matmul(matrix_t *left, matrix_t *right) {
if(left->cols != right->rows){
return NULL;
}
  matrix_t *out = alloc_matrix(left->rows, right->cols);

  MARKER_START = 97;
  for (int i = 0; i < left->rows; i++) {
    for (int j = 0; j < right->cols; j++) {
   int sum=0;
      for (int k = 0; k < left->cols; k++) {
      sum += *access_matrix(left, i, k) * (*access_matrix(right, k, j));
      }
        *access_matrix(out, i, j) = sum;
      }
    }
  
  MARKER_END = 98;

  return out;
}

int main(int argc, char **argv) {
  // Construct L and R matrices from the input
  // Input: a b c l1 l2 l3 l4... ln r1 r2 r3 ... rn (Left matrix is a x b, right
  // matrix is b x c), rest are left and right input values respectively.

  int a = atoi(argv[1]);
  int b = atoi(argv[2]);
  int c = atoi(argv[3]);

  matrix_t *left = alloc_matrix(a, b);
  matrix_t *right = alloc_matrix(b, c);

  for (int i = 0; i < a; i++) {
    for (int j = 0; j < b; j++) {
      printf("writing %d to %p\n", atoi(argv[4 + (i * b) + j]),
             access_matrix(left, i, j));
      *access_matrix(left, i, j) = atoi(argv[4 + (i * b) + j]);
    }
  }

  for (int i = 0; i < b; i++) {
    for (int j = 0; j < c; j++) {
      *access_matrix(right, i, j) = atoi(argv[4 + (a * b) + (i * c) + j]);
    }
  }

  print_matrix(stdout, left);
  print_matrix(stdout, right);

  matrix_t *out = matmul(left, right);

  FILE *marker_f = fopen(".marker", "wb");
  FILE *mat_f = fopen(".mat", "wb");

  fprintf(marker_f, "%llx\n%llx", (unsigned long long int)&MARKER_START,
          (unsigned long long int)&MARKER_END);
  print_matrix(mat_f, out);

  free_matrix(out);
  free_matrix(left);
  free_matrix(right);
}
