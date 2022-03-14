#include "OpenclSystem.h"

#include <iostream>


OpenclSystem &OpenclSystem::Get()
{
    static OpenclSystem system;
    return system;
}

bool OpenclSystem::Init()
{
    if (_deviceData.ret == CL_SUCCESS)
        return true;

    _deviceData.ret = clGetPlatformIDs(1,
                                       &_deviceData.platform_id,
                                       &_deviceData.ret_num_platforms);
    if(_deviceData.ret != CL_SUCCESS)
    {
        std::cout << _deviceData.ret << "\n";
        return false;
    }

    std::cout << "platform_id: " << _deviceData.platform_id << "\n";
    _deviceData.ret = clGetDeviceIDs(_deviceData.platform_id,
                                     CL_DEVICE_TYPE_GPU,
                                     1,
                                     &_deviceData.device_id,
                                     &_deviceData.ret_num_devices);
    if(_deviceData.ret != CL_SUCCESS)
    {
        std::cout << _deviceData.ret << "\n";
        return false;
    }

    std::cout << "device_id: " << _deviceData.device_id << "\n";
    _deviceData.context = clCreateContext(NULL, 1, &_deviceData.device_id,
                                          NULL,
                                          NULL,
                                          &_deviceData.ret);
    std::cout << "context: " << _deviceData.context << "\n";
    _deviceData.command_queue = clCreateCommandQueueWithProperties(_deviceData.context,
                                                                   _deviceData.device_id,
                                                                   0,
                                                                   &_deviceData.ret);
    _wasInited = true;
    return true;
}

void OpenclSystem::Destroy()
{
    if (!_wasInited)
        return;

    _deviceData.ret = clFlush(_deviceData.command_queue);
    _deviceData.ret = clFinish(_deviceData.command_queue);
    if(_deviceData.program)
        _deviceData.ret = clReleaseProgram(_deviceData.program);
    if(_deviceData.kernel)
        _deviceData.ret = clReleaseKernel(_deviceData.kernel);
    _deviceData.ret = clReleaseCommandQueue(_deviceData.command_queue);
    _deviceData.ret = clReleaseContext(_deviceData.context);

    _wasInited = false;
}

void OpenclSystem::Compile(const std::string &code)
{
    if (code.empty() || code == _code)
        return;

    if(!_code.empty())
    {
        _deviceData.ret = clReleaseProgram(_deviceData.program);
        _deviceData.ret = clReleaseKernel(_deviceData.kernel);
    }
    const char* code_src = code.data();
    _deviceData.program = clCreateProgramWithSource(_deviceData.context,
                                                    1,
                                                    (const char**)&code_src,
                                                    NULL,
                                                    &_deviceData.ret);
    if (!_deviceData.program)
    {
        std::cout << "Failed to create compute program!\n";
        return;
    }
    _deviceData.ret = clBuildProgram(_deviceData.program,
                                     1,
                                     &_deviceData.device_id,
                                     NULL,
                                     NULL,
                                     NULL);
    if (_deviceData.ret != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        std::cout << "Failed to build program executable!\n";
        clGetProgramBuildInfo(_deviceData.program,
                              _deviceData.device_id,
                              CL_PROGRAM_BUILD_LOG,
                              sizeof(buffer),
                              buffer,
                              &len);
        std::cout << buffer << "\n";
        clReleaseProgram(_deviceData.program);
        _deviceData.program = 0;
        return;
    }
    _code = code;
}

bool OpenclSystem::Run(const std::string& functionName, const KernelArguments& args)
{
    if (!_deviceData.program)
    {
        std::cout << "Program not compiled\n";
        return false;
    }

    _deviceData.kernel = clCreateKernel(_deviceData.program,
                                        functionName.c_str(),
                                        &_deviceData.ret);
    if (!_deviceData.kernel || _deviceData.ret != CL_SUCCESS)
    {
        std::cout << "Failed to create compute kernel!\n";
        return false;
    }

    // Create gpu buffers
    cl_mem out_mem_obj = clCreateBuffer(_deviceData.context,
                                        CL_MEM_WRITE_ONLY,
                                        args.output.TotalSize(),
                                        NULL,
                                        &_deviceData.ret);
    if (!out_mem_obj)
    {
        std::cout << "Failed to allocate device memory!\n";
        return false;
    }

    _deviceData.ret = clSetKernelArg(_deviceData.kernel, 0, sizeof(cl_mem), (void *)&out_mem_obj);
    if (_deviceData.ret != CL_SUCCESS)
    {
        std::cout << "Failed to set kernel main buffer\n";
        clReleaseMemObject(out_mem_obj);
        return false;
    }

    for (size_t i = 0; i < args.optional.size(); ++i)
    {
        _deviceData.ret = clSetKernelArg(_deviceData.kernel,
                                         i + 1,
                                         args.optional[i].TotalSize(),
                                         args.optional[i].ref);

        if (_deviceData.ret != CL_SUCCESS)
        {
            std::cout << "Failed to set kernel argument at ";
            std::cout << i << "\n";
            clReleaseMemObject(out_mem_obj);
            return false;
        }
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    size_t global;  // global domain size for our calculation
    size_t local;   // local domain size for our calculation
    _deviceData.ret = clGetKernelWorkGroupInfo(_deviceData.kernel,
                                               _deviceData.device_id,
                                               CL_KERNEL_WORK_GROUP_SIZE,
                                               sizeof(local),
                                               &local,
                                               NULL);
    if (_deviceData.ret != CL_SUCCESS)
    {
        std::cout << "Failed to retrieve kernel work group info "
                  << _deviceData.ret << "\n";
        clReleaseMemObject(out_mem_obj);
        return false;
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    global = args.output.count;
    _deviceData.localGroupSize = local;

    _deviceData.ret = clEnqueueNDRangeKernel(_deviceData.command_queue,
                                             _deviceData.kernel,
                                             1,
                                             NULL,
                                             &global,
                                             &local,
                                             0, NULL,
                                             NULL);
    if (_deviceData.ret)
    {
        std::cout << "Failed to execute kernel\n";
        clReleaseMemObject(out_mem_obj);
        return false;
    }
    clFinish(_deviceData.command_queue);

    _deviceData.ret = clEnqueueReadBuffer(_deviceData.command_queue,
                                          out_mem_obj,
                                          CL_TRUE,
                                          0,
                                          args.output.TotalSize(),
                                          args.output.ref,
                                          0,
                                          NULL,
                                          NULL);
    if (_deviceData.ret != CL_SUCCESS)
    {
        std::cout << "Failed to read output array - ";
        std::cout <<_deviceData.ret << "\n";
        clReleaseMemObject(out_mem_obj);
        return false;
    }
    _deviceData.ret = clReleaseMemObject(out_mem_obj);
    return true;
}
