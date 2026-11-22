/* second file: defines the shared global counter and the function */
int counter = 1000;                 /* definition (external linkage) */
int next_token_ext(void) { return ++counter; }
