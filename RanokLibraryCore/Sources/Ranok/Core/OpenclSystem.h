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
    cl_int           ret;
    cl_program       program = 0;
    cl_kernel        kernel = 0;
    cl_uint          localGroupSize = 0;
};

struct KernelArguments
{
    struct Item
    {
        Item(void* ref, long size, long count = 1):
            ref(ref),
            size(size),
            count(count) {}

        unsigned TotalSize() const { return size * count; }

        void* const ref;
        const unsigned size;
        const unsigned count;
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

    void Compile(const std::string& code);
    bool Run(const std::string& functionName, const KernelArguments& args);

    inline const OpenclDeviceData& GetDeviceData() const { return _deviceData; };


private:
    OpenclSystem() = default;
    bool _wasInited = false;

    std::vector<std::pair<unsigned, void*>> _kernelArgs;
    OpenclDeviceData _deviceData;
    std::string _code;
};

#endif // OPENCLSYSTEM_H
