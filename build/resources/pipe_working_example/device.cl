#define MAX_REC_N 16


pipe long p0 __attribute__((xcl_reqd_pipe_depth(MAX_REC_N)));

kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void kernel0(__global long *g_input)
{
    __local long l_input[MAX_REC_N];

    async_work_group_copy(l_input, g_input, MAX_REC_N, 0);

    printf("\n[kernel0] before pipe:\n", get_pipe_max_packets(p0));
    for (int i = 0; i < MAX_REC_N; ) {
        printf("%llx ", l_input[i]);
        int ret = write_pipe(p0, &l_input[i]);
        if (ret == 0) {// write_pipe successful
            i++;
        }
    }
    printf("\n");

    return;
}

kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void kernel1(__global long *g_output)
{
    __local long l_output[MAX_REC_N];

    printf("\n[kernel1] after pipe:\n", get_pipe_max_packets(p0));
    for (int i = 0; i < MAX_REC_N; ) {
        int ret = read_pipe(p0, &l_output[i]);
        if (ret == 0) {
            printf("%llx ", l_output[i]);
            i++;
        }
    }
    printf("\n");

    async_work_group_copy(g_output, l_output, MAX_REC_N, 0);

    return;
}

