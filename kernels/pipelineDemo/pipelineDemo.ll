; ModuleID = '../../kernels/pipelineDemo/pipelineDemo.cl'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v16:16:16-v24:32:32-v32:32:32-v48:64:64-v64:64:64-v96:128:128-v128:128:128-v192:256:256-v256:256:256-v512:512:512-v1024:1024:1024"
target triple = "spir"

%opencl.event_t = type opaque

@kernel1.l_output = internal addrspace(3) global [128 x i32] zeroinitializer, align 4
@.str = private addrspace(2) unnamed_addr constant [19 x i8] c"read_pipe(p0, %d)\0A\00", align 1
@.str1 = private addrspace(2) unnamed_addr constant [29 x i8] c"read_pipe(p0, %d) Succeeded\0A\00", align 1
@.str2 = private addrspace(2) unnamed_addr constant [26 x i8] c"read_pipe(p0, %d) Failed\0A\00", align 1
@.str3 = private addrspace(2) unnamed_addr constant [33 x i8] c"async_work_group_copy in kernel1\00", align 1
@.str4 = private addrspace(2) unnamed_addr constant [42 x i8] c"async_work_group_copy finished in kernel1\00", align 1
@kernel0.l_input = internal addrspace(3) global [128 x i32] zeroinitializer, align 4
@.str5 = private addrspace(2) unnamed_addr constant [20 x i8] c"write_pipe(p0, %d)\0A\00", align 1
@.str6 = private addrspace(2) unnamed_addr constant [31 x i8] c"write_pipe(p0, %d) Succeeded.\0A\00", align 1
@.str7 = private addrspace(2) unnamed_addr constant [28 x i8] c"write_pipe(p0, %d) Failed.\0A\00", align 1
@0 = internal addrspace(4) constant [16 x i32] zeroinitializer
@p0 = internal global [16 x i32] addrspace(4)* @0, align 4

define spir_kernel void @kernel1(i32 addrspace(1)* %g_output) nounwind {
  %1 = alloca i32 addrspace(1)*, align 4
  %i = alloca i32, align 4
  %ret = alloca i32, align 4
  store i32 addrspace(1)* %g_output, i32 addrspace(1)** %1, align 4
  store i32 0, i32* %i, align 4
  br label %2

; <label>:2                                       ; preds = %22, %0
  %3 = load i32* %i, align 4
  %4 = icmp slt i32 %3, 128
  br i1 %4, label %5, label %23

; <label>:5                                       ; preds = %2
  %6 = load i32* %i, align 4
  %7 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([19 x i8] addrspace(2)* @.str, i32 0, i32 0), i32 %6)
  %8 = load i32 addrspace(4)** bitcast ([16 x i32] addrspace(4)** @p0 to i32 addrspace(4)**), align 4
  %9 = load i32* %i, align 4
  %10 = getelementptr inbounds [128 x i32] addrspace(3)* @kernel1.l_output, i32 0, i32 %9
  %11 = call i32 @_Z9read_pipePU3AS18ocl_pipeiPU3AS1i(i32 addrspace(4)* %8, i32 addrspace(3)* %10)
  store i32 %11, i32* %ret, align 4
  %12 = load i32* %ret, align 4
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %14, label %19

; <label>:14                                      ; preds = %5
  %15 = load i32* %i, align 4
  %16 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([29 x i8] addrspace(2)* @.str1, i32 0, i32 0), i32 %15)
  %17 = load i32* %i, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, i32* %i, align 4
  br label %22

; <label>:19                                      ; preds = %5
  %20 = load i32* %i, align 4
  %21 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([26 x i8] addrspace(2)* @.str2, i32 0, i32 0), i32 %20)
  br label %22

; <label>:22                                      ; preds = %19, %14
  br label %2

; <label>:23                                      ; preds = %2
  %24 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([33 x i8] addrspace(2)* @.str3, i32 0, i32 0))
  %25 = load i32 addrspace(1)** %1, align 4
  %26 = call spir_func %opencl.event_t* @_Z21async_work_group_copyPU3AS0iPKU3AS1ij9ocl_event(i32 addrspace(1)* %25, i32 addrspace(3)* getelementptr inbounds ([128 x i32] addrspace(3)* @kernel1.l_output, i32 0, i32 0), i32 128, %opencl.event_t* null)
  %27 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([42 x i8] addrspace(2)* @.str4, i32 0, i32 0))
  ret void
}

