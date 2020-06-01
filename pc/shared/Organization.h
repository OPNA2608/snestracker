#pragma once

#define COMPANY_NAME "Black Hole Studios"
#define APP_NAME "SNES Tracker"
#define APP_VER_MAJ 0
#define APP_VER_MIN 1
#define APP_VER_MIC 0
#define APP_VER_STRING(maj, min, mic) #maj "." #min "." #mic
#define APP_VER_STRING2(maj, min, mic) APP_VER_STRING(maj, min, mic)
#define APP_VERSION APP_VER_STRING2(APP_VER_MAJ, APP_VER_MIN, APP_VER_MIC)
#define APP_NAME_VERSION APP_NAME " " APP_VERSION
