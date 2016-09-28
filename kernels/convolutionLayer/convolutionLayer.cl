typedef float dType;
#define MAX_WEIGHT_DATA_NUM  405000
#define MAX_BIAS_DATA_NUM  100
#define MAX_OUTPUT_FM_DATA_NUM 11520
#define MAX_POOLING_SIZE 2

#define pingPongSize 2
#define MEM_PHASE  (computingPhase == false)    // 1, 0
#define COMPUTE_PHASE (computingPhase == true)  // 0, 1
#define STR(x) #x
// string concate for macro: http://stackoverflow.com/questions/2191316/macro-for-concatenating-two-strings-in-c
#define ASSERT(condition) \
  if (!(condition)) { printf("ASSERT FAILURE: " STR(condition) ); return; }
/*
所以PingPong Buffer是足够的。

dataOnly 1  -> store image in 0.
conv 0 ->      read from computing phase(0), store in memory phase(1)
conv 1 ->      read from computing phase(1), store in memory phase(0)
dataLayer 0 -> read from computing phase(0), store in output_feature_map, new image store in memory 1.
conv 1 ->      read from computing phase(1), store in memory phase(0)
*/


global dType weightCache[pingPongSize][MAX_WEIGHT_DATA_NUM];
global dType biasCache[pingPongSize][MAX_BIAS_DATA_NUM];
global dType fmCache[pingPongSize][MAX_OUTPUT_FM_DATA_NUM];


