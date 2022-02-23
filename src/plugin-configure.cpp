#include <plugin-configure.h>
#include <dicom-tag.h>
#include <iostream>

nlm::json PluginConfigurer::config;
DicomAnonymizer PluginConfigurer::filter;
nlm::json PluginConfigurer::hardlinks;

int PluginConfigurer::Initialize() {
    try {
        config = nlm::json::parse(OrthancPluginGetConfiguration(globals::context));
        hardlinks = config.at("DataAnon").at("Hardlinks");
        if (config["StorageDirectory"].is_string()) {
            globals::storage_location = config["StorageDirectory"].get<std::string>();
            fs::create_directories(globals::storage_location);
            fs::permissions(globals::storage_location, globals::dir_permissions);
        } else {
            DEBUG_LOG(PLUGIN_ERRORS, "Configuration json does not contain a StorageDirectory field.");
            return -1;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return DicomAnonymizer::Configure(config);
}

void PluginConfigurer::UnitTestInitialize(nlm::json &cfg) {
    DicomAnonymizer::Configure(cfg);
    config = cfg;
    try {
        if (cfg["StorageDirectory"].is_string()) {
            globals::storage_location = cfg["StorageDirectory"].get<std::string>();
            fs::create_directories(globals::storage_location);
            fs::permissions(globals::storage_location, globals::dir_permissions);
        } else {
            DEBUG_LOG(PLUGIN_ERRORS, "Configuration json does not contain a StorageDirectory field.");
            return;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::string PluginConfigurer::GetDateFormat(tag_uint64_t tag) {
    auto dadt = config.at("DataAnon").at("DateTruncation");
    auto tag_key = HexToKey(DecToHex(tag, 4));
    if (dadt.contains(tag_key)) {
        return dadt.at(tag_key).get<std::string>();
    }
    return dadt.at("default").get<std::string>();
}
json_kv PluginConfigurer::GetHardlinks() {
    return hardlinks.items();
}
