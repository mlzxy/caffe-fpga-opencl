#define MAX_REC_N 16


pipe int p0 __attribute__((xcl_reqd_pipe_depth(MAX_REC_N)));

kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void kernel0(__global int *g_input)
{
    __local int l_input[MAX_REC_N];

    async_work_group_copy(l_input, g_input, MAX_REC_N, 0);

    printf("\n[kernel0] before pipe:\n", get_pipe_max_packets(p0));
    for (int i = 0; i < MAX_REC_N; ) {
        
        int ret = write_pipe(p0, &l_input[i]);
        if (ret == 0) {// write_pipe successful
        	printf("write %llx successfully\n", l_input[i]);
            i++;
        }
    }
    printf("\n");

    return;
}

kernel __attribute__((reqd_work_group_size(1, 1, 1)))
void kernel1(__global int *g_output)
{
    __local int l_output[MAX_REC_N];

    printf("\n[kernel1] after pipe:\n", get_pipe_max_packets(p0));
    for (int i = 0; i < MAX_REC_N; ) {
        int ret = read_pipe(p0, &l_output[i]);
        if (ret == 0) {
            printf("read %llx successfully \n", l_output[i]);
            i++;
        }
    }
    printf("\n");

    async_work_group_copy(g_output, l_output, MAX_REC_N, 0);

    return;
}



/*
大概知道怎么回事了， 只有第一个kernel有input， 最后一个kernel有output， 然后enqueue全部，
等待所有kernel执行完毕， 要根据此修改helper.cpp, 变为一次运行多个kernel的感觉。
具体kernel内部的pipe关系，是在kernel.cl这个文件里面处理的。
*/


/*
#include <clc.h>   // needed for OpenCL kernels
// 这个解决方式很不错，也算是全体pipeline了吧， 那个邮件不用发了。
pipe int p0 __attribute__((xcl_reqd_pipe_depth(512)));
pipe int p1 __attribute__((xcl_reqd_pipe_depth(512)));
// Stage 1
kernel __attribute__ ((reqd_work_group_size(256, 1, 1)))
void input_stage(__global int *input) {
write_pipe(p0, &input[get_local_id(0)]);
}
// Stage 2
kernel __attribute__ ((reqd_work_group_size(256, 1, 1)))
void adder_stage(int inc) {
int input_data, output_data;
read_pipe(p0, &input_data);
output_data = input_data + inc;
write_pipe(p1, &output_data);
}
// Stage 3
kernel __attribute__ ((reqd_work_group_size(256, 1, 1)))
void output_stage(__global int *output) {
read_pipe(p1, &output[get_local_id(0)]);
}
*/
