#ifndef OPENCLSYSTEM_H
#define OPENCLSYSTEM_H

#include <string>
#include <vector>
#include <CL/cl.h>


struct OpenclDeviceData
{
    cl_platform_id   platform_id = 0;
    cl_device_id     device_id = 0;
    cl_context       context;
    cl_command_queue command_queue;
    cl_uint          ret_num_devices;
    cl_uint          ret_num_platforms;
    cl_int           ret = CL_INVALID_CONTEXT;
    cl_program       program = 0;
    cl_kernel        kernel = 0;
    cl_uint          localGroupSize = 0;
};

#include <iostream>
struct KernelArguments
{
    struct Item
    {
        Item(void* ref, size_t size, size_t count = 1):
            ref(ref),
            size(size),
            count(count) {}

        size_t TotalSize() const {
            std::cout << "Total size:" <<size * count<<"\n";
            return size * count;
        }

        void* const ref;
        const size_t size;
        const size_t count;
    };

    KernelArguments(const Item& output, const std::vector<Item>& optional = {}):
        output(output),
        optional(optional) {}

    Item output;
    std::vector<Item> optional;
};


class OpenclSystem
{
public:
    static OpenclSystem& Get();

    bool Init();
    void Destroy();

    bool Compile(const std::string& code);
    bool Run(const std::string& functionName, const KernelArguments& args);

    void DumpDeviceData();

    inline const OpenclDeviceData& GetDeviceData() const { return _deviceData; }


private:
    OpenclSystem() = default;
    bool _wasInited = false;

    std::vector<std::pair<unsigned, void*>> _kernelArgs;
    OpenclDeviceData _deviceData;
    std::string _code;
};

#endif // OPENCLSYSTEM_H
