#pragma once
#include <Windows.h>

#define GE_SELECT_OPTIMUM_GPU extern"C"{_declspec(dllexport)DWORD NvOptimusEnablement=0x00000001;_declspec(dllexport)DWORD AmdPowerXpressRequestHighPerformance=0x00000001;}

namespace GamEncin
{
    namespace Toolkit
    {
        // all the layers that can be used in the game
        enum Layer
        {
            Default, IgnoreRay, UI, Player, Layer0, Layer1, Layer2, Layer3, Layer4, Layer5, Layer6, Layer7, Layer8, Layer9
        };

        // all end types, exit codes that can be used in the game
        enum LogType
        {
            Safe, GLFWErr, GLADErr, ShaderCompilationErr, ShaderLinkingErr, ElementCouldNotFoundErr, TypeMismachErr, IOErr, IODeviceWarn, ProgramDuplicationErr, NullPointerErr, ElementDuplicationErr
        };

        class MeshBuilder
        {
            //TODO move all primitive shapes to this class and make a model importer

        };
    }
}

using namespace GamEncin::Toolkit;
