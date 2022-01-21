#pragma once
#include <pqxx/pqxx>
#include <dicom-file.h>

// todo: name better?
class DBInterface { // not an 'interface'
public:
    static void connect(std::string password);
    static void disconnect();
    static bool is_open();
    static void HandlePHI(const DicomFile &dicom);
    
    void UpdateChecksum(std::string uuid, int64_t size, char *hash);
};