declare spir_func i32 @_Z6printfPKU3AS2cz(i8 addrspace(2)*, ...)

declare spir_func i32 @_Z9read_pipePU3AS18ocl_pipeiPU3AS1i(i32 addrspace(4)*, i32 addrspace(3)*)

declare spir_func %opencl.event_t* @_Z21async_work_group_copyPU3AS0iPKU3AS1ij9ocl_event(i32 addrspace(1)*, i32 addrspace(3)*, i32, %opencl.event_t*)

define spir_kernel void @kernel0(i32 addrspace(1)* %g_input) nounwind {
  %1 = alloca i32 addrspace(1)*, align 4
  %i = alloca i32, align 4
  %ret = alloca i32, align 4
  store i32 addrspace(1)* %g_input, i32 addrspace(1)** %1, align 4
  %2 = load i32 addrspace(1)** %1, align 4
  %3 = call spir_func %opencl.event_t* @_Z21async_work_group_copyPU3AS1iPKU3AS0ij9ocl_event(i32 addrspace(3)* getelementptr inbounds ([128 x i32] addrspace(3)* @kernel0.l_input, i32 0, i32 0), i32 addrspace(1)* %2, i32 128, %opencl.event_t* null)
  store i32 0, i32* %i, align 4
  br label %4

; <label>:4                                       ; preds = %24, %0
  %5 = load i32* %i, align 4
  %6 = icmp slt i32 %5, 128
  br i1 %6, label %7, label %25

; <label>:7                                       ; preds = %4
  %8 = load i32* %i, align 4
  %9 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([20 x i8] addrspace(2)* @.str5, i32 0, i32 0), i32 %8)
  %10 = load i32 addrspace(4)** bitcast ([16 x i32] addrspace(4)** @p0 to i32 addrspace(4)**), align 4
  %11 = load i32* %i, align 4
  %12 = getelementptr inbounds [128 x i32] addrspace(3)* @kernel0.l_input, i32 0, i32 %11
  %13 = call i32 @_Z10write_pipePU3AS18ocl_pipeiPU3AS1i(i32 addrspace(4)* %10, i32 addrspace(3)* %12)
  store i32 %13, i32* %ret, align 4
  %14 = load i32* %ret, align 4
  %15 = icmp eq i32 %14, 0
  br i1 %15, label %16, label %21

; <label>:16                                      ; preds = %7
  %17 = load i32* %i, align 4
  %18 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([31 x i8] addrspace(2)* @.str6, i32 0, i32 0), i32 %17)
  %19 = load i32* %i, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, i32* %i, align 4
  br label %24

; <label>:21                                      ; preds = %7
  %22 = load i32* %i, align 4
  %23 = call spir_func i32 (i8 addrspace(2)*, ...)* @_Z6printfPKU3AS2cz(i8 addrspace(2)* getelementptr inbounds ([28 x i8] addrspace(2)* @.str7, i32 0, i32 0), i32 %22)
  br label %24

; <label>:24                                      ; preds = %21, %16
  br label %4

; <label>:25                                      ; preds = %4
  ret void
}

declare spir_func %opencl.event_t* @_Z21async_work_group_copyPU3AS1iPKU3AS0ij9ocl_event(i32 addrspace(3)*, i32 addrspace(1)*, i32, %opencl.event_t*)

declare spir_func i32 @_Z10write_pipePU3AS18ocl_pipeiPU3AS1i(i32 addrspace(4)*, i32 addrspace(3)*)

!opencl.kernels = !{!0, !7}

!0 = metadata !{void (i32 addrspace(1)*)* @kernel1, metadata !1, metadata !2, metadata !3, metadata !4, metadata !5, metadata !6}
!1 = metadata !{metadata !"kernel_arg_addr_space", i32 1}
!2 = metadata !{metadata !"kernel_arg_access_qual", metadata !"none"}
!3 = metadata !{metadata !"kernel_arg_type", metadata !"int*"}
!4 = metadata !{metadata !"kernel_arg_type_qual", metadata !""}
!5 = metadata !{metadata !"kernel_arg_name", metadata !"g_output"}
!6 = metadata !{metadata !"reqd_work_group_size", i32 1, i32 1, i32 1}
!7 = metadata !{void (i32 addrspace(1)*)* @kernel0, metadata !1, metadata !2, metadata !3, metadata !4, metadata !8, metadata !6}
!8 = metadata !{metadata !"kernel_arg_name", metadata !"g_input"}