kernel void convolutionLayer( // the possible data will be transfered.
                              __global dType *inputFeatureMap,  __global float *outputFeatureMap,
                              /***** all current conv layer related computation parameters: *****/
                              const unsigned int convStride,
                              const unsigned int dilation,
                              const unsigned int kernelSize,
                              const unsigned int convPad,
                              const unsigned int inputChannel,
                              const unsigned int inputFeatureMapHeight,
                              const unsigned int inputFeatureMapWidth,
                              const unsigned int outputChannel,
                              const unsigned int outputFeatureMapHeight,
                              const unsigned int outputFeatureMapWidth,
                              // use activation relu or not.
                              const bool relu,
                              const unsigned int poolingSize,
                              // fill the next buffer carefully.
                              const unsigned int nextConvPadding,
                              /***   end   ***/

                              // all data loading related parameter
                              const bool dataOutputLayer,
                              const bool dataLayer,
                              const bool dataOnly,
                              __global float *nextWeight, __global float *nextBias,
                              const unsigned int dataChannel,
                              const unsigned int dataHeight,
                              const unsigned int dataWidth,
                              const unsigned int nextInputChannel,
                              const unsigned int nextOutputChannel,
                              const unsigned int nextKernelSize,
                              const dType dataScale,
                              // phase ping pong
                              const bool computingPhase)
{
    // important dimension variable
    unsigned int globalSize_0 = get_global_size(0);
    unsigned int globalSize_1 = get_global_size(1);
    unsigned int globalSize_2 = get_global_size(2);
    unsigned int localSize_0 = get_local_size(0);
    unsigned int localSize_1 = get_local_size(1);
    unsigned int localSize_2 = get_local_size(2);
    unsigned int groupSize_0 = get_num_groups(0);
    unsigned int groupSize_1 = get_num_groups(1);
    unsigned int groupSize_2 = get_num_groups(2);
    unsigned int globalSize = globalSize_0*globalSize_1*globalSize_2;
    unsigned int gid_0 = get_global_id(0);  // no pooling
    unsigned int gid_1 = get_global_id(1); // has pooling
    unsigned int gid_2 = get_global_id(2); // has pooling

    // unsigned int lid_0 = get_local_id(0);  // no pooling

    unsigned int lid_1 = get_local_id(1); // has pooling
    unsigned int lid_2 = get_local_id(2); // has pooling
    unsigned int gop_0 = get_group_id(0);  // no pooling
    unsigned int gop_1 = get_group_id(1); // has pooling
    unsigned int gop_2 = get_group_id(2); // has pooling
    unsigned int gid = gid_0 * outputFeatureMapWidth*outputFeatureMapHeight + gid_1 * outputFeatureMapWidth + gid_2;

    // dimension check and hints.
    ASSERT(localSize_0 == 1)
    ASSERT(localSize_1 == poolingSize) //poolingSize = localSize_1
    ASSERT(localSize_2 == poolingSize)
    ASSERT(globalSize_0 == outputChannel) //outputChannel = globalSize_0
    ASSERT(globalSize_1 == outputFeatureMapHeight) //outputFeatureMapHeight = globalSize_1
    ASSERT(globalSize_2 == outputFeatureMapWidth) //outputFeatureMapWidth = globalSize_2
    ASSERT(groupSize_0 == outputChannel)
    ASSERT(groupSize_1 == (unsigned int)(outputFeatureMapHeight/poolingSize))
    ASSERT(groupSize_2 == (unsigned int)(outputFeatureMapWidth/poolingSize))




    // load weight and bias
    if(gid < nextOutputChannel) {
        biasCache[MEM_PHASE][gid] = nextBias[gid];
    }
    __private unsigned int totalWeightToLoad =  nextKernelSize * nextKernelSize * nextInputChannel * nextOutputChannel;
    __private unsigned int weightLoadCounter = gid;
    while(weightLoadCounter < totalWeightToLoad){
        weightCache[MEM_PHASE][weightLoadCounter] = nextWeight[weightLoadCounter];
        weightLoadCounter += globalSize;
    }


    // load new image if needed.
    if(dataLayer){
        __private unsigned int dataLoadCounter = gid;
        __private unsigned int totalDataToLoad = dataChannel*dataHeight*dataWidth;
        while(dataLoadCounter < totalDataToLoad){
            fmCache[MEM_PHASE][dataLoadCounter] = inputFeatureMap[dataLoadCounter] * dataScale;
            dataLoadCounter += globalSize;
        }
        if(dataOnly)
            return;
    }

    // do convolution computation.
    __private unsigned int dilatedKernelSize = (kernelSize-1)*dilation+1;
    __private dType workItemResult = 0;
    __private unsigned int paddedInputFeatureMapWidth = inputFeatureMapWidth + (2*convPad);
    __private unsigned int paddedInputFeatureMapHeight = inputFeatureMapHeight + (2*convPad);
    for (unsigned int c = 0; c<inputChannel; c++)
        for(unsigned int i = 0; i<dilatedKernelSize; i++)
            for(unsigned int j = 0; j<dilatedKernelSize; j++)
                workItemResult += (fmCache[COMPUTE_PHASE][c*(paddedInputFeatureMapWidth*paddedInputFeatureMapHeight)         //accessing which channel
                                                            +(gid_1*convStride + i*dilation) * paddedInputFeatureMapWidth
                                                            +(gid_2*convStride + j*dilation)]                                  //
                                                            *
                                   weightCache[COMPUTE_PHASE][gid_0 * kernelSize * kernelSize * inputChannel // which filter
                                                            + c * (kernelSize*kernelSize)   //which channel
                                                            + i * kernelSize + j])  // inside this channel, 2d filter.
    barrier(CLK_LOCAL_MEM_FENCE);


    //find the max among work group, max pooling.
    __local dType maxValue;
    maxValue = FLT_MIN;
    for (unsigned int i = 0; i<localSize_1;i++)
        for(unsigned int j = 0; j<localSize_2;j++){
            barrier(CLK_LOCAL_MEM_FENCE);
            if(lid_1 == i && lid_2 == j){
                maxValue = maxValue > workItemResult? maxValue : workItemResult;
            }
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    // activation
    maxValue =  relu ? ( maxValue > 0 ? maxValue : 0 ) : maxValue;


    // outputFeatureMapWidth=(unsigned int)outputFeatureMapWidth/poolingSize;
    // outputFeatureMapHeight=(unsigned int)outputFeatureMapHeight/poolingSize;
    groupSize_1 += nextConvPadding;
    groupSize_2 += nextConvPadding;
    __private unsigned int outputIndex = gop_0*(groupSize_1*groupSize_2) + gop_1*groupSize_2 + gop_2;
    if(dataLayer){
        outputFeatureMap[outputIndex] = maxValue;
    }else {
        fmCache[MEM_PHASE][outputIndex] = maxValue;
    }
}
