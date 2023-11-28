// Perform function calls by causing segmentation faults 👍

#include <iostream>
#include <csignal>
#include <setjmp.h>

using namespace std;

/*
 * setup the function to call, its args, and a jump buffer to recover from
 * the SIGSEGV with. then cause a segfault with *(char*)0 = 0.
 * the SIGSEGV handler will call the function and store the result in sigResult,
 * which we cast to the appropriate type and return.
 */
#define CALL(F, A, T) \
    ([A](){ \
        sigArgs = (void*)A; \
        jmp_buf buf; \
        jmpBuf = &buf; \
        if (!sigsetjmp(buf, SIGSEGV)) { *(char*)(&F) = 0; } \
        return (T)(*(T*)sigResult); \
    })()

void *sigArgs;
void *sigResult = nullptr;
jmp_buf *jmpBuf;

void *add(void *args) {
    int *fargs = (int*)args;
    int *result = (int*)malloc(sizeof(int));
    *result = fargs[0] + fargs[1];
    return (void*)result;
}

void *vec3Add(void *args) {
    float *fargs = (float*)args;
    float *result = (float*)calloc(3, sizeof(float));
    for (int i = 0; i < 3; i++) {
        result[i] = fargs[i] + fargs[i+3];
    }

    // must return as a ptr to the real result
    float **resPtr = (float**)malloc(sizeof(float*));
    *resPtr = result;
    return resPtr;
}

void segv_handler(int sig, siginfo_t *info, void *uctx) {
    // clean up the previous return value if it exists
    if (sigResult != nullptr)
        free(sigResult);

    void *(*sigCall)(void*) = (void *(*)(void*)) info->si_addr;
    sigResult = (*sigCall)(sigArgs);
    siglongjmp(*jmpBuf, 1);
}

void setup_handler() {
    struct sigaction act = {0};
    act.sa_sigaction = segv_handler;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &act, NULL);
}

int main() {
    setup_handler();
    
    printf("calling add\n");
    int args[2] = { 3, 7 };
    int addRes = CALL(add, args, int);
    printf("%d + %d = %d\n", args[0], args[1], addRes);

    printf("calling add again!\n");
    args[0] = 20;
    args[1] = 30;
    addRes = CALL(add, args, int);
    printf("%d + %d = %d\n", args[0], args[1], addRes);
    
    printf("calling vecAdd\n");
    float vecArg[6] = {
        1.5, 2.1, 3.34,
        10.2, 20.6, 30.66
    };
    float *vecRes = CALL(vec3Add, vecArg, float*);
    printf(
        "[%.2f, %.2f, %.2f] + [%.2f, %.2f, %.2f] = [%.2f, %.2f, %.2f]\n",
        vecArg[0], vecArg[1], vecArg[2],
        vecArg[3], vecArg[4], vecArg[5],
        vecRes[0], vecRes[1], vecRes[2]
    );

    free(vecRes);
    free(sigResult);
    return 0;
}
