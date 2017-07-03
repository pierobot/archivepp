# archivepp

OS | Compiler | Status | Coverage
---|----------|--------|----------
Linux (Ubuntu 14.04)|g++ 4.8.4|[![Build Status](https://travis-ci.org/pierobot/archivepp.svg?branch=master)](https://travis-ci.org/pierobot/archivepp)|[![codecov](https://codecov.io/gh/pierobot/archivepp/branch/master/graph/badge.svg)](https://codecov.io/gh/pierobot/archivepp)|
Windows|MSVC 2015|[![Build status](https://ci.appveyor.com/api/projects/status/xh4vt4cuw4l4d6hg/branch/master?svg=true)](https://ci.appveyor.com/project/pierobot/archivepp/branch/master)| |

### Supported formats:
* zip
* rar

### Features
- One function to open any of the supported archive formats. (see below for example)
- Not restricted to zip and rar extensions.
    - Whitelist for linking extensions to formats.
    - Accepts files without an extension. If no extension is found, it will scan for known archive signatures.
- Unicode support for paths, filenames, and passwords on Windows.

### Example usage
```cpp
#include <archivepp/archive.hpp>

int main()
{
    archivepp::string path = ARCHIVEPP_STR("~/random/path/archive.zip");
    std::error_code error;
    auto archive_ptr = archivepp::archive::open(path, error);
    if (!archive_ptr || error)
        return 1;

    // Get contents by name
    auto contents = archive_ptr->get_contents(ARCHIVEPP_STR("archive/file"), error);
    if (error)
        return 1;

    // Do something with contents

    return 0;
}
```

### Requirements:
* zlib: http://zlib.net/
* libzip: https://nih.at/libzip/
* unrar: http://www.rarlab.com/rar_add.htm (UnRAR source)

