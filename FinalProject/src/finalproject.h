#pragma once

#include "GLView.h"
#include <irrKlang.h>
#include <string>
#include <vector>
#include <optional>
#include "Vector.h"
#include "Mat4.h"

using namespace irrklang;

namespace Aftr
{
    class Camera;

    class GLViewNewModule : public GLView
    {
    public:
        static GLViewNewModule* New(const std::vector<std::string>& outArgs);
        virtual ~GLViewNewModule();
        virtual void updateWorld(); ///< Called once per frame
        virtual void loadMap(); ///< Called once at startup to build this module's scene
        virtual void createNewModuleWayPoints();
        virtual void transformball(Vector globalr, Vector localr, Vector translate, WO* cubewo);
        virtual void onResizeWindow(GLsizei width, GLsizei height);
        virtual void onMouseDown(const SDL_MouseButtonEvent& e);
        virtual void onMouseUp(const SDL_MouseButtonEvent& e);
        virtual void onMouseMove(const SDL_MouseMotionEvent& e);
        virtual void onKeyDown(const SDL_KeyboardEvent& key);
        virtual void onKeyUp(const SDL_KeyboardEvent& key);

        // New methods for networking
        void sendNetMsgSynchronousTCP(const std::string& action, const Vector& position, const Mat4& displayMat);
        void receiveNetMsgSynchronousTCP(std::string& action, Vector& position, Mat4& displayMat);

        bool hasReachedEndpoint();
        std::vector<std::string> args;  // Store command-line arguments
        void setAsServer();
        void setAsClient();

    protected:
        GLViewNewModule();
        virtual void onCreate();

        WO* startPoint;  // Point A
        WO* endPoint;    // Point B
        WO* playerBox;   // The box the player controls

        Vector startPos; // Starting position of the player box
        Vector endPos;   // End position (point B)
        bool isServer;
        std::vector<std::string> blocks_loc;
    };

} // namespace Aftr
