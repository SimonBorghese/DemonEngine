//
// Created by simon on 7/30/22.
//

#ifndef DEMONENGINE_DEMONLOG_H
#define DEMONENGINE_DEMONLOG_H
#include <stdio.h>

#define _alert(name, line, file) fprintf(stdout, "[%s] [file %s at line %d] ", name, file, line)
#define ALERT(name) _alert(name, __LINE__, __FILE_NAME__);

#endif //DEMONENGINE_DEMONLOG_H
