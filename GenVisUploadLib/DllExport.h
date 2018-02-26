#pragma once
#ifdef GENVISUPLAODLIB_EXPORTS
#define EXPORTED __declspec(dllexport)
#else
#define EXPORTED __declspec(dllimport)
#endif