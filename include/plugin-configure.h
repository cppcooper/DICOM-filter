#pragma once
#include <core.h>
#include <dicom-anonymizer.h>
using json_kv = nlm::detail::iteration_proxy<nlm::basic_json<>::iterator>;

class PluginConfigurer {
private:
    static nlm::json config;
    static DicomAnonymizer filter;
    static std::unordered_map<tag_uint64_t,std::string> date_formats;
    static nlm::json hardlinks;
protected:
public:
    static int Initialize();
    static void UnitTestInitialize(nlm::json &cfg);
    static DicomAnonymizer& GetDicomFilter() { return filter; }
    static std::string GetDateFormat(uint64_t tag_uint64_t = 0);
    static json_kv GetHardlinks();
};